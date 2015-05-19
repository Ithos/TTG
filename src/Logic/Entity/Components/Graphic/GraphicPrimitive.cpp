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

#include "GraphicPrimitive.h"

#include <cassert>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include "../../../Scene/Scene.h"

#include "../Movement/Transform.h"
#include "../../Entity.h"

#include <Common/Data/Spawn_Constants.h>
#include <Common/Data/TTG_Types.h>

#include <Common/Map/MapEntity.h>

namespace Logic
{
	namespace Component
	{
		IMP_FACTORY(CGraphicPrimitive);
		CGraphicPrimitive::~CGraphicPrimitive()
		{
			if(m_ogreNode){
				m_ogreNode->detachAllObjects();
				m_ogreScene->destroySceneNode(m_ogreNode);
				m_ogreNode = nullptr;
			}
			if(m_ogreEntity){
				m_ogreScene->destroyEntity(m_ogreEntity);
				m_ogreEntity = nullptr;
			}
		}

		bool CGraphicPrimitive::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
		{
			if(!IComponent::spawn(entity,scene,entityInfo)) return false;

			using namespace Common::Data;
			using namespace Common::Data::Spawn;

			assert(entityInfo->hasAttribute(PHYSIC_ENTITY));

			//if(entityInfo->hasAttribute("physic_file")) return CPhysicActor::spawn(entity,scene,entityInfo);

			m_ogreScene = scene->getSceneManager();

			std::string shape (entityInfo->getStringAttribute(PHYSIC_ENTITY));

			std::string model ("");
			float scale (0.0f);

			if(entityInfo->hasAttribute("model"))
				model = entityInfo->getStringAttribute("model");

			if(entityInfo->hasAttribute("scale")){
				scale = entityInfo->getFloatAttribute("scale");
			}

			if(shape == "box"){
				assert(entityInfo->hasAttribute(PHYSIC_DIMENSION));

				const Vector3 pos = 
					(static_cast<CTransform*>( m_entity->getComponentByName(TRANSFORM_COMP)))->getPosition();

				const Vector3 dim = entityInfo->getVector3Attribute(PHYSIC_DIMENSION);

				try {
					m_ogreEntity = m_ogreScene->createEntity(Ogre::SceneManager::PrefabType::PT_CUBE);
				} catch (std::exception e) {
					return false;
				}

				m_ogreNode = m_ogreScene->getRootSceneNode()->createChildSceneNode(m_entity->getName() + "_prim_node");
				m_ogreNode->attachObject(m_ogreEntity);

				return true;
			}

			if(shape == "sphere"){
				assert(entityInfo->hasAttribute(PHYSIC_RADIUS));

				const Vector3 pos = 
					(static_cast<CTransform*>( m_entity->getComponentByName(TRANSFORM_COMP)))->getPosition();

				float radius = entityInfo->getFloatAttribute(PHYSIC_RADIUS);

				try {
					//m_ogreEntity = m_ogreScene->createEntity(Ogre::SceneManager::PrefabType::PT_SPHERE);
					m_ogreEntity = m_ogreScene->createEntity(m_entity->getName()+"_primitive",model);
				} catch (std::exception e) {
					return false;
				}

				m_ogreNode = m_ogreScene->getRootSceneNode()->createChildSceneNode(m_entity->getName() + "_prim_node");
				m_ogreNode->attachObject(m_ogreEntity);

				CTransform* t_target = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));
				m_ogreNode->setPosition(t_target->getPosition());
				//m_ogreNode->setOrientation(t_target->getOrientation());

				//m_ogreNode->setScale(Ogre::Real(radius/10),Ogre::Real(radius/10),Ogre::Real(radius/10));
				//m_ogreNode->setScale(20,20,20);
				m_ogreNode->setScale(Ogre::Real(scale),Ogre::Real(scale),Ogre::Real(scale));

				return true;
			}

			if ((entityInfo->hasAttribute("static")) && (entityInfo->getBoolAttribute("static")))
				m_entity->getScene()->addStaticOgreNode(m_ogreNode);
			else
				m_entity->getScene()->addDynamicOgreNode(m_ogreNode);

			return false;
		}

		void CGraphicPrimitive::tick(unsigned int msecs)
		{
			if(!m_activate) return;
			IComponent::tick(msecs);
       
			CTransform* t_target = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));
		    m_ogreNode->setPosition(t_target->getPosition());
		    //m_ogreNode->setOrientation(t_target->getOrientation());
		}

		bool CGraphicPrimitive::activate()
		{
			if(!IComponent::activate()) return false;

			if(m_ogreEntity) m_ogreEntity->setVisible(true);

			return true;
		}

		void CGraphicPrimitive::deactivate()
		{
			IComponent::deactivate();
			if(m_ogreEntity) m_ogreEntity->setVisible(false);
		}

	}
}