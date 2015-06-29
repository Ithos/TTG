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
#include "../Gameplay/Shield.h"
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

		Map::CMapEntity* entinf = new Map::CMapEntity("..");
		entinf->setAttribute(PHYSIC_ENTITY,  entityInfo->getStringAttribute(PHYSIC_ENTITY));
        entinf->setAttribute("physic_type", "dynamic");
        entinf->setAttribute("physic_shape", "sphere");
        entinf->setAttribute("physic_mass",  "1");
		entinf->setAttribute(PHYSIC_RADIUS,  entityInfo->getStringAttribute(BOMB_RANGE));
		entinf->setAttribute(PHYSIC_TRIGGER, entityInfo->getStringAttribute(PHYSIC_TRIGGER));

		if(!CPhysicEntity::spawn(entity,scene,entinf)) return false;

		delete entinf;

		if(entityInfo->hasAttribute(BOMB_DAMAGE))
			m_damage = entityInfo->getIntAttribute(BOMB_DAMAGE);
		
		m_ff = false;
		if(entityInfo->hasAttribute(BOMB_FF))
			m_ff = entityInfo->getBoolAttribute(BOMB_FF);

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
		if(!m_shooted) return;

		if(!m_explode) return;
		for(auto it = m_entitiesOnRange.begin(); it != m_entitiesOnRange.end(); ++it){
			CShield* sh = static_cast<CShield*>((*it)->getComponentByName(SHIELD_COMP));
			if(sh && sh->getValue() > 0){
				sh->decreaseShield(m_damage);
				//TODO also decrease life?
				continue;
			}

			if (static_cast<CLife*>((*it)->getComponentByName(LIFE_COMP))->decreaseLife(m_damage)){
				Vector3 pos = static_cast<CTransform*>((*it)->getComponentByName(TRANSFORM_COMP))->getPosition();  
				m_scene->deactivateEntity(*it);
				m_scene->deleteEntity(*it);
                m_particles->startNextExplosion(pos);
			}
		}
		m_entitiesOnRange.clear();
		m_explode = false;
		m_shooted = false;
	}

    void CBombTrigger::onOverlapBegin(IPhysic* otherComponent)
	{
		std::string type = otherComponent->getEntity()->getType();

		bool filter = (type == "Asteroid" || type == "Enemy" || type == "Player");

		if(!m_ff){
			filter = filter && (type != m_parent->getType());// Type
			//filter = filter && (otherComponent->getEntity() != m_parent);
		}

		if(filter){
			auto it = m_entitiesOnRange.begin();
			bool found = false;
			while(it != m_entitiesOnRange.end() && !(found = (*it == otherComponent->getEntity()))){
				++it;
			}
			if(found) return;
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

	void CBombTrigger::setPosition(const Vector3& pos, CEntity* parent)
	{
		physx::PxRigidDynamic* actor = m_actor->isRigidDynamic();
		if(!actor) return;
		Matrix4 tf; tf.setTrans(pos);
		m_physicMng->moveDynamicActor(actor,tf);
		m_shooted = true;
		m_parent = parent;
	}
}}