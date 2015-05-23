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
#include "../Gameplay/Life.h"

using namespace Common::Util::PxConversor;
using namespace Common::Data::Spawn;
using namespace Common::Data;

namespace Logic
{
	namespace Component
	{

        IMP_FACTORY(CMissileTrigger)

        int CMissileTrigger::numEntity = 0;

        bool CMissileTrigger::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            CEntity* thisEnt = m_entity;
            if (!CPhysicEntity::spawn(entity, scene, entityInfo))
                return false;

            m_entity      = thisEnt;
            m_parent      = entity;
            m_parentTrans = static_cast<CTransform*>(m_parent->getComponentByName(TRANSFORM_COMP));
            reinterpret_cast<physx::PxRigidDynamic*>(m_actor)->setKinematicTarget(Matrix4ToPxTransform(m_parentTrans->getTransform()));
            
            if (entityInfo->hasAttribute(MISSILE_SPEED))
                m_speed = entityInfo->getFloatAttribute(MISSILE_SPEED);

            m_trans = Matrix4::IDENTITY;
            m_instance = numEntity;

            return true;
        }

        CMissileTrigger::~CMissileTrigger()
        {
            m_parent = nullptr;
        }

        void CMissileTrigger::tick(unsigned int msecs)
        {
            if (!m_shooted)
                return;

           if (!moveFunc) {
                m_pos = m_pos + (m_dir * m_speed * msecs);
            }
            else {
                moveFunc(m_pos, m_dir, m_speed);
            }

            m_trans.setTrans(m_pos);
            reinterpret_cast<physx::PxRigidDynamic*>(m_actor)->setKinematicTarget(Matrix4ToPxTransform(m_trans));
        }

        void CMissileTrigger::onOverlapBegin(IPhysic* hitComp)
        {
            CEntity* hitEnt = hitComp->getEntity();
            std::string type = hitComp->getEntity()->getType();
            if (  type == "Enemy" || type == "Asteroid") {
                m_shooted = false;
                m_entity->deactivate();
                unsigned int* life = static_cast<CLife*>(hitComp->getEntity()->getComponentByName(LIFE_COMP))->m_life;
                if ( *life > 0) {
                    *life = (*life <= m_damage)? 0 : *life - m_damage;
                        
                    if (*life  == 0) {
                        hitEnt->deactivate();
                        //m_particles->startNextExplosion(m_currPos);
                    }
                    else {
                        //m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));
                    }
                }
                // make boom boom with particles
            }
            else if (type == "PlanetLimitTrigger") {
            
            }
        }

		void CMissileTrigger::onOverlapEnd(IPhysic* hitPhysicComp)
        {
        }

        void CMissileTrigger::shoot(const Vector3& src, const Vector3& dir, float damage)
        {
            m_shooted = true;
            m_pos     = src;
            m_dir     = dir;
            m_damage  = damage;
        }
    }
}