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

#include "log.h"

namespace Common
{
	namespace BLog
	{
		void changeLogLevel(Level::TLogLevel level)
		{
			switch(level)
			{
			case Level::TRACE:
				{
					boost::log::core::get()->set_filter
					(
					boost::log::trivial::severity >= boost::log::trivial::trace
					);
					break;
				}
			case Level::DEBUG:
				{
					boost::log::core::get()->set_filter
					(
					boost::log::trivial::severity >= boost::log::trivial::debug
					);
					break;
				}
			case Level::INFO:
				{
					boost::log::core::get()->set_filter
					(
					boost::log::trivial::severity >= boost::log::trivial::info
					);
					break;
				}
			case Level::WARNING:
				{
					boost::log::core::get()->set_filter
					(
					boost::log::trivial::severity >= boost::log::trivial::warning
					);
					break;
				}
			case Level::ERROR_:
				{
					boost::log::core::get()->set_filter
					(
					boost::log::trivial::severity >= boost::log::trivial::error
					);
					break;
				}
			case Level::FATAL:
				{
					boost::log::core::get()->set_filter
					(
					boost::log::trivial::severity >= boost::log::trivial::fatal
					);
					break;
				}
			}
		}

		void initDefaultLog(const char* fileName, Level::TLogLevel level, bool onFile)
		{
			if(onFile)
				if(!fileName) {
					boost::log::add_file_log(LOG_FILE);
				} else {
					boost::log::add_file_log(fileName);
				}
			changeLogLevel(level);
		}

		void trace_(const char* id, const std::string & msg)
		{
			BOOST_LOG_TRIVIAL(trace) << id << " " << msg;
		}

		void debug_(const char* id, const std::string & msg)
		{
			BOOST_LOG_TRIVIAL(debug) << id << " " << msg;
		}

		void info_(const char* id, const std::string & msg)
		{
			BOOST_LOG_TRIVIAL(info) << id << " " << msg;
		}

		void warning_(const char* id, const std::string & msg)
		{
			BOOST_LOG_TRIVIAL(warning) << id << " " << msg;
		}

		void error_(const char* id, const std::string & msg)
		{
			BOOST_LOG_TRIVIAL(error) << id << " " << msg;
		}

		void fatal_(const char* id, const std::string & msg)
		{
			BOOST_LOG_TRIVIAL(fatal) << id << " " << msg;
		}
	}
}