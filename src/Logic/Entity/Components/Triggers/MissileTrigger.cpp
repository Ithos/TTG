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

#include "MissileTrigger.h"
#include "Logic/Entity/Entity.h"
#include "Common/Map/MapEntity.h"
#include "Common/Data/TTG_Types.h"
#include "Common/Util/PxConversor.h"
#include "Common/data/Spawn_Constants.h"
#include "../../Components/Movement/Transform.h"
#include <PxPhysicsAPI.h>
#include <Common/Physic/PhysicManager.h>
#include "../Gameplay/Life.h"
#include "../Gameplay/Shield.h"
#include "common/Particles/ParticleManager.h"
#include "../Cameras/Camera.h"

#include <Logic/Scene/Scene.h>
#include "OGRE\OgreRibbonTrail.h"
#include <OGRE\OgreBillboardSet.h>
#include <OGRE\OgreBillboard.h>

using namespace Common::Util::PxConversor;
using namespace Common::Data::Spawn;
using namespace Common::Data;

namespace Logic
{
	namespace Component
	{
        IMP_FACTORY(CMissileTrigger)

        Ogre::BillboardSet* CMissileTrigger::m_set      = nullptr;
        Ogre::SceneManager* CMissileTrigger::m_sceneMgr = nullptr;
        Ogre::SceneNode*    CMissileTrigger::m_node     = nullptr;

        bool CMissileTrigger::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            // create trigger
            if (!m_parent) {
                CEntity* thisEnt = m_entity;
                if (!CPhysicEntity::spawn(entity, scene, entityInfo))
                    return false;

                m_entity      = thisEnt;
                m_parent      = entity;			
                m_parentTrans = static_cast<CTransform*>(m_parent->getComponentByName(Common::Data::TRANSFORM_COMP));
                m_scene       = scene;
                m_isPlayer    = m_parent->isPlayer();
            
                if (entityInfo->hasAttribute(MISSILE_SPEED))
                    m_speed = entityInfo->getFloatAttribute(MISSILE_SPEED);

                if (entityInfo->hasAttribute(MISSILE_DAMAGE))
                    m_damage = entityInfo->getFloatAttribute(MISSILE_DAMAGE);

                if (entityInfo->hasAttribute(MISSILE_RANGE))
                    m_range =  entityInfo->getFloatAttribute(MISSILE_RANGE);

                m_trans = Matrix4::IDENTITY;
                m_particles = Common::Particles::CParticleManager::getInstance();
            }
            else 
                m_stillActive = true;

            if (!m_sceneMgr)
                m_sceneMgr = scene->getSceneManager();

            if (!m_set) {
                m_set = m_sceneMgr->createBillboardSet();
                m_set->setMaterialName("Missile");
                m_set->setDefaultDimensions(20, 20);
                m_node = m_sceneMgr->getRootSceneNode();
                m_node->attachObject(m_set);
            }
            Vector3 pos = m_parentTrans->getPosition();
            if (m_bb)
                m_set->removeBillboard(m_bb);

            m_bb = m_set->createBillboard(pos);
            return true;
        }

        CMissileTrigger::~CMissileTrigger()
        {
            if (m_node) {
                if (m_set) {
                    m_sceneMgr->destroyBillboardSet(m_set);
                    m_set = nullptr;
                }
                m_sceneMgr = nullptr;
                m_parent   = nullptr;
            }
        }

        void CMissileTrigger::tick(unsigned int msecs)
        {
            if (!m_shooted)
                return;

           if (!moveFunc) {
                m_pos = m_pos + (m_dir * m_speed * msecs);
                // for moving a billboard move this node for make a trail.
                m_bb->setPosition(m_pos);
            }
            else {
                moveFunc(m_pos, m_dir, m_speed);
            }

            m_trans.setTrans(m_pos);
			m_physicMng->moveKinematicActor(m_actor->isRigidDynamic(), m_trans);
        }

        void CMissileTrigger::onOverlapBegin(IPhysic* hitComp)
        {
            if (!m_shooted)
                return;

            CEntity* hitEnt = hitComp->getEntity();
            std::string type = hitComp->getEntity()->getType();

            if (type == "Asteroid") {
                if (!hitEnt->isActivated()) return;

				if (static_cast<CLife*>(hitComp->getEntity()->getComponentByName(LIFE_COMP))->decreaseLife(m_damage)) {
                        Vector3 pos = static_cast<CTransform*>(hitEnt->getComponentByName(TRANSFORM_COMP))->getPosition();  
						m_scene->deactivateEntity(hitEnt);
						m_scene->deleteEntity(hitEnt);
                        m_particles->startNextExplosion(pos);
                    }
                    else {
                        //m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));
                    }

                if (m_bb) { 
                    m_shooted = false;
                    m_set->removeBillboard(m_bb); 
                    m_bb = nullptr; 
                }
            }
            // hit with enemy shooting the player
            else if (type == "Enemy" && m_isPlayer) {
                if (!hitEnt->isActivated()) return;
               
                // else if enemy impact with other enemy is friendly fire and we havent
                // if is the player-> destroy tha mothafucking ship
				if(static_cast<CLife*>(hitComp->getEntity()->getComponentByName(LIFE_COMP))->decreaseLife(m_damage)){
					Vector3 pos = static_cast<CTransform*>(hitEnt->getComponentByName(TRANSFORM_COMP))->getPosition();
					m_scene->deactivateEntity(hitEnt);
					m_scene->deleteEntity(hitEnt);
					m_particles->startNextExplosion(pos);
				}
                else
                    //m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));

                if (m_bb) { 
                    m_shooted = false;
                    m_set->removeBillboard(m_bb); 
                    m_bb = nullptr; 
                }
            }
            // only missiles fired by enemies
            else if (type == "Player" && !m_isPlayer) {
                if (!m_playerShield)
                    m_playerShield = static_cast<CShield*>(hitEnt->getComponentByName(SHIELD_COMP));

                if (!m_playerLife)
                    m_playerLife = static_cast<CLife*>(hitEnt->getComponentByName(LIFE_COMP)); 

                if (m_playerShield->hasShield()) {
                    m_playerShield->decreaseShield(m_damage);
 
                }  
                // if hit with player-> that means that player has no shield
                else if (m_playerLife->decreaseLife(m_damage)) {
              //      m_scene->deactivateEntity(hitEnt);
				//	m_scene->deleteEntity(hitEnt);
                    Vector3 pos = static_cast<CTransform*>(hitEnt->getComponentByName(TRANSFORM_COMP))->getPosition();
                    m_particles->startNextExplosion(pos);
                }
                else {
                    //m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));
                }

                if (m_bb) { 
                    m_shooted = false;
                    m_set->removeBillboard(m_bb); 
                    m_bb = nullptr; 
                }
            }
            else if (type == "PlanetLimitTrigger") {
                m_scene->deactivateEntity(hitEnt);

                if (m_bb) { 
                    m_shooted = false;
                    m_set->removeBillboard(m_bb); 
                    m_bb = nullptr; 
                }
            }
        }

		void CMissileTrigger::onOverlapEnd(IPhysic* hitPhysicComp)
        {
        }

        void CMissileTrigger::shoot(const Vector3& src, const Vector3& dir)
        {
            m_shooted = true;
            m_pos     = src;
            m_dir     = dir;
        }

		bool CMissileTrigger::activate()
		{
			if (!IComponent::activate()) return false;
            if (!m_stillActive)          m_physicMng->activateActor(m_actor,true);
            return true;
		}
    }
}