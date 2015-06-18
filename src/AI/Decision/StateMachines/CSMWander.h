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

#ifndef __AI_SMWANDER_H
#define __AI_SMWANDER_H

#include "StateMachine.h"
#include "Condition.h"
#include "AI/Decision/LatentActions/LAGoTo.h"
#include "AI/Decision/LatentActions/SimpleLatentActions.h"

namespace AI
{
	class CSMWander : public CStateMachine<CLatentAction>
	{
	public:
		/*
		Constructor. Añade los nodos y las aristas, establece el nodo de inicio
		y deja la máquina de estado lista para ser ejecutada.
		*/
		CSMWander(Logic::CEntity* entity) : CStateMachine(entity) {
			int routeTo = this->addNode(new CLAShoottingGoTo(entity,50));
			int wait = this->addNode(new CLAWait(3000));
			this->addEdge(routeTo, wait, new CConditionSuccess());
			this->addEdge(routeTo, routeTo, new CConditionFail());
			this->addEdge(wait, routeTo, new CConditionFinished());
			this->setInitialNode(routeTo);
			this->resetExecution();
		}
	};

}

#endif
