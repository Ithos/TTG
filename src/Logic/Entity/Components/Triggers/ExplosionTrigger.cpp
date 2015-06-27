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

#include "ExplosionTrigger.h"
#include "Logic/Entity/Entity.h"
#include "Common/Map/MapEntity.h"
#include "Common/Data/TTG_Types.h"
#include "Common/Util/PxConversor.h"
#include "Common/data/Spawn_Constants.h"
#include "../Gameplay/Life.h"
#include "../Gameplay/Shield.h"
#include <Common/Physic/PhysicManager.h>
#include "common/Particles/ParticleManager.h"
#include "../Cameras/Camera.h"

#include <PxPhysicsAPI.h>

#include "BombTrigger.h"

using namespace Common::Util::PxConversor;
using namespace Common::Data::Spawn;
using namespace Common::Data;

namespace Logic
{
	namespace Component
	{
        IMP_FACTORY(CExplosionTrigger)

        bool CExplosionTrigger::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            // create trigger
            if (!m_parent) {
                CEntity* thisEnt = m_entity;
                if (!CPhysicEntity::spawn(entity, scene, entityInfo))
                    return false;

                m_entity      = thisEnt;
                m_parent      = entity;			
         //       m_parentTrans = static_cast<CTransform*>(m_parent->getComponentByName(Common::Data::TRANSFORM_COMP));
       //         m_scene       = scene;
                m_isPlayer    = m_parent->isPlayer();
            
                if (entityInfo->hasAttribute(MISSILE_DAMAGE))
                    m_damage = entityInfo->getFloatAttribute(MISSILE_DAMAGE);

//                m_trans = Matrix4::IDENTITY;
                m_particles = Common::Particles::CParticleManager::getInstance();
            }
            else 
                m_stillActive = true;
        
        }

        bool CExplosionTrigger::activate()
        {
            if (!IComponent::activate()) return false;
            if (!m_stillActive)          m_physicMng->activateActor(m_actor,true);
            return true;
        }

		void CExplosionTrigger::tick(unsigned int msecs)
		{
			if(!m_shooted) return;

			m_acumT += msecs;
			if(m_time > m_acumT) return;

			static_cast<CBombTrigger*>(this->m_entity->getComponentByName("CBombTrigger"))->m_explode  = true;
		}
        
        void CExplosionTrigger::onOverlapBegin(IPhysic* otherComponent)
        {
			if(!m_shooted) return;
			static_cast<CBombTrigger*>(this->m_entity->getComponentByName("CBombTrigger"))->m_explode  = true;
        }

        void CExplosionTrigger::onOverlapEnd(IPhysic* otherComponent)
        {
        }

        void CExplosionTrigger::shoot(const Vector3& src)
        {
            m_shooted = true;
            m_pos     = src;
        }

		void CExplosionTrigger::setPosition(const Vector3& pos)
		{
			physx::PxRigidDynamic* actor = m_actor->isRigidDynamic();
			if(!actor) return;
			Matrix4 tf; tf.setTrans(pos);
			m_physicMng->moveDynamicActor(actor,tf);
		}
    }
}