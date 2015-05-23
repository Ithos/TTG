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

#include "CollisionManager.h"

#include <log.h>

#include <Logic/Entity/Components/Physic/PhysicEntity.h>
#include "Conversions.h"

#include <PxRigidActor.h>
#include <PxShape.h> 
#include <PxSimulationEventCallback.h> 

#include <PxPhysicsAPI.h>

using namespace Logic::Component;
using namespace physx;

namespace Common { namespace Physic {

const char* const LOG_PHYSIC = "Common::Physic";

CCollisionManager::CCollisionManager()
{

}

CCollisionManager::~CCollisionManager()
{

}

void CCollisionManager::onConstraintBreak(PxConstraintInfo *constraints, PxU32 count)
{
}

void CCollisionManager::onWake(PxActor **actors, PxU32 count)
{
}

void CCollisionManager::onSleep(PxActor **actors, PxU32 count)
{	
}

void CCollisionManager::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) 
{
	for(PxU32 i = 0; i < nbPairs; ++i){
		const PxContactPair& cp = pairs[i];

		if(cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND){
			if(PxRigidDynamic* actor = pairHeader.actors[0]->isRigidDynamic()){
					//PxActor* actor = pairHeader.actors[0];

				if(actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC){
					Logic::Component::IPhysic* comp1 = static_cast<Logic::Component::IPhysic*>(actor->userData);
					Logic::Component::IPhysic* comp2 = static_cast<Logic::Component::IPhysic*>(pairHeader.actors[1]->userData);
					comp1->onContact(comp2);
				} else {
					Logic::Component::IPhysic* comp1 = static_cast<Logic::Component::IPhysic*>(actor->userData);
					Logic::Component::IPhysic* comp2 = static_cast<Logic::Component::IPhysic*>(pairHeader.actors[1]->userData);
					comp1->onContact(comp2);
				}

			}
			if(PxRigidDynamic* actor = pairHeader.actors[1]->isRigidDynamic()){
				//PxActor* actor = pairHeader.actors[1];
				if(actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC){
					Logic::Component::IPhysic* comp1 = static_cast<Logic::Component::IPhysic*>(actor->userData);
					Logic::Component::IPhysic* comp2 = static_cast<Logic::Component::IPhysic*>(pairHeader.actors[0]->userData);
					comp1->onContact(comp2);
				} else {
					Logic::Component::IPhysic* comp1 = static_cast<Logic::Component::IPhysic*>(actor->userData);
					Logic::Component::IPhysic* comp2 = static_cast<Logic::Component::IPhysic*>(pairHeader.actors[0]->userData);
					comp1->onContact(comp2);
				}
			}
		}
	}
}

void CCollisionManager::onTrigger(PxTriggerPair *pairs, PxU32 count)
{
	for (unsigned int i=0; i<count; i++) {
		if (pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
			continue;

		bool enter = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND;
		bool exit = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST;

		if (!enter && !exit)
			continue;

		IPhysic* comp1 = static_cast<IPhysic*>( pairs[i].triggerActor->userData);
		IPhysic* comp2 = (IPhysic*) pairs[i].otherActor->userData;

		if(enter){
			comp1->onOverlapBegin(comp2);
			comp2->onOverlapBegin(comp1);
		} else {
			comp1->onOverlapEnd(comp2);
			comp2->onOverlapEnd(comp1);
		}
	}	
}

void CCollisionManager::onShapeHit(const PxControllerShapeHit &hit)
{
}

void CCollisionManager::onControllerHit(const PxControllersHit &hit) 
{
}

void CCollisionManager::onObstacleHit(const PxControllerObstacleHit &hit)
{
}

void CContactManager::onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count)
{
	return;
	for(PxU32 i = 0; i < count; i++){
		const PxContactModifyPair& cp = pairs[i];

		if(const PxRigidDynamic* actor = cp.actor[0]->isRigidDynamic()){
				//PxActor* actor = pairHeader.actors[0];

			if(actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC){
				Logic::Component::IPhysic* comp1 = static_cast<Logic::Component::IPhysic*>(actor->userData);
				Logic::Component::IPhysic* comp2 = static_cast<Logic::Component::IPhysic*>(cp.actor[1]->userData);
				comp1->onContact(comp2);
			} else {
				Logic::Component::IPhysic* comp1 = static_cast<Logic::Component::IPhysic*>(actor->userData);
				Logic::Component::IPhysic* comp2 = static_cast<Logic::Component::IPhysic*>(cp.actor[1]->userData);
				comp1->onContact(comp2);
			}

		}
		if(const PxRigidDynamic* actor = cp.actor[1]->isRigidDynamic()){
			//PxActor* actor = pairHeader.actors[1];
			if(actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC){
				Logic::Component::IPhysic* comp1 = static_cast<Logic::Component::IPhysic*>(actor->userData);
				Logic::Component::IPhysic* comp2 = static_cast<Logic::Component::IPhysic*>(cp.actor[0]->userData);
				comp1->onContact(comp2);
			} else {
				Logic::Component::IPhysic* comp1 = static_cast<Logic::Component::IPhysic*>(actor->userData);
				Logic::Component::IPhysic* comp2 = static_cast<Logic::Component::IPhysic*>(cp.actor[0]->userData);
				comp1->onContact(comp2);
			}
		}
	}
}

}}