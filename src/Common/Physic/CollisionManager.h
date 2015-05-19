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

#ifndef __COMMON_PHYSIC_COLLISIONMANAGER_H
#define __COMMON_PHYSIC_COLLISIONMANAGER_H

#include <PxSimulationEventCallback.h> 
#include <characterkinematic/PxController.h>

namespace Common
{
	namespace Physic
	{
		class CCollisionManager : public physx::PxSimulationEventCallback,
							  public physx::PxUserControllerHitReport
						  
		{
		public:
			CCollisionManager();

			~CCollisionManager();

			void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count); 
 
			void onWake(physx::PxActor **actors, physx::PxU32 count);
  
			void onSleep(physx::PxActor **actors, physx::PxU32 count); 
   
			void onContact(const physx::PxContactPairHeader &pairHeader, 
						   const physx::PxContactPair *pairs, physx::PxU32 nbPairs); 
  
			void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

			void onShapeHit(const physx::PxControllerShapeHit &hit);
  
			void onControllerHit(const physx::PxControllersHit &hit); 
   
			void onObstacleHit(const physx::PxControllerObstacleHit &hit);
   
		};
	}
}

#endif