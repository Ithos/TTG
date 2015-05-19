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

#ifndef __LOGIC_PLANET_ROTATION_H
#define __LOGIC_PLANET_ROTATION_H

#include "../Component.h"
#include <Common/Util/Math.h>

namespace Ogre
{
	class SceneNode;
}

namespace Logic
{
	namespace Component
	{
		class CPlanetRotation : public IComponent
		{
			DEC_FACTORY(CPlanetRotation);

		public:
			CPlanetRotation() : IComponent(), m_rot(0.0f,0.0f,0.0f), m_acumRot(0.0f,0.0f,0.0f), m_node(nullptr) {}
			virtual ~CPlanetRotation() {};
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);
			virtual void tick(unsigned int msecs);

		protected:
			Vector3 m_rot, m_acumRot;//yaw,pitch,roll  deg/s
			Ogre::SceneNode* m_node; 

		};

	REG_FACTORY(CPlanetRotation);
	}
}

#endif