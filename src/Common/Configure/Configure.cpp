/*
 	Copyright 2015 Francisco Javier Martinez Garcia
	Copyright 2015 Alvaro Perez Corral
	Copyright 2015 Luis Valero Martin
	Copyright 2015 Adrian Vizcaino Gonzalez
	This file is part of Through the galaxy.
	Through the galaxy is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	any later version.
	Through the galaxy is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with Through the galaxy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Configure.h"

#include <log.h>

#include <fstream>

namespace Common
{
	namespace Configuration
	{

		const char* const LOG_CONFIGURE = "Common::Configuration";

		typedef std::map<std::string, CConfigure*> TFilesMap;

		static std::string defaultFile (CONFIGURE_FILE);
		static TFilesMap fm;

		bool Common::Configuration::setDefaultFile(const char* path)
		{
			if((path == 0) || (path[0] == 0)) return false;

			defaultFile = path;
			return true;
		}

		std::string Common::Configuration::getDefaultValue(const std::string &name)
		{
			return (CConfigure::getConfiguration().getValue(name));
		}

		CConfigure& CConfigure::getConfiguration(const char* path)
		{
			std::string spath (defaultFile);
			if(path != 0)
				spath = path;
			return (CConfigure::getConfiguration(spath));
		}

		CConfigure& CConfigure::getConfiguration(const std::string &path)
		{
			if(fm.find(path) == fm.end()){
				CConfigure* cf = new CConfigure(path);
				fm.insert(std::pair<std::string, CConfigure*> (path,cf));
			}

			CConfigure* ret (fm[path]);
			if( ret == 0 ) log_fatal(LOG_CONFIGURE, "File not found!\n");

			return (*ret);
		}

		CConfigure::CConfigure(const std::string &path):m_path(path),
														m_configurationValues()
		{
			using std::string;
			using std::ifstream;

			ifstream in (m_path.c_str());

			while(in){
				char start = in.peek();
				if((start == '#') || (start == '\n')){
					string comment;
					getline(in,comment);
				} else {
					string key, value;
					getline(in,key,'=');
					getline(in,value);
					this->m_configurationValues[key] = value;
				}
			}
		}

		bool CConfigure::checkProperty(const std::string &key) const
		{
			return (this->m_configurationValues.find(key) != 
				this->m_configurationValues.end());
		}

		std::string CConfigure::getValue(const std::string &key) const
		{
			if(this->checkProperty(key)){
				std::string value (this->m_configurationValues.find(key)->second);
				return (value);
			} else {
				log_error(LOG_CONFIGURE,"Property not found!\n");
				return ("");
			}
		}

		void removeConfiguration(const char* path)
		{
			if(path){
				TFilesMap::const_iterator it (fm.find(path));
				if( it != fm.end() ){
					delete it->second;
					fm.erase(it);
				}
			} else {
				TFilesMap::const_iterator it (fm.begin());
				while ( it != fm.end() ){
					delete it->second;
					++it;
				}
				fm.clear();
			}
		}
	}
}