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

#ifndef __COMMON_PHYSIC_PHYSICMANAGER_H
#define __COMMON_PHYSIC_PHYSICMANAGER_H

#include "../Util/Math.h"

namespace Logic
{
	class CEntity;
	namespace Component
	{
		class CPhysicEntity;
		class IPhysic;
	}
}

namespace Common
{
	namespace Physic
	{
		class CCollisionManager;
		class CErrorManager;
		class CContactManager;
	}
}

namespace physx
{
	class PxActor;
	class PxAllocatorCallback;
	class PxCapsuleController;
	class PxController;
	class PxControllerManager;
	class PxCooking;
	class PxDefaultAllocator;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	class PxMaterial;
	class PxPhysics;
	class PxProfileZoneManager;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxRigidStatic;
	class PxScene;
	class PxCudaContextManager;

	namespace debugger
	{
		namespace comm
		{
			class PvdConnection;
		}
	}
}

struct FilterGroup
{
        enum Enum
        {
                eSPACE_FILTER = (1 << 0)
        };
};

namespace Common
{
	namespace Physic
	{
		class CPhysicManager
		{
		public:
			static CPhysicManager* getInstance() { return m_instance; }

			static bool Init();

			static void Release();

			bool tick(unsigned int msecs);

			void createScene();
			void destroyScene();

			void setGroupCollisions(int g1, int g2, bool enable);

			physx::PxRigidStatic* createPlane(const Vector3& point, const Vector3& normal, int group, const Logic::Component::IPhysic* component);

			physx::PxRigidStatic* createStaticBox(const Vector3 &position, const Vector3 &dimensions, bool trigger, int group, const Logic::Component::IPhysic *component);

			physx::PxRigidDynamic* createDynamicBox(const Vector3 &position, const Vector3 &dimensions, float mass, bool kinematic, bool trigger, int group, const Logic::Component::IPhysic *component);

			physx::PxRigidStatic* createStaticSphere(const Vector3 &position, const float &radius, bool trigger, int group, const Logic::Component::IPhysic *component);

			physx::PxRigidDynamic* createDynamicSphere(const Vector3 &position, const float &radius, float mass, bool kinematic, bool trigger, int group, const Logic::Component::IPhysic *component);

			physx::PxRigidActor* createFromFile(const std::string &file, int group, const Logic::Component::IPhysic *component);

			void destroyActor(physx::PxActor *actor);

			Matrix4 getActorTransform(const physx::PxRigidActor *actor);

			void moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform);

			void moveDynamicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform);

			void moveKinematicActor(physx::PxRigidDynamic *actor, const Vector3 &displ);
			//void moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4& tf);

			bool isKinematic(const physx::PxRigidDynamic *actor);

			Logic::CEntity* raycastClosest (const Ray& ray, float maxDist) const;

			Logic::CEntity* raycastClosest (const Ray& ray, float maxDist, int group) const;

			const float getActorRadius(physx::PxRigidDynamic* actor);

			bool isTrigger(physx::PxRigidActor* actor);

			bool activateActor(physx::PxRigidActor *actor, bool trigger = false);
			void deactivateActor(physx::PxRigidActor *actor);

		protected:
		private:

			CPhysicManager();

			~CPhysicManager();

			static CPhysicManager* m_instance;

			physx::PxErrorCallback* m_errorManager;

			physx::PxAllocatorCallback* m_allocator;

			physx::PxFoundation* m_foundation;
			physx::PxProfileZoneManager* m_profileZoneManager;
			physx::PxDefaultCpuDispatcher* m_cpuDispatcher;
			physx::PxCudaContextManager* m_cudaContextManager;
			physx::debugger::comm::PvdConnection* m_pvdConnection;
			physx::PxCooking* m_cooking;

			physx::PxPhysics* m_physics;

			physx::PxScene* m_scene;

			physx::PxMaterial* m_defaultMaterial;

			CCollisionManager* m_collisionManager;

			CContactManager* m_contactManager;

			void setupFiltering(physx::PxRigidActor* actor, unsigned int filterGroup, unsigned int filterMask);
		};
	}
}

#endif