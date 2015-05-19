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

#ifndef __LOGIC_ROTATION_H
#define __LOGIC_ROTATION_H

#include "../Component.h"
#include <Common/Util/Math.h>

namespace Logic
{
	namespace Component
	{
		class CRotation : public IComponent
		{
			DEC_FACTORY(CRotation);

		public:
			CRotation() : IComponent(), m_rot(0.0f,0.0f,0.0f) {}
			virtual ~CRotation() {};
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);
			virtual void tick(unsigned int msecs);

		protected:
			Vector3 m_rot;//yaw,pitch,roll  deg/s
		};

	REG_FACTORY(CRotation);
	}
}

#endif
