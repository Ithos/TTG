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


#undef blog_trace
#undef blog_debug
#undef blog_info
#undef blog_warning
#undef blog_error
#undef blog_fatal
#undef blog_set_up

#ifndef __COMMON_BLOG_H
#define __COMMON_BLOG_H

#include <boost\log\core.hpp>
#include <boost\log\trivial.hpp>
#include <boost\log\expressions.hpp>
#include <boost\log\utility\setup\file.hpp>

const char* const LOG_FILE = "ttg.log";

#ifdef NDEBUG

#define blog_trace ((void)0)
#define blog_debug ((void)0)
#define blog_info ((void)0)
#define blog_warning ((void)0)
#define blog_error ((void)0)
#define blog_fatal ((void)0)
#define blog_set_up ((void)0)

#else

#define blog_trace Common::BLog::trace_
#define blog_debug Common::BLog::debug_
#define blog_info Common::BLog::info_
#define blog_warning Common::BLog::warning_
#define blog_error Common::BLog::error_
#define blog_fatal Common::BLog::fatal_
#define blog_set_up Common::BLog::initDefaultLog

#endif

namespace Common
{
	namespace BLog
	{
		namespace Level
		{
			enum TLogLevel
			{
				TRACE = 0,
				DEBUG = 1,
				INFO = 2,
				WARNING = 3,
				ERROR_ = 4,
				FATAL = 5
			};
		}
		void changeLogLevel(Level::TLogLevel level);
		void initDefaultLog(const char* fileName = 0,Level::TLogLevel level = Level::FATAL, 
								bool onFile = true);
		void trace_(const char* id, const std::string & msg);
		void debug_(const char* id, const std::string & msg);
		void info_(const char* id, const std::string & msg);
		void warning_(const char* id, const std::string & msg);
		void error_(const char* id, const std::string & msg);
		void fatal_(const char* id, const std::string & msg);
	}
}

#endif