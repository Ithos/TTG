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

#include "OrbitMovement.h"

#include <Common/Map/MapEntity.h>
#include <Common/Data/TTG_Types.h>

#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreSceneManager.h>

#include "../../Entity.h"
#include "../Movement/Transform.h"
#include "../../../Scene/Scene.h"

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(COrbitMovement);

	bool COrbitMovement::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo)) return false;

		if(entityInfo->hasAttribute("centralPosition")){
			m_center = entityInfo->getVector3Attribute("centralPosition");
		}

		if(entityInfo->hasAttribute("angle")){
			m_angle = entityInfo->getIntAttribute("angle");
		}

		if(entityInfo->hasAttribute("distance")){
			m_radius = entityInfo->getFloatAttribute("distance");
		}

		if(entityInfo->hasAttribute("angularVelocity")){
			m_inc = entityInfo->getFloatAttribute("angularVelocity");
		}

		if(entityInfo->hasAttribute("centerNode")){
			m_nodeName = entityInfo->getStringAttribute("centerNode");
		}

		if(entityInfo->hasAttribute("direction")){
			m_dir = entityInfo->getBoolAttribute("direction");
		}

		CTransform* comp = static_cast<Logic::Component::CTransform*>
							(m_entity->getComponentByName("CTransform"));

		Vector3 pos = comp->getPosition();

		pos.x = m_radius * cos(m_angle) + m_center.x;
		pos.y = m_center.y;
		pos.z = m_radius * sin(m_angle) + m_center.z;

		comp->setPosition(pos);

		drawCircle(scene,scene->getSceneManager()->getSceneNode(m_nodeName + "_node"),std::string(entity->getName() + "_orbit").c_str());
 
		return true;
	}

	void COrbitMovement::tick(unsigned int msecs)
	{
		if(m_angle >= (2 * Common::Util::Math::PI)){
			m_angle = 0;
		} else {
			m_dir?m_angle += m_inc * msecs:m_angle -= m_inc * msecs;
		}

		CTransform* comp = static_cast<Logic::Component::CTransform*>
							(m_entity->getComponentByName("CTransform"));

		Vector3 pos = comp->getPosition();

		pos.x = m_radius * cos(m_angle) + m_center.x;
		pos.z = m_radius * sin(m_angle) + m_center.z;

		comp->setPosition(pos);
	}

	void COrbitMovement::drawCircle(CScene* scene, Ogre::SceneNode *centerNode, const char* str)
	{
		Ogre::ManualObject* circle = scene->getSceneManager()->createManualObject(str);  
		circle->begin("GreyLine",Ogre::RenderOperation::OT_LINE_STRIP);
		circle->setCastShadows(false);
		unsigned int accuracy = 35;
		unsigned int index = 0;
		for(float theta = 0; theta <= 2 * Ogre::Math::PI; theta += Ogre::Math::PI / accuracy) {
			circle->position(m_radius * cos(theta) + m_center.x, m_center.y, m_radius * sin(theta) + m_center.z);
			circle->index(index++);
		}

		circle->index(0);
		circle->end();

		circle->setQueryFlags(Common::Data::ORBIT_MASK);

		Ogre::SceneNode* subNode = centerNode->createChildSceneNode(std::string(str)+"_subNode");
		subNode->setInheritScale(false);
		subNode->attachObject(circle);
	}
}