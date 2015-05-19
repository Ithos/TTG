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

#ifndef __LOGIC_CAMERALOOKAT_H
#define __LOGIC_CAMERALOOKAT_H

#include "../Component.h"

#include <Common/Util/Math.h>

namespace Ogre
{
	class Camera;
	class SceneManager;
	class SceneNode;
}

namespace Logic
{
	namespace Component
	{
		class CCameraLookAt : public IComponent
		{
			DEC_FACTORY(CCameraLookAt);

		public:
			CCameraLookAt() : IComponent(),m_camera(nullptr),m_target(0,0,0),m_position(0,0,0),m_change(false){}
			virtual ~CCameraLookAt(){}
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);
			virtual void tick(unsigned int msecs);

			void changePosition(Vector3 pos);
			void changeTarget(Vector3 target);

		protected:
		private:

			Ogre::Camera* m_camera;
			Vector3 m_position;
			Vector3 m_target;
			bool m_change;
		};
		REG_FACTORY(CCameraLookAt);
	}
}

#endif