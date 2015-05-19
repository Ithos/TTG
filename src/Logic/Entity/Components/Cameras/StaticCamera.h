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

#ifndef __LOGIC_STATICCAMERA_H
#define __LOGIC_STATICCAMERA_H

#include "../Component.h"
#include <OgreVector3.h>

namespace Ogre
{
	class Camera;
	class SceneManager;
	class SceneNode;
    class Viewport;
}

namespace Logic
{
	namespace Component
	{
		class CStaticCamera : public IComponent
		{
			DEC_FACTORY(CStaticCamera);
		public:
			CStaticCamera() : IComponent(), m_camera(nullptr), m_viewPort(nullptr), m_camName(""), m_camNear(1.0f), m_camFar(10000.0f), m_FOV(60.0f),
				m_isLookAt(false), m_nodeName("")
			{}
			virtual ~CStaticCamera();

			bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);
			virtual void deactivate();
			virtual bool activate();

        private:
            Ogre::Camera*   m_camera;
            Ogre::Viewport* m_viewPort;
            std::string     m_camName;
            std::string     m_nodeName;
			float m_camNear, m_camFar, m_FOV;
			bool m_isLookAt;
			Ogre::Vector3 m_lookAtPos;
		};

	REG_FACTORY(CStaticCamera);
	}
}
#endif