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

#include "CameraLookAt.h"

#include <OgreSceneNode.h>
#include <OgreCamera.h>

#include <Common/Map/MapEntity.h>

#include "../../Entity.h"
#include "../../../Scene/Scene.h"

#include "../Movement/Transform.h"

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CCameraLookAt);

	bool CCameraLookAt::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo)) return false;

		// ????
		if(!m_camera)
			return false;

		if(entityInfo->hasAttribute("position")){
			m_position = entityInfo->getVector3Attribute("position");
		}
		
		if(entityInfo->hasAttribute("lookAtPosition")){
			m_target = entityInfo->getVector3Attribute("lookAtPosition");
		}

		Vector3 orientation(0,0,0);
		if(entityInfo->hasAttribute("orientation")){
			orientation = entityInfo->getVector3Attribute("orientation");
		}

		{
			using namespace Ogre;
			SceneNode* cameraNode = m_camera->getParentSceneNode();
			cameraNode->setPosition(m_position);
			m_camera->lookAt(m_target);
		}

		return true;
	}

	void CCameraLookAt::tick(unsigned int msecs)
	{
		if(m_change){
			using namespace Ogre;
			SceneNode* cameraNode = m_camera->getParentSceneNode();
			cameraNode->setPosition(m_position);
			m_camera->lookAt(m_target);
			m_change = false;
		}
	}

	void CCameraLookAt::changePosition(Vector3 pos)
	{
		m_change = true;
		m_position = pos;
	}

	void CCameraLookAt::changeTarget(Vector3 target)
	{
		m_change = true;
		m_target = target;
	}
}