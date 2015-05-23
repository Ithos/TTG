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

using namespace Common::Util::PxConversor;

namespace Logic
{
	namespace Component
	{

        IMP_FACTORY(CMissileTrigger)

        bool CMissileTrigger::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            if (!CPhysicEntity::spawn(entity, scene, entityInfo))
                return false;

            m_parent      = entity;
            m_parentTrans = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));
            //reinterpret_cast<physx::PxRigidDynamic*>(m_actor)->setKinematicTarget(Matrix4ToPxTransform(m_parentTrans->getTransform()));
			
			//Esto en el activate
			//m_physicMng->moveKinematicActor(static_cast<physx::PxRigidDynamic*>(m_actor),m_parentTrans->getTransform());

            
            using namespace Common::Data::Spawn;

            if (entityInfo->hasAttribute(MISSILE_SPEED))
                m_speed = entityInfo->getFloatAttribute(MISSILE_SPEED);

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
                m_pos += (m_dir * m_speed * msecs);
                m_trans.setTrans(m_pos);
            }
            else {
                moveFunc(m_pos, m_dir, m_speed);
            }

            m_trans.setTrans(m_pos);
            reinterpret_cast<physx::PxRigidDynamic*>(m_actor)->setKinematicTarget(Matrix4ToPxTransform(m_trans));
        }

        void CMissileTrigger::onOverlapBegin(IPhysic* hitPhysicComp)
        {
            m_shooted = false;
            /* quitar vida emplotar y todo eso*/
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
			if(!CPhysicEntity::activate()) return false;

			m_physicMng->moveKinematicActor(static_cast<physx::PxRigidDynamic*>(m_actor),m_parentTrans->getTransform());

			return true;
		}
    }
}