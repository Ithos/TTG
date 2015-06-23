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

#include "Language.h"

#include <fstream>
#include <map>

namespace Common { namespace Language
{

	const char* const LNG_CONG_FILE = "./conf/lng.conf";
	const char* const LNG_PATH = "./lng/";

	static std::string LANGUAGE ("en");

	static std::map<std::string,std::string> languageM;

	std::string getString(const std::string& tag)
	{
		if(languageM.find(tag) != languageM.end()){
			std::string ret (languageM.find(tag)->second);
			return (ret);
		}

		return ("");
	}

	void loadLng(std::ifstream& in)
	{
		languageM.clear();

		while(in){
			char start = in.peek();
			if((start == '#') || (start == '\n')){
				std::string comment;
				getline(in,comment);
			} else {
				std::string tag ("");
				std::string val ("");

				getline(in,tag);
				getline(in,val);

				languageM.insert(std::pair<std::string,std::string>(tag,val));
			}
		}

	}

	bool changeLanguage(const std::string& lng)
	{
		std::string path (LNG_PATH);
		path += lng;

		std::ifstream in (path);
		
		if(!in) return (false);

		loadLng(in);

		in.close();

		return (true);
	}
}}