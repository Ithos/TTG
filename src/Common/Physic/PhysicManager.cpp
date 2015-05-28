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

#include "PhysicManager.h"

#include <log.h>

#include "ErrorManager.h"
#include "CollisionManager.h"
#include "Conversions.h"

#include <Logic/Entity/Components/Physic/IPhysic.h>

#include <PxPhysicsAPI.h>
#include <extensions/PxExtensionsAPI.h>
#include <extensions/PxVisualDebuggerExt.h>

namespace Common
{
namespace Physic
{
	using namespace physx;
	using namespace Logic::Component;

	const char* const LOG_PHYSIC = "Common::Physic";

	CPhysicManager* CPhysicManager::m_instance = nullptr;

	CPhysicManager::CPhysicManager() : m_cudaContextManager(nullptr), m_scene(nullptr)
	{
		m_errorManager = new CErrorManager();

		m_allocator = new PxDefaultAllocator();

		m_collisionManager = new CCollisionManager();

		m_contactManager = new CContactManager();

		m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_allocator,*m_errorManager);

		m_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(m_foundation);

#ifdef PX_WINDOWS
		PxCudaContextManagerDesc cudaContextManagerDesc;
		m_cudaContextManager = PxCreateCudaContextManager(*m_foundation, cudaContextManagerDesc, m_profileZoneManager);
		if(m_cudaContextManager){
			if(!m_cudaContextManager->contextIsValid()){
				m_cudaContextManager->release();
				m_cudaContextManager = nullptr;
			}
		}
#endif

		PxTolerancesScale toleranceScale;
		bool recordMemoryAllocations = true;
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION,*m_foundation,toleranceScale,recordMemoryAllocations,m_profileZoneManager);

		PxCookingParams params(toleranceScale);
		m_cooking = PxCreateCooking(PX_PHYSICS_VERSION,*m_foundation,params);

		if (!PxInitExtensions(*m_physics))
			assert(false && "PxInitExtensions failed!");

		float staticFriction = 0.5f;
		float dynamicFriction = 0.5f;
		float restitution = 0.1f;
		m_defaultMaterial = m_physics->createMaterial(staticFriction,dynamicFriction,restitution);

		m_pvdConnection = 0;

#ifdef _DEBUG
		debugger::comm::PvdConnectionManager *pvdConnectionManager = m_physics->getPvdConnectionManager();
		assert(pvdConnectionManager && "Error en PxPhysics::getPvdConnectionManager");

		const char *ip = "127.0.0.1";
		int port = 5425;						
		unsigned int timeout = 100;				

		PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

		m_pvdConnection = PxVisualDebuggerExt::createConnection(pvdConnectionManager, ip, port, timeout, connectionFlags);

#endif
	}

	CPhysicManager::~CPhysicManager()
	{
		if (m_pvdConnection) {
			m_pvdConnection->release();
			m_pvdConnection = nullptr;
		}

		if(m_defaultMaterial){
			m_defaultMaterial->release();
			m_defaultMaterial = nullptr;
		}

		if(m_cooking){
			m_cooking->release();
			m_cooking = nullptr;
		}

		if (m_cudaContextManager) {
			m_cudaContextManager->release();
			m_cudaContextManager = nullptr;
		}

		if(m_physics){
			m_physics->release();
			m_physics = nullptr;
		}

		if(m_profileZoneManager){
			m_profileZoneManager->release();
			m_profileZoneManager = nullptr;
		}

		PxCloseExtensions();

		if(m_foundation){
			m_foundation->release();
			m_foundation = nullptr;
		}

		if(m_contactManager){
			delete m_contactManager;
			m_contactManager = nullptr;
		}

		if(m_collisionManager){
			delete m_collisionManager;
			m_collisionManager = nullptr;
		}

		if (m_allocator) {
			delete m_allocator;
			m_allocator = nullptr;
		}

		if (m_errorManager) {
			delete m_errorManager;
			m_errorManager = nullptr;
		}
	}

	bool CPhysicManager::Init() 
	{
		if (!m_instance) {
			m_instance = new CPhysicManager();
		}

		return true;
	}

	void CPhysicManager::Release()
	{
		if(m_instance) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	void CPhysicManager::createScene ()
	{
		assert(m_instance);

		PxSceneDesc sceneDesc(m_physics->getTolerancesScale());

		sceneDesc.simulationEventCallback = m_collisionManager;
		sceneDesc.contactModifyCallback = m_contactManager;

		if (!sceneDesc.cpuDispatcher) {
			int mNbThreads = 1;
			m_cpuDispatcher  = PxDefaultCpuDispatcherCreate(mNbThreads);
			assert (m_cpuDispatcher && "Error en PxDefaultCpuDispatcherCreate");

			sceneDesc.cpuDispatcher = m_cpuDispatcher;
		}

		if (!sceneDesc.filterShader)
			sceneDesc.filterShader = CCollisionManager::PxCustomSpaceFilterShader;

		sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_PAIRS;
		sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS;

#ifdef PX_WINDOWS
		if (!sceneDesc.gpuDispatcher && m_cudaContextManager)
		{
			sceneDesc.gpuDispatcher = m_cudaContextManager->getGpuDispatcher();
		}
#endif
		m_scene = m_physics->createScene(sceneDesc);
	}

	void CPhysicManager::destroyScene ()
	{
		assert(m_instance);

		if(m_scene){
			m_scene->release();
			m_scene = nullptr;
		}


		if (m_cpuDispatcher) {
			m_cpuDispatcher->release();
			m_cpuDispatcher = nullptr;
		}
	}

	bool CPhysicManager::tick(unsigned int msecs) 
	{
		assert(m_scene);
		m_scene->simulate(msecs / 1000.0f);
		return m_scene->fetchResults(true);
	}

	PxRigidStatic* CPhysicManager::createPlane(const Vector3 &point, const Vector3 &normal, int group, 
										const IPhysic *component)
	{
		assert(m_scene);

		PxVec3 p = Vector3ToPxVec3(point);
		PxVec3 n = Vector3ToPxVec3(normal);
		PxPlane plane(p,n);
		PxRigidStatic *actor = PxCreatePlane(*m_physics,plane, *m_defaultMaterial);

		actor->userData = (void*)component;
		PxSetGroup(*actor,group);
		//m_scene->addActor(*actor);

		return actor;
	}
	PxRigidStatic* CPhysicManager::createStaticBox(const Vector3 &position, const Vector3 &dimensions, bool trigger, 
											int group, const IPhysic *component)
	{
		assert(m_scene);
		PxVec3 p = Vector3ToPxVec3(position);
		PxVec3 d = Vector3ToPxVec3(dimensions);

		PxTransform pose(p);
		PxBoxGeometry geom(d);
		PxTransform localPose(PxVec3(0,dimensions.y,0));

		PxRigidStatic *actor = PxCreateStatic(*m_physics,pose,geom,*m_defaultMaterial,localPose);

		if(trigger){
			PxShape *shape; actor->getShapes(&shape,1,0);
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		actor->userData = (void*)component;

		PxSetGroup(*actor,group);

		setupFiltering(actor,FilterGroup::eSPACE_FILTER,FilterGroup::eSPACE_FILTER);

		//m_scene->addActor(*actor);
	
		return actor;
	}
	PxRigidDynamic* CPhysicManager::createDynamicBox(const Vector3 &position, const Vector3 &dimensions, 
											  float mass, bool kinematic, bool trigger, int group, 
											  const IPhysic *component)
	{
		assert(m_scene);

		PxTransform pose(Vector3ToPxVec3(position));
		PxBoxGeometry geom(Vector3ToPxVec3(dimensions));
		PxMaterial *material = m_defaultMaterial;
		float density = mass / (dimensions.x * dimensions.y * dimensions.z);
		PxTransform localPose(PxVec3(0, dimensions.y, 0));

		PxRigidDynamic *actor = nullptr;
		if(kinematic){
			actor = PxCreateKinematic(*m_physics,pose,geom,*material,density,localPose);
		}else{
			actor = PxCreateDynamic(*m_physics,pose,geom,*material,density,localPose);
		}

		if(trigger){
			PxShape *shape; actor->getShapes(&shape,1,0);
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		actor->userData = (void*)component;

		PxSetGroup(*actor,group);

		setupFiltering(actor,FilterGroup::eSPACE_FILTER,FilterGroup::eSPACE_FILTER);

		//m_scene->addActor(*actor);

		return actor;
	}

	physx::PxRigidStatic* CPhysicManager::createStaticSphere(const Vector3 &position, const float &radius, bool trigger, int group, const Logic::Component::IPhysic *component)
	{
		assert(m_scene);
		PxVec3 p = Vector3ToPxVec3(position);

		PxTransform pose(p);
		PxSphereGeometry geom(radius);

		PxRigidStatic *actor = PxCreateStatic(*m_physics,pose,geom,*m_defaultMaterial);

		if(trigger){
			PxShape *shape; actor->getShapes(&shape,1,0);
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		actor->userData = (void*)component;

		PxSetGroup(*actor,group);

		setupFiltering(actor,FilterGroup::eSPACE_FILTER,FilterGroup::eSPACE_FILTER);

		//m_scene->addActor(*actor);
	
		return actor;
	}

	physx::PxRigidDynamic* CPhysicManager::createDynamicSphere(const Vector3 &position, const float &radius, float mass, bool kinematic, bool trigger, int group, const Logic::Component::IPhysic *component)
	{
		assert(m_scene);

		PxTransform pose(Vector3ToPxVec3(position));
		PxSphereGeometry geom(radius);
		PxMaterial *material = m_defaultMaterial;
		float density = mass / ((4/3) * Common::Util::Math::PI * radius * radius * radius);

		PxRigidDynamic *actor = nullptr;
		if(kinematic){
			actor = PxCreateKinematic(*m_physics,pose,geom,*material,density);
		}else{
			actor = PxCreateDynamic(*m_physics,pose,geom,*material,density);
		}

		if(trigger){
			PxShape *shape; actor->getShapes(&shape,1,0);
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		actor->userData = (void*)component;

		PxSetGroup(*actor,group);

		setupFiltering(actor,FilterGroup::eSPACE_FILTER,FilterGroup::eSPACE_FILTER);

		PxD6Joint* joint = PxD6JointCreate(*m_physics, actor, PxTransform::createIdentity(), nullptr, actor->getGlobalPose());
		joint->setMotion(PxD6Axis::eX,PxD6Motion::eFREE);
		joint->setMotion(PxD6Axis::eY,PxD6Motion::eLOCKED);
		joint->setMotion(PxD6Axis::eZ,PxD6Motion::eFREE);
		joint->setMotion(PxD6Axis::eSWING1,PxD6Motion::eFREE);
		joint->setMotion(PxD6Axis::eSWING2,PxD6Motion::eLOCKED);
		joint->setMotion(PxD6Axis::eTWIST,PxD6Motion::eLOCKED);
		//TODO release

		//m_scene->addActor(*actor);

		return actor;
	}

	PxRigidActor* CPhysicManager::createFromFile(const std::string &file, int group, const IPhysic *component)
	{
		assert(m_scene);

		PxSerializationRegistry* registry = PxSerialization::createSerializationRegistry(*m_physics); 
		PxDefaultFileInputData data(file.c_str());
		PxCollection* collection;

		collection = PxSerialization::createCollectionFromXml(data, *m_cooking, *registry);
	
		m_scene->addCollection(*collection); 
	
		PxRigidActor *actor = nullptr;
		for (unsigned int i=0; (i<collection->getNbObjects()) && !actor; i++) {
			actor = collection->getObject(i).is<PxRigidActor>();		
		}
		assert(actor);

		actor->userData = (void*)component;

		PxSetGroup(*actor,group);

		collection->release();
		registry->release();

		return actor;
	}
	void CPhysicManager::destroyActor(physx::PxActor *actor)
	{
		assert(m_scene);
		m_scene->removeActor(*actor);
		actor->release();
	}
	Matrix4 CPhysicManager::getActorTransform(const PxRigidActor *actor)
	{
		assert(actor);
		return PxTransformToMatrix4(actor->getGlobalPose());
	}
	void CPhysicManager::moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform)
	{
		assert(actor);
		assert(isKinematic(actor));
		actor->setKinematicTarget(Matrix4ToPxTransform(transform));
	}

	void CPhysicManager::moveDynamicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform)
	{
		assert(actor);
		actor->setGlobalPose(Matrix4ToPxTransform(transform));
	}

	void CPhysicManager::moveKinematicActor(physx::PxRigidDynamic *actor, const Vector3 &displ)
	{
		assert(actor);
		assert(isKinematic(actor));
		PxTransform tf = actor->getGlobalPose();
		tf.p += Vector3ToPxVec3(displ);
		actor->setKinematicTarget(tf);
	}

	bool CPhysicManager::isKinematic(const PxRigidDynamic *actor)
	{
		assert(actor);
		return actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC;
	}
	void CPhysicManager::setGroupCollisions(int group1, int group2, bool enable)
	{
		PxSetGroupCollisionFlag(group1, group2, enable);
	}

	Logic::CEntity* CPhysicManager::raycastClosest (const Ray& ray, float maxDist) const
	{
		return nullptr;
	}

	Logic::CEntity* CPhysicManager::raycastClosest(const Ray& ray, float maxDist, int group) const 
	{
		assert(m_scene);
		PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());
		PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());
		PxReal maxDistance = maxDist;
		PxRaycastHit hit;                 
		const PxSceneQueryFlags outputFlags;

		PxRaycastHit hits[64];
		bool blockingHit;
		PxI32 nHits = m_scene->raycastMultiple(origin, unitDir, maxDistance, outputFlags, hits, 64, blockingHit); 
	
		for (int i=0; i<nHits; i++) {
			PxRigidActor *actor = hits[i].shape->getActor();
			if (PxGetGroup(*actor) == group) {
				IPhysic *component = (IPhysic *) actor->userData;
				if (component) {
					return component->getEntity();
				}
			}
		}

		return nullptr;
	}

	const float CPhysicManager::getActorRadius(physx::PxRigidDynamic* actor)
	{
		PxShape* buff;
		PxU32 num =  actor->getShapes(&buff,actor->getNbShapes());
		if(num == 1){
			PxSphereGeometry geom;
			if(buff->getSphereGeometry(geom)){
				return geom.radius;
			}
		}
		return 0;
	}

	bool CPhysicManager::activateActor(physx::PxRigidActor *actor, bool trigger)
	{
		if(trigger){
			PxShape *shape; actor->getShapes(&shape,1,0);
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		m_scene->addActor(*actor);
		return true;
	}

	void CPhysicManager::deactivateActor(physx::PxRigidActor *actor)
	{
		PxShape *shape; actor->getShapes(&shape,1,0);
		
		if(shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE){
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
		}
		m_scene->removeActor(*actor);
	}

	void CPhysicManager::setupFiltering(physx::PxRigidActor* actor, unsigned int filterGroup, unsigned int filterMask)
	{
		using namespace physx;
		PxFilterData filterData;
		filterData.word0 = filterGroup;
		filterData.word1 = filterMask;
		const PxU32 numShapes = actor->getNbShapes();
		PxShape** shapes = (PxShape**) malloc (sizeof(PxShape*)*numShapes);
		actor->getShapes(shapes, numShapes);
		for(PxU32 i = 0; i < numShapes; i++)
		{
				PxShape* shape = shapes[i];
				shape->setSimulationFilterData(filterData);
		}
		free(shapes);
	}
}}