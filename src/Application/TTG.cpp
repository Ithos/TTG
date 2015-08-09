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

#ifdef _DEBUG

#include <sstream>
#include <time.h>

#define _VISUALLEAKDETECTOR

#ifdef _VISUALLEAKDETECTOR
//#include <vld.h>
#endif

#if (!defined _VISUALLEAKDETECTOR) && (defined _LEAKDETECTOR) && (defined _WIN32)
#include <crtdbg.h>
#include <assert.h>
#endif 

#endif

#include <log.h>
#include <csignal>
#include <string>

#include <Common/Log/log.h>

#include "TTGApplication.h"

#if (defined _DEBUG) || !(defined _WIN32)
int main(int argc, char **argv) 
{
#if (!defined _VISUALLEAKDETECTOR) && (defined _LEAKDETECTOR)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1);
#endif
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
int WINAPI
WinMain(HINSTANCE zhInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
#endif
	{
		const char* const logFile = "./conf/log.conf";
		Application::CTTGApplication* ttg(new Application::CTTGApplication());
		try{
			
#ifdef _DEBUG
			std::stringstream ss;
			ss << "./log/ttg_" << time(0) <<".log";
			std::string log_file = ss.str();
#else
			std::string log_file = "";
#endif
			log_set_up(logFile,log_file.c_str(),true);
			blog_set_up(0,Common::BLog::Level::TRACE,false);
			log_trace("MAIN","Opening...\n");
			if(ttg->init()) 
				while(ttg->run()){
					ttg->release();
					delete ttg;
					ttg = new Application::CTTGApplication();
					ttg->init();
				}
			ttg->release();
			delete ttg;
			log_trace("MAIN","Closing...\n");
		} catch(std::runtime_error e){
			//Needed for the log
			//ttg.release();
			exit(-1);
		}
		//Needed for the log
		try{
		std::raise(SIGTERM);
		} catch(...){

		}
	}
	return 0;
}