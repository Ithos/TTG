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

#include "RTTCamera.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreTextureManager.h>
#include <OgreTexture.h>
#include <OgreRenderTexture.h>
#include <OgreHardwarePixelBuffer.h>

#include <Common/Map/MapEntity.h>
#include <Common\data\TTG_Types.h>
#include <Common/Data/Spawn_Constants.h>

#include "../../../Logic.h"
#include "../../../Scene/Scene.h"
#include "../../Entity.h"


using namespace Common::Data::Spawn;
using namespace Common::Data;

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CRTTCamera);

	CRTTCamera::~CRTTCamera()
    {
    }

	bool CRTTCamera::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity, scene, entityInfo))
			return false;

		if(entityInfo->hasAttribute(STATIC_CAMERA_NAME))
			m_camName = entityInfo->getStringAttribute(STATIC_CAMERA_NAME);

		m_nodeName = m_camName + "node_camera";

		m_textureName = m_camName + "render_texture";

		if(entityInfo->hasAttribute(STATIC_CAMERA_NEAR))
			m_camNear = entityInfo->getFloatAttribute(STATIC_CAMERA_NEAR);

		if(entityInfo->hasAttribute(STATIC_CAMERA_FAR))
			m_camFar = entityInfo->getFloatAttribute(STATIC_CAMERA_FAR);

		if(entityInfo->hasAttribute(STATIC_CAMERA_FOV))
			m_FOV = entityInfo->getFloatAttribute(STATIC_CAMERA_FOV);

		if(entityInfo->hasAttribute(COMMON_POSITION))
			m_pos = entityInfo->getVector3Attribute(COMMON_POSITION);

		if(entityInfo->hasAttribute(STATIC_CAMERA_LOOKAT))
			m_lookAt = entityInfo->getVector3Attribute(STATIC_CAMERA_LOOKAT);

		if(entityInfo->hasAttribute(TEXTURE_CAMERA_ORTHO))
		{
			m_ortho = entityInfo->getBoolAttribute(TEXTURE_CAMERA_ORTHO);

			if(entityInfo->hasAttribute(TEXTURE_CAMERA_ORTHO_WIDTH))
				m_orthoWidth = entityInfo->getFloatAttribute(TEXTURE_CAMERA_ORTHO_WIDTH);

			if(entityInfo->hasAttribute(TEXTURE_CAMERA_ORTHO_HEIGHT))
				m_orthoHeight = entityInfo->getFloatAttribute(TEXTURE_CAMERA_ORTHO_HEIGHT);
		}
				
		return true;
	}

	bool CRTTCamera::activate()
	{
        m_activate = true;

		m_camNode = m_entity->getScene()->getSceneManager()->getRootSceneNode()->createChildSceneNode(m_nodeName);
        m_camera  = m_entity->getScene()->getSceneManager()->createCamera(m_camName);
		m_camera->setAutoAspectRatio(true);

		if(!m_camera)
			return false;

        // Set attributes
        m_camera->setNearClipDistance(m_camNear);
        m_camera->setFarClipDistance(m_camFar);
		if(m_ortho){
			m_camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
			m_camera->setOrthoWindow(m_orthoWidth,m_orthoHeight);
		}else{
			m_camera->setFOVy(Ogre::Radian(Ogre::Degree(m_FOV)));
		}

        m_camNode->attachObject(m_camera);

		m_camNode->setPosition(m_pos);
		m_camNode->lookAt(m_lookAt, Ogre::Node::TransformSpace::TS_WORLD);

		Ogre::TexturePtr tex = m_entity->getScene()->getRootTextureManager()->createManual(
			m_textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
			512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET
			);

		Ogre::RenderTexture *rtex = tex->getBuffer()->getRenderTarget();

		m_viewPort = rtex->addViewport(m_camera);
		m_viewPort->setOverlaysEnabled(false);
		m_viewPort->setClearEveryFrame(true);
		m_viewPort->setBackgroundColour(Ogre::ColourValue::Black);
		m_viewPort->setVisibilityMask(Common::Data::ONLY_TEXTURE_MASK);

		rtex->setAutoUpdated(true);

		return true;
	}

	void CRTTCamera::deactivate()
	{
        m_activate = false;

		if (m_viewPort)
            m_entity->getScene()->getRenderWindow()->removeViewport(m_viewPort->getZOrder());

		m_entity->getScene()->getRootTextureManager()->remove(m_textureName);

		Ogre::SceneNode* node = m_entity->getScene()->getSceneManager()->getSceneNode(m_nodeName);
        node->detachAllObjects();
        m_entity->getScene()->getSceneManager()->destroyCamera(m_camera);
        m_entity->getScene()->getSceneManager()->destroySceneNode(node);
	}
}