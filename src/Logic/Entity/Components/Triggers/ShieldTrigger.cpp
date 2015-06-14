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
#include "common/particles/ParticleManager.h"
#include "../Movement/Transform.h"
#include "../Gameplay/Life.h"
#include "Logic/Scene/Scene.h"
#include "../Gameplay/Shield.h"
#include <PxPhysicsAPI.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace Logic
{
	namespace Component
	{
        IMP_FACTORY(CShieldTrigger)

        CShieldTrigger::~CShieldTrigger()
        {
            m_parent = nullptr;
            m_particleMngr->releaseShield();
        }

        bool CShieldTrigger::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            using namespace Common::Data::Spawn;
			if (!CPhysicEntity::spawn(entity, scene, entityInfo))
                return false;

            m_parent       = entity;
            m_trans        = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
            m_particleMngr = Common::Particles::CParticleManager::getInstance();
            m_compShield   = static_cast<CShield*>(m_parent->getComponentByName(SHIELD_COMP));
            m_scene        = scene;

            if (entityInfo->hasAttribute(SHIELD_TIMEOUT))
               m_secsNoShield = entityInfo->getIntAttribute(SHIELD_TIMEOUT);

            if (entityInfo->hasAttribute(SHIELD_RELOADTIME))
                m_msecsToReload = entityInfo->getIntAttribute(SHIELD_RELOADTIME);

            if (m_compShield->m_player) {
                m_particleMngr->initShield(scene->getSceneManager()->getSceneNode("Player_node"));
                m_particleMngr->startShield();
            }
            else {
                // billboard or something
            }

            if (entityInfo->hasAttribute(GRAPHIC_MODEL)) {
         //       Ogre::Entity* ent = scene->getSceneManager()->createEntity("shield_model", entityInfo->getStringAttribute(Common::Data::Spawn::GRAPHIC_MODEL));
         //       ent->setMaterialName("Shield");
         //       m_nameChildNode =  "shield_" + std::to_string(m_numTriggers);
          //      m_sceneNode = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode(m_nameChildNode);
              //  m_sceneNode->attachObject(ent);
              //  m_sceneNode->setScale(Vector3(80));
            }

            return true;
        }

        void CShieldTrigger::tick(unsigned int msecs)
        {
            using namespace Common::Util::PxConversor;
            reinterpret_cast<physx::PxRigidDynamic*>(m_actor)->setKinematicTarget(Matrix4ToPxTransform(m_trans->getTransform()));
     //       m_sceneNode->setPosition(m_trans->getPosition());

            // time with no shield
            if (!m_activateShield) {
                m_timeOut += msecs;
                if (m_secsNoShield < (m_timeOut*0.001)) { 
                    m_timeOut = 0;
                    m_activateShield = true;
                    m_firstTime = true;
                }
            }
            else if ( !m_compShield->isFull() ) { // refilling shield
                m_timeOutReload += msecs;
                if (m_timeOutReload >= m_msecsToReload) {
                    m_timeOutReload = 0;
                    m_compShield->regenShield();
                    // change quota every 1 of each 10 times
                    if ( ++m_timesChangeQuota >= 10 || m_firstTime) {
                        m_particleMngr->changeQuota(m_compShield->getValue(), m_compShield->getMaxShield());
                        m_timesChangeQuota = 0;
                        m_firstTime = false;
                    }
                }
            }
        }

        void CShieldTrigger::onOverlapBegin(IPhysic* hitEnt)
        {
            if (!m_activateShield) return;

            CEntity* ent = hitEnt->getEntity();
			if ( (ent->getType() == "Asteroid") && (m_compShield->getValue() > 0) ) {
                m_compShield->destroyShield();
                *static_cast<CLife*>(ent->getComponentByName(LIFE_COMP))->m_life = 0;
                Vector3 pos = static_cast<CTransform*>(ent->getComponentByName(TRANSFORM_COMP))->getPosition();
                m_particleMngr->startNextExplosion(pos);
                m_particleMngr->changeQuota(0, m_compShield->getMaxShield());
                m_scene->deactivateEntity(ent);
                m_activateShield = false;
            }
            else if ( (ent->getType() == "Enemy") && (m_compShield->getValue() > 0) ) {
                m_compShield->destroyShield();
                *static_cast<CLife*>(ent->getComponentByName(LIFE_COMP))->m_life = 0;
                Vector3 pos = static_cast<CTransform*>(ent->getComponentByName(TRANSFORM_COMP))->getPosition();
                m_particleMngr->startNextExplosion(pos);
                m_particleMngr->changeQuota(0, m_compShield->getMaxShield());
                m_scene->deactivateEntity(ent);
                m_activateShield = false;
            }
            else if (ent->getType() == "Missile_Linear") {
                m_compShield->decreaseShield(10);
            }
        }

		void CShieldTrigger::onOverlapEnd(IPhysic* otherComponent)
        {
        }

    }
}