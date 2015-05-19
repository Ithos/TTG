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

#include "DirectionalLight.h"

#include <OgreSceneManager.h>
#include <OgreLight.h>

#include <Common/Map/MapEntity.h>

#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include "../Movement/Transform.h"

namespace Logic
{
	namespace Component
	{
		IMP_FACTORY(CDirectionalLight);
		CDirectionalLight::~CDirectionalLight()
		{
			if(m_ogreLight)
				m_ogreScene->destroyLight(m_ogreLight);
		}

		bool CDirectionalLight::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
		{
			if(!IComponent::spawn(entity,scene,entityInfo))
				return false;

			m_ogreScene = m_entity->getScene()->getSceneManager();

			if(entityInfo->hasAttribute("lightStatic"))
				m_static = entityInfo->getBoolAttribute("lightStatic");

			m_lightName = m_entity->getName() + "_light";
			m_ogreLight = m_ogreScene->createLight(m_lightName);

			m_ogreLight->setType(Ogre::Light::LT_DIRECTIONAL);

			CTransform* t_target = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));
			if(!t_target)
				return false;
			m_ogreLight->setPosition(t_target->getPosition());
			m_ogreLight->setDirection(t_target->getOrientation().x,t_target->getOrientation().y,t_target->getOrientation().z);

			if(entityInfo->hasAttribute("diffuseColour")){
				Ogre::Vector3 tmpVec(entityInfo->getVector3Attribute("diffuseColour"));
				m_ogreLight->setDiffuseColour(tmpVec.x,tmpVec.y,tmpVec.z);
			}

			if(entityInfo->hasAttribute("specularColour")){
				Ogre::Vector3 tmpVec(entityInfo->getVector3Attribute("specularColour"));
				m_ogreLight->setSpecularColour(tmpVec.x,tmpVec.y,tmpVec.z);
			}
		

			return true;



		}

		void CDirectionalLight::tick(unsigned int msecs)
		{
			if(!m_static)
			{
				CTransform* t_target = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));
				m_ogreLight->setPosition(t_target->getPosition());
				m_ogreLight->setDirection(t_target->getOrientation().x,t_target->getOrientation().y,t_target->getOrientation().z);

			}
		}
	}

}

