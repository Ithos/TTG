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

#include "ShieldTrigger.h"
#include "../../Entity.h"
#include "common/Data/TTG_Types.h"
#include "Common/Map/MapEntity.h"
#include <Common/Util/PxConversor.h>
#include "Common/Data/Spawn_Constants.h"
#include "../../Components/Movement/Transform.h"
#include <PxPhysicsAPI.h>
#include "Logic/Scene/Scene.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace Logic
{
	namespace Component
	{
		
        IMP_FACTORY(CShieldTrigger)

        bool CShieldTrigger::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
			if (!CPhysicEntity::spawn(entity, scene, entityInfo))
                return false;

            m_parent = entity;
            m_trans  = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));

            if (entityInfo->hasAttribute(Common::Data::Spawn::GRAPHIC_MODEL)) {
                Ogre::Entity* ent = scene->getSceneManager()->createEntity("shield_model", entityInfo->getStringAttribute(Common::Data::Spawn::GRAPHIC_MODEL));
                ent->setMaterialName("Shield");
                m_sceneNode = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode("mandafurcias");
                m_sceneNode->attachObject(ent);
                m_sceneNode->setScale(Vector3(70));
            }

            return true;
        }

        void CShieldTrigger::tick(unsigned int msecs)
        {
            using namespace Common::Util::PxConversor;
            reinterpret_cast<physx::PxRigidDynamic*>(m_actor)->setKinematicTarget(Matrix4ToPxTransform(m_trans->getTransform()));
            m_sceneNode->setPosition(m_trans->getPosition());
        }

        void CShieldTrigger::onOverlapBegin(IPhysic* otherComponent)
        {
			if (otherComponent->getEntity()->getType() == "Asteroid") {
             // *(static_cast<CShield*>(m_parent->getComponentByName(Common::Data::SHIELD_COMP))->m_amount) -= 10;
            }
            else if (otherComponent->getEntity()->getType() == "Enemy" ) {
            
            }
        }

		void CShieldTrigger::onOverlapEnd(IPhysic* otherComponent)
        {
        }

    }
}