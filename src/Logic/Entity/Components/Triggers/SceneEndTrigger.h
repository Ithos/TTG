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

#ifndef __LOGIC_END_TRIGGER_H
#define __LOGIC_END_TRIGGER_H

#include "../Physic/PhysicEntity.h"

namespace Logic
{
	namespace Component
	{
		class CSceneEndTrigger : public CPhysicEntity
		{
			DEC_FACTORY(CSceneEndTrigger);
		public:

			CSceneEndTrigger()
			{}

			~CSceneEndTrigger()
			{}

			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

			virtual void onOverlapBegin(IPhysic* otherComponent);
		};

		REG_FACTORY(CSceneEndTrigger);

	}

}


#endif