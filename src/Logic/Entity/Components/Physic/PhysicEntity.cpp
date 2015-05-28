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

#include "PhysicEntity.h"

#include <log.h>

#include "../../Entity.h"
#include "../Movement/Transform.h"
#include "../Movement/Movement.h"

#include <Common/Map/MapEntity.h>
#include <Common/Physic/PhysicManager.h>
#include <Common/Physic/Conversions.h>
#include <Common/Data/TTG_Types.h>

#include <PxPhysicsAPI.h>

namespace Logic { namespace Component {
	IMP_FACTORY(CPhysicEntity);

	using namespace physx;
	using namespace Common::Physic;
	using namespace Common::Data;

	const char* const LOG_PHYSIC = "Logic::CPhysic";

	CPhysicEntity::CPhysicEntity() : IPhysic(), m_actor(nullptr), m_movement(0,0,0),m_onContact(false)
{
	m_physicMng = CPhysicManager::getInstance();
	m_lastPosition.setTrans(m_movement);
}

CPhysicEntity::~CPhysicEntity() 
{
	if (m_actor) {
		m_physicMng->destroyActor(m_actor);
		m_actor = nullptr;
	}

	m_physicMng = nullptr;
} 

bool CPhysicEntity::spawn(CEntity* entity, CScene *scene, const Map::CMapEntity *entityInfo) 
{
	if(!IComponent::spawn(entity,scene,entityInfo))
		return false;

	m_actor = createActor(entityInfo);

	return true;
}

PxRigidActor* CPhysicEntity::createActor(const Map::CMapEntity *entityInfo)
{
	assert(entityInfo->hasAttribute("physic_entity"));
	const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	assert((physicEntity == "rigid") || (physicEntity == "plane") || (physicEntity == "fromFile"));

	if (physicEntity == "plane") 
		return createPlane(entityInfo);
	
	if (physicEntity == "rigid") 
		return createRigid(entityInfo);
	
	if (physicEntity == "fromFile")
		return createFromFile(entityInfo);

	return nullptr;
}

PxRigidStatic* CPhysicEntity::createPlane(const Map::CMapEntity *entityInfo)
{
	const Vector3 point = static_cast<CTransform*> (m_entity->getComponentByName(TRANSFORM_COMP))->getPosition();
	
	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");

	int group = 0;
	if(entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");
 
	return m_physicMng->createPlane(point,normal,group,this);
}

PxRigidActor* CPhysicEntity::createRigid(const Map::CMapEntity *entityInfo)
{
	const Vector3 position = static_cast<CTransform*> (m_entity->getComponentByName(TRANSFORM_COMP))->getPosition();
	
	assert(entityInfo->hasAttribute("physic_type"));
	const std::string physicType = entityInfo->getStringAttribute("physic_type");
	assert((physicType == "static") || (physicType == "dynamic") || (physicType == "kinematic"));

	const std::string physicShape = entityInfo->getStringAttribute("physic_shape");

	bool trigger = false;
	if(entityInfo->hasAttribute("physic_trigger"))
		trigger = entityInfo->getBoolAttribute("physic_trigger");

	int group = 0;
	if(entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	if (physicType == "static") {
		if (physicShape == "box") {
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			return m_physicMng->createStaticBox(position, physicDimensions, trigger, group, this);
		} else if(physicShape == "sphere"){
			const float radius = entityInfo->getFloatAttribute("physic_radius");
			
			return m_physicMng->createStaticSphere(position, radius, trigger, group, this);
		}

	} else {
		float mass = 1;
		if(entityInfo->hasAttribute("physic_mass"))
			mass = entityInfo->getFloatAttribute("physic_mass");

		bool kinematic = (physicType == "kinematic");

		if (physicShape == "box") {
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			return m_physicMng->createDynamicBox(position,physicDimensions, mass,kinematic,trigger,group,this);
		} else if(physicShape == "sphere"){
			const float radius = entityInfo->getFloatAttribute("physic_radius");
			
			return m_physicMng->createDynamicSphere(position, radius, mass, kinematic, trigger, group, this);
		}
	}

	return nullptr;
}

PxRigidActor* CPhysicEntity::createFromFile(const Map::CMapEntity *entityInfo)
{
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");
	
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	return m_physicMng->createFromFile(file,group,this);
}


void CPhysicEntity::onOverlapBegin(IPhysic* otherComponent)
{

}
void CPhysicEntity::onOverlapEnd(IPhysic* otherComponent)
{

}

void CPhysicEntity::tick(unsigned int msecs) 
{
	PxRigidDynamic *dinActor = m_actor->isRigidDynamic();
	if (!dinActor) 
		return;

	if(!m_physicMng->isKinematic(dinActor)){
		if(m_entity->getType() == "Asteroid"){
			Matrix4 m = m_physicMng->getActorTransform(m_actor);
			static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->setTransform(m);
			return;
		}
		if(m_onContact){
			physx::PxRigidDynamic* actor =  static_cast<PxRigidDynamic*>(m_actor);
			contactMovement();
			//actor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, false);
			actor->addForce(Common::Physic::Vector3ToPxVec3(m_movement)*10,physx::PxForceMode::eFORCE);
			Matrix4 m = m_physicMng->getActorTransform(m_actor);
			static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->setTransform(m);
			return;
		}
		m_physicMng->moveDynamicActor(dinActor,static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->getTransform());
	}
	if(m_physicMng->isKinematic(dinActor)){
		if(m_onContact){
			Matrix4 m = m_physicMng->getActorTransform(m_actor);
			static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->setTransform(m);
			return;
		}
		m_physicMng->moveKinematicActor(dinActor,static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->getTransform());
	}
	
}

void CPhysicEntity::onContact(IPhysic* otherComponent)
{
	//if(!m_onContact) m_onContact = true; return;
	if(m_actor->isRigidDynamic() && static_cast<CPhysicEntity*>(otherComponent)->m_actor->isRigidDynamic()){
		//if(m_physicMng->isKinematic(static_cast<PxRigidDynamic*>(m_actor))
		//	&& m_physicMng->isKinematic(static_cast<PxRigidDynamic*>(static_cast<CPhysicEntity*>(otherComponent)->m_actor))){
		if(!m_physicMng->isKinematic(static_cast<PxRigidDynamic*>(m_actor))){
			std::string otype = otherComponent->getEntity()->getType();
			std::string mtype = m_entity->getType();
			if(m_entity->getType() == "Enemy" && (otherComponent->getEntity()->getType() == "Enemy" || otherComponent->getEntity()->isPlayer())){
			log_trace(LOG_PHYSIC,"Moving on contact\n");
			//if(!m_onContact) m_onContact = true;
			
			physx::PxRigidDynamic* actor =  static_cast<PxRigidDynamic*>(m_actor);
			physx::PxRigidDynamic* actor2 =  static_cast<PxRigidDynamic*>(static_cast<CPhysicEntity*>(otherComponent)->m_actor);

			if(m_physicMng->isTrigger(actor) || m_physicMng->isTrigger(actor2)) return;
			if(!otherComponent->isActivated() || !this->m_activate) return;

			static_cast<CMovement*>(m_entity->getComponentByName(MOVEMENT_COMP))->m_onContact = true;
			m_onContact = true;
			m_contacts.push_back(actor2);
			contactMovement();
			//actor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, false);
			actor->addForce(Common::Physic::Vector3ToPxVec3(m_movement)*10,physx::PxForceMode::eFORCE);
			}
		}
	}
}

void CPhysicEntity::onContactEnd(IPhysic* otherComponent)
{
	std::vector<physx::PxRigidActor*>::iterator it  (m_contacts.begin());
	while(it != m_contacts.end()){
		if((*it) == (static_cast<CPhysicEntity*>(otherComponent)->m_actor)){
			break;
		}
		++it;
	}

	if(it == m_contacts.end()) return;

	m_contacts.erase(it);

	if(m_contacts.empty()){
		m_onContact = false;		
		physx::PxRigidDynamic* actor =  static_cast<PxRigidDynamic*>(m_actor);
		//actor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, true);
		actor->clearForce();
		static_cast<CMovement*>(m_entity->getComponentByName(MOVEMENT_COMP))->m_onContact = false;
	}
}

void CPhysicEntity::contactMovement()
{
	std::vector<physx::PxRigidActor*>::const_iterator it (m_contacts.begin());
	physx::PxRigidDynamic* actor =  static_cast<PxRigidDynamic*>(m_actor);
	m_movement = Vector3(0,0,0);
	for(;it != m_contacts.end(); ++it){
		
		physx::PxRigidDynamic* actor2 =  static_cast<PxRigidDynamic*>(*it);

		Matrix4 m = m_physicMng->getActorTransform(actor);
		Matrix4 m2 = m_physicMng->getActorTransform(actor2);

		const float radius = m_physicMng->getActorRadius(actor);
		const float radius2 = m_physicMng->getActorRadius(actor2);

		float distance = radius + radius2;
		float curr_distanace = m.getTrans().distance(m2.getTrans());
		if(distance > curr_distanace){
			Vector3 dir(m2.getTrans() - m.getTrans());
			dir.normalise();
			m_movement += dir;
		}
	}
	m_movement.normalise();
}

bool CPhysicEntity::activate()
{
	if(!IComponent::activate()) return false;

	return m_physicMng->activateActor(m_actor);

	return true;
}
void CPhysicEntity::deactivate()
{
	m_physicMng->deactivateActor(m_actor);

	IComponent::deactivate();
}



}}