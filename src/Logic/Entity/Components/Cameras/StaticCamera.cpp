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

#include "StaticCamera.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>

#include <Common/Map/MapEntity.h>
#include <Common/Data/Spawn_Constants.h>
#include <Common/Data/TTG_Types.h>

#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include "../Movement/Transform.h"

using namespace Common::Data::Spawn;
using namespace Common::Data;

namespace Logic
{
	using namespace Component;
    IMP_FACTORY(CStaticCamera);

	CStaticCamera::~CStaticCamera()
	{
        
    }

	bool CStaticCamera::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

		if(entityInfo->hasAttribute(STATIC_CAMERA_NAME))
			 m_camName = entityInfo->getStringAttribute(STATIC_CAMERA_NAME);
		m_nodeName = m_camName + "node_camera";

		if(entityInfo->hasAttribute(STATIC_CAMERA_NEAR))
			m_camNear = entityInfo->getFloatAttribute(STATIC_CAMERA_NEAR);

		if(entityInfo->hasAttribute(STATIC_CAMERA_FAR))
			m_camFar = entityInfo->getFloatAttribute(STATIC_CAMERA_FAR);

		if(entityInfo->hasAttribute(STATIC_CAMERA_FOV))
			m_FOV = entityInfo->getFloatAttribute(STATIC_CAMERA_FOV);

		if (entityInfo->hasAttribute(STATIC_CAMERA_LOOKAT)){
			m_lookAtPos = entityInfo->getVector3Attribute(STATIC_CAMERA_LOOKAT);
			m_isLookAt = true;
		}
		
		return true;
	}

	 bool CStaticCamera::activate()
	 {
         m_activate = true;

		 //create camera
        Ogre::SceneNode* node = m_entity->getScene()->getSceneManager()->getRootSceneNode()->createChildSceneNode(m_nodeName);
        m_camera = m_entity->getScene()->getSceneManager()->createCamera(m_camName);

        if (!m_camera)
            return false;

        m_camera->setNearClipDistance(m_camNear);
        m_camera->setFarClipDistance(m_camFar);
        m_camera->setFOVy(Ogre::Radian(Ogre::Degree(m_FOV)));
        node->attachObject(m_camera);
        m_entity->getScene()->setSceneCamera(m_camera);

        // add to viewPort
        m_viewPort = m_entity->getScene()->getRenderWindow()->addViewport(m_camera);
		m_viewPort->setBackgroundColour(Ogre::ColourValue::Black);
		m_viewPort->setVisibilityMask(Common::Data::NON_TEXTURE_MASK);

		Ogre::CompositorInstance* com = Ogre::CompositorManager::getSingleton().addCompositor(m_viewPort,"Bloom");
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_viewPort,"Bloom.compositor",true);
		com->setEnabled(true);

        // move camera
        Ogre::SceneNode* ogreNode = m_camera->getParentSceneNode();
        CTransform* comp = static_cast<Logic::CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));	
		ogreNode->setPosition(comp->getTransform().getTrans());
		ogreNode->setOrientation(comp->getTransform().extractQuaternion());

        if (m_isLookAt)
            m_camera->lookAt(m_lookAtPos);

		return true;
	 }

	void CStaticCamera::deactivate()
	{
        m_activate = false;

		 Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_viewPort,"Bloom.compositor",false);
		 Ogre::CompositorManager::getSingleton().removeCompositor(m_viewPort,"Bloom.compositor");

		Ogre::SceneNode* node = m_entity->getScene()->getSceneManager()->getSceneNode(m_nodeName);
        node->detachAllObjects();
        m_entity->getScene()->getSceneManager()->destroyCamera(m_camera);
        m_entity->getScene()->getSceneManager()->destroySceneNode(node);

        if (m_viewPort)
            m_entity->getScene()->getRenderWindow()->removeViewport(m_viewPort->getZOrder());
	}
}