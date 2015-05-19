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

#include "Camera.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

#include <Common/Map/MapEntity.h>
#include <Common\data\TTG_Types.h>
#include <Common/Data/Spawn_Constants.h>

#include "../../../Logic.h"
#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include "../Movement/Transform.h"
#include "../Movement/Movement.h"
#include "../Movement/Interpolation.h"

using namespace Common::Data::Spawn;
using namespace Common::Data;

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CCamera);

    CCamera::~CCamera()
    {
    }

	bool CCamera::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity, scene, entityInfo))
			return false;

        if (entityInfo->hasAttribute(STATIC_CAMERA_NAME))
            m_camName = entityInfo->getStringAttribute(STATIC_CAMERA_NAME);

        m_nodeName = m_camName + "node_camera";

        if (entityInfo->hasAttribute(CAMERA_NEAR))
            m_camNear = entityInfo->getFloatAttribute(CAMERA_NEAR);

        if (entityInfo->hasAttribute(CAMERA_FAR))
            m_camFar = entityInfo->getFloatAttribute(CAMERA_FAR);

        if (entityInfo->hasAttribute(CAMERA_FOV))
            m_FOV = entityInfo->getFloatAttribute(CAMERA_FOV);

        if (entityInfo->hasAttribute(CAMERA_HEIGHT)){
            m_height = entityInfo->getFloatAttribute(CAMERA_HEIGHT);

		 }

		if (entityInfo->hasAttribute(CAMERA_DIST_CONSTANT))
			m_distConstant = entityInfo->getFloatAttribute(CAMERA_DIST_CONSTANT);

		if (entityInfo->hasAttribute(CAMERA_MIN_DISTANCE))
			m_minDist = entityInfo->getFloatAttribute(CAMERA_MIN_DISTANCE);

		if(entityInfo->hasAttribute(CAMERA_ACCEL))
			m_accelConst = entityInfo->getFloatAttribute(CAMERA_ACCEL);
				
		return true;
	}

    bool CCamera::activate()
	{
        m_activate = true;
		m_camNode  = m_entity->getScene()->getSceneManager()->getRootSceneNode()->createChildSceneNode(m_nodeName);
        m_camera   = m_entity->getScene()->getSceneManager()->createCamera(m_name);

		if (!m_camera)
			return false;

        // Set attributes
        m_camera->setNearClipDistance(m_camNear);
        m_camera->setFarClipDistance(m_camFar);
        m_camera->setFOVy(Ogre::Radian(Ogre::Degree(m_FOV)));
        m_camNode->attachObject(m_camera);
        m_entity->getScene()->setSceneCamera(m_camera);

         // add to viewPort
        m_viewPort = m_entity->getScene()->getRenderWindow()->addViewport(m_camera);
		m_viewPort->setBackgroundColour(Ogre::ColourValue::Black);
		m_viewPort->setVisibilityMask(Common::Data::NON_TEXTURE_MASK);

		m_target = CLogic::getInstance()->getScene()->getEntityByName("Player");

		return true;
	}

	 void CCamera::deactivate() 
	 {
         m_activate = false;

         m_camNode->detachAllObjects();
         m_entity->getScene()->getSceneManager()->destroyCamera(m_camera);
         m_entity->getScene()->getSceneManager()->destroySceneNode(m_camNode);
         
         if (m_viewPort)
             m_entity->getScene()->getRenderWindow()->removeViewport(m_viewPort->getZOrder());
		 
         m_target = nullptr; 
	 }
	
	void CCamera::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (m_target) {
            if (!m_interpolation) {
                m_interpolation = static_cast<Logic::CInterpolation*>(m_target->getComponentByName(INTERPOLATION_COMP));
                m_controller    = static_cast<Logic::CMovement*>(m_target->getComponentByName(MOVEMENT_COMP));
            }
            Vector3 targetpoint = m_interpolation->getRenderTransform().getTrans();
         //   float factor = Common::Util::Math::easeOutElastic(1, 1, 1);

            /*m_camera->getParentSceneNode()->setPosition(targetpoint + Vector3(0, m_height, m_controller->getSpeed()*300+10));*/

			m_cameraDist += m_cameraVel * msecs*0.001;
			if(m_cameraDist < m_minDist) m_cameraDist = m_minDist;

			m_camera->getParentSceneNode()->setPosition(targetpoint + Vector3(0, m_height, m_cameraDist));

			m_camera->lookAt(targetpoint);

			float targetDist(m_controller->getSpeed() * m_distConstant + m_minDist);

			if(targetDist < 0)
				targetDist = -targetDist;

			float err(m_cameraDist - targetDist);

			m_cameraVel -= m_accelConst*err*msecs*0.001;

			if(m_cameraDist >= targetDist - 10 && m_cameraDist <= targetDist + 10)
				m_cameraVel = 0;
		}
	}

}
