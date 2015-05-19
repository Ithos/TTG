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

#ifndef __AI_STATEMACHINEFACTORY_H
#define __AI_STATEMACHINEFACTORY_H

#include "StateMachine.h"
#include "CSMDummy.h"
#include "CSMWander.h"
#include "AI/Decision/LatentActions/LatentAction.h"

namespace AI
{
	/*
	Factoría que devuelve máquinas de estado predefinidas.
	*/
	class CStateMachineFactory 
	{
	public:
		static CStateMachine<CLatentAction>* getStateMachine(std::string smName, Logic::CEntity* entity)
		{
			if (smName == "dummy")	return new CSMDummy(entity);
			if (smName == "wander")	return new CSMWander(entity);

			return 0;
		}
	};
}

#endif
