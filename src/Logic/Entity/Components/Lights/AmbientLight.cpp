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

#include "AmbientLight.h"

#include <OgreSceneManager.h>

#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include <Common/Map/MapEntity.h>

namespace Logic
{
	namespace Component
	{
		IMP_FACTORY(CAmbientLight);
		bool CAmbientLight::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
		{
			if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

			m_ogreScene = m_entity->getScene()->getSceneManager();

			if(entityInfo->hasAttribute("colourValue"))
			{
				Ogre::Vector3 tmpVec(entityInfo->getVector3Attribute("colourValue"));
				m_ogreScene->setAmbientLight(Ogre::ColourValue(tmpVec.x,tmpVec.y,tmpVec.z));
			}else{
				m_ogreScene->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
			}

			return true;
		}
	}
}