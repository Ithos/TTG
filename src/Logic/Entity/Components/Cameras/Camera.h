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

#ifndef __LOGIC_CAMERA_H
#define __LOGIC_CAMERA_H

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
        static const unsigned MAX_AMPLITUDE = 4;
        class CInterpolation;
        class CMovement;

		class CCamera : public IComponent
		{
			DEC_FACTORY(CCamera);

		public:
            CCamera() : 
                IComponent(), m_camera(nullptr), m_height(490), m_interpolation(nullptr), m_controller(nullptr), 
                m_viewPort(nullptr), m_camName(""), m_nodeName(""), m_camNear(1.0f), m_camFar(10000.0f),
                m_FOV(60.0f), m_cameraVel(0), m_cameraDist(0), m_distConstant(700), m_minDist(10), m_accelConst(1.0)
			{ }
			
            ~CCamera();
			bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

			bool activate();
            void deactivate();

			void tick(unsigned int msecs);

            static void shake() { 
                m_shake = true; 
                m_amplitude = 2.0;
                m_timeShaking = 0;
            }

            static bool isShaking() { return m_shake; }

            static void resetShake()
            {
                m_timeShaking = 0;
                m_amplitude   = 4.0f;
                m_valShake    = 0.0f;
                m_shake       = false;
            }

            static void resetAndIncrementShake()
            {
                resetShake();
                if (m_amplitude < MAX_AMPLITUDE)
                    m_amplitude += 1;
            }

		protected:
			/**
			Ogre camera
			*/
			Ogre::Camera* m_camera;

			/**
			entity to be followed by the camera
			*/
			CEntity* m_target;

			/**
			Distance between the camera and the target entity
			*/
			float m_height;

            CInterpolation*  m_interpolation;
            CMovement*     m_controller;
            Ogre::Viewport*  m_viewPort;

            std::string m_nodeName;
            std::string m_camName;

			float m_camNear;
            float m_camFar;
            float m_FOV;
            float m_friction; // temporal, luego se quitara por otra cosa
            static bool m_shake;
            static unsigned m_timeShaking;
            static float m_amplitude;
            static float m_valShake;

            Ogre::SceneNode* m_camNode;

			float m_cameraVel, m_cameraDist, m_accelConst;
			int m_distConstant, m_minDist;
		};

	REG_FACTORY(CCamera);

	}
}

#endif