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
#include "common/Particles/ParticleManager.h"

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
            CEntity* thisEnt = m_entity;
            if (!CPhysicEntity::spawn(entity, scene, entityInfo))
                return false;

            m_entity      = thisEnt;
            m_parent      = entity;			
            m_parentTrans = static_cast<CTransform*>(m_parent->getComponentByName(Common::Data::TRANSFORM_COMP));
            
            if (entityInfo->hasAttribute(MISSILE_SPEED))
                m_speed = entityInfo->getFloatAttribute(MISSILE_SPEED);

            if (entityInfo->hasAttribute(MISSILE_DAMAGE))
                m_damage = entityInfo->getFloatAttribute(MISSILE_DAMAGE);

            if (entityInfo->hasAttribute(MISSILE_RANGE))
                m_range =  entityInfo->getFloatAttribute(MISSILE_RANGE);

            m_trans = Matrix4::IDENTITY;
            m_particles = Common::Particles::CParticleManager::getInstance();

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
                delete m_bb;

            m_bb = m_set->createBillboard(pos);
            
      /*      m_rt = static_cast<Ogre::RibbonTrail*>(scene->getSceneManager()->createMovableObject("testribbon", "RibbonTrail"));
            m_rt->setMaterialName("LightRibbonTrail");
            m_rt->setTrailLength(80);
            m_rt->setMaxChainElements(500);
            m_rt->setInitialColour(0, 0.58, 0.7, 0.88, 0.74);
            m_rt->setColourChange(0, 1, 1, 1, 0.8);
            m_rt->setInitialWidth(0, 6);
            m_rt->setWidthChange(0, 3);
            m_rt->addNode(node);
            m_node->attachObject(m_rt);*/

            return true;
        }

        CMissileTrigger::~CMissileTrigger()
        {
            m_node->detachAllObjects();
            m_sceneMgr->destroySceneNode(m_node);
            m_sceneMgr->destroyBillboardSet(m_set);
            m_sceneMgr = nullptr;
            m_parent   = nullptr;
        }

        void CMissileTrigger::tick(unsigned int msecs)
        {
            if (!m_shooted)
                return;

            // if (m_pos - initalPos > m_range) destroy();

           if (!moveFunc) {
                m_pos = m_pos + (m_dir * m_speed * msecs);
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
            if (  type == "Enemy" || type == "Asteroid") {
                m_shooted = false;
                m_entity->deactivate();
                int* life = static_cast<CLife*>(hitComp->getEntity()->getComponentByName(LIFE_COMP))->m_life;
                Vector3 pos = static_cast<CTransform*>(hitEnt->getComponentByName(TRANSFORM_COMP))->getPosition();
                if ( *life > 0) {
                    *life = (*life <= m_damage)? 0 : *life - m_damage;
                        
                    if (*life  <= 0) {
                        hitEnt->deactivate();
                        m_particles->startNextExplosion(pos);
                        delete m_bb;
                        m_bb =  nullptr;
                    }
                    else {
                        //m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));
                    }
                }
            }
            else if (type == "PlanetLimitTrigger") {
                m_shooted = false;
                m_entity->deactivate();
                delete m_bb;
                m_bb =  nullptr;
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
    }
}