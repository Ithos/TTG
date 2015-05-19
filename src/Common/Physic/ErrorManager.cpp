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

#include "ErrorManager.h"

#include <log.h>

namespace Common
{
	using namespace Physic;

	const char* const LOG_CPxErrorMng = "Common::Physic::CErrorManager";

	void CErrorManager::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		char* codeStr = "";

		switch(code) {
		case physx::PxErrorCode::eNO_ERROR: codeStr = "NO_ERROR"; break;
		case physx::PxErrorCode::eDEBUG_INFO: codeStr = "DEBUG_INFO"; break;
		case physx::PxErrorCode::eDEBUG_WARNING: codeStr = "DEBUG_WARNING"; break;
		case physx::PxErrorCode::eINVALID_PARAMETER: codeStr = "INVALID_PARAMETER"; break;
		case physx::PxErrorCode::eINVALID_OPERATION: codeStr = "INVALID_OPERATION"; break;
		case physx::PxErrorCode::eOUT_OF_MEMORY: codeStr = "OUT_OF_MEMORY"; break;
		case physx::PxErrorCode::eINTERNAL_ERROR: codeStr = "INTERNAL_ERROR"; break;
		case physx::PxErrorCode::eABORT: codeStr = "ABORT"; break;
		case physx::PxErrorCode::ePERF_WARNING: codeStr = "PERF_WARNING"; break;
		}

		log_error(LOG_CPxErrorMng,"File: %s Line: %i Code: %s %s\n",file,line,codeStr,message);
	}
}