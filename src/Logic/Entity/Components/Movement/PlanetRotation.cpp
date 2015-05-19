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
#include "PlanetRotation.h"

#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include <Common/Map/MapEntity.h>
#include <Common/Data/TTG_Types.h>

#include <OgreSceneNode.h>

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CPlanetRotation);

	bool CPlanetRotation::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

		if(entityInfo->hasAttribute("yaw"))
			m_rot.x = entityInfo->getFloatAttribute("yaw");

		if(entityInfo->hasAttribute("pitch"))
			m_rot.y = entityInfo->getFloatAttribute("pitch");

		if(entityInfo->hasAttribute("roll"))
			m_rot.z = entityInfo->getFloatAttribute("roll");

		m_node = scene->getSceneManager()->getSceneNode(m_entity->getName() + "_node");

		return true;
	}

	void CPlanetRotation::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(m_rot.x != 0)
		{
			m_node->yaw(Ogre::Radian(m_acumRot.x),Ogre::Node::TS_WORLD);
			m_acumRot.x +=  m_rot.x * msecs/1000;
		}
		else if(m_rot.y != 0)
		{
			m_node->pitch(Ogre::Radian(m_acumRot.y),Ogre::Node::TS_WORLD);
			m_acumRot.y +=  m_rot.y * msecs/1000;
		}
		else if(m_rot.z != 0)
		{
			m_node->roll(Ogre::Radian(m_acumRot.z),Ogre::Node::TS_WORLD);
			m_acumRot.z +=  m_rot.z * msecs/1000;
		}

	}

}