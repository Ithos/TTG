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

#include "BombTrigger.h"

#include <Common/Data/TTG_Types.h>
#include <Common/data/Spawn_Constants.h>
#include <Common/Map/MapEntity.h>
#include <common/Physic/PhysicManager.h>
#include <Logic/Scene/Scene.h>
#include <Common/Particles/ParticleManager.h>

#include <PxPhysicsAPI.h>

#include "../../Entity.h"
#include "../Gameplay/Life.h"
#include "../../Components/Movement/Transform.h"

using namespace Common::Data::Spawn;
using namespace Common::Data;

namespace Logic { namespace Component 
{
	IMP_FACTORY(CBombTrigger);

	CBombTrigger::~CBombTrigger()
	{

	}

	bool CBombTrigger::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!CPhysicEntity::spawn(entity,scene,entityInfo)) return false;

		if(entityInfo->hasAttribute(BOMB_DAMAGE))
			m_damage = entityInfo->getIntAttribute(BOMB_DAMAGE);

		m_scene = scene;
		m_entitiesOnRange.clear();
		m_particles = Common::Particles::CParticleManager::getInstance();

		return true;
	}

	bool CBombTrigger::activate()
	{
		if(!CPhysicEntity::activate()) return false;
		return true;
	}

	void CBombTrigger::tick(unsigned int)
	{
		if(!m_explode) return;
		for(auto it = m_entitiesOnRange.begin(); it != m_entitiesOnRange.end(); ++it){
			if (static_cast<CLife*>((*it)->getComponentByName(LIFE_COMP))->decreaseLife(m_damage)){
				Vector3 pos = static_cast<CTransform*>((*it)->getComponentByName(TRANSFORM_COMP))->getPosition();  
				m_scene->deactivateEntity(*it);
				m_scene->deleteEntity(*it);
                m_particles->startNextExplosion(pos);
			}
		}
		m_explode = false;
	}

    void CBombTrigger::onOverlapBegin(IPhysic* otherComponent)
	{
		std::string type = otherComponent->getEntity()->getType();

		if(type == "Asteroid" || type == "Enemy" || type == "Player"){
			m_entitiesOnRange.push_back(otherComponent->getEntity());
		}
	}

	void CBombTrigger::onOverlapEnd(IPhysic* otherComponent)
	{
		auto it = m_entitiesOnRange.begin();
		bool found = false;
		while(it != m_entitiesOnRange.end() && !(found = (*it == otherComponent->getEntity()))){
			++it;
		}
		if(found){
			m_entitiesOnRange.erase(it);
		}
	}

	void CBombTrigger::setPosition(const Vector3& pos)
	{
		physx::PxRigidDynamic* actor = m_actor->isRigidDynamic();
		if(!actor) return;
		Matrix4 tf; tf.setTrans(pos);
		m_physicMng->moveDynamicActor(actor,tf);
	}
}}