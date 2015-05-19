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

#include "Scene.h"

#include <log.h>

#include <OgreRenderWindow.h>
#include <OgreStaticGeometry.h>

#include <PxPhysicsAPI.h>

#include <Common/Map/Map.h>
#include <Common/Physic/PhysicManager.h>

#include "../EntityFactory.h"

#include <Common/Particles/ParticleManager.h>

namespace Logic
{
	const char* const LOG_CSCENE = "Logic::CScene";

	void StencilOpQueueListener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{ 
		if (queueGroupId == RENDER_QUEUE_OUTLINE_GLOW_OBJECTS) // outline glow object 
		{ 
			Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 

			rendersys->clearFrameBuffer(Ogre::FBT_STENCIL); 
			rendersys->setStencilCheckEnabled(true); 
			rendersys->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS,
				STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK,STENCIL_FULL_MASK, 
				Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
		} 
		if (queueGroupId == RENDER_QUEUE_OUTLINE_GLOW_GLOWS)  // outline glow
		{ 
			Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
			rendersys->setStencilCheckEnabled(true); 
			rendersys->setStencilBufferParams(Ogre::CMPF_NOT_EQUAL,
				STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK,STENCIL_NULL_MASK, 
				Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
		} 
		if (queueGroupId == RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW)  // full glow - alpha glow
		{ 
			Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
			rendersys->setStencilCheckEnabled(true); 
			rendersys->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS,
				STENCIL_VALUE_FOR_FULL_GLOW,STENCIL_FULL_MASK,STENCIL_FULL_MASK, 
				Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
		} 

		if (queueGroupId == RENDER_QUEUE_FULL_GLOW_GLOW)  // full glow - glow
		{ 
			Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
			rendersys->setStencilCheckEnabled(true); 
			rendersys->setStencilBufferParams(Ogre::CMPF_EQUAL,
				STENCIL_VALUE_FOR_FULL_GLOW,STENCIL_FULL_MASK,STENCIL_NULL_MASK, 
				Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_ZERO,false);       
		} 

	} 

	void StencilOpQueueListener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
	{ 
			Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
			rendersys->setStencilCheckEnabled(false); 
			rendersys->setStencilBufferParams(); 
	} 

	CScene::CScene(const std::string &name,Ogre::Root* root, Ogre::RenderWindow* render,
		Common::Data::SceneType type)
		: 
        m_name(name),m_root(root),m_renderWindow(render),m_type(type),m_sceneMgr(nullptr),m_isAlive(false),
		m_StencilOpFrameListener(nullptr),
		m_ogreStaticGeom(nullptr)
	{
		m_StencilOpFrameListener = new StencilOpQueueListener();
		createOgreScene();
		Common::Physic::CPhysicManager::getInstance()->createScene();
        // init particles
        Common::Particles::CParticleManager::getInstance()->init(m_sceneMgr);

        if (type  == Common::Data::SceneType::BATTLE) {
            Common::Particles::CParticleManager::getInstance()->initHits();
            Common::Particles::CParticleManager::getInstance()->initExplosions();
            Common::Particles::CParticleManager::getInstance()->initShoots();
        }
	}

	CScene::~CScene()
	{
		removeAllEntities();

		Common::Physic::CPhysicManager::getInstance()->destroyScene();

		destroyOgreScene();

        if (m_name == "planet") {
            Common::Particles::CParticleManager::getInstance()->releaseHits();
            Common::Particles::CParticleManager::getInstance()->releaseExplosions();
            Common::Particles::CParticleManager::getInstance()->releaseShoots();
        }
	}

	CScene* CScene::createSceneFromFile(const std::string &filename,
						Ogre::Root* root, Ogre::RenderWindow* render, Common::Data::SceneType type)
	{
		log_trace(LOG_CSCENE,"Creating scene...\n");


		CScene *scene = new CScene(filename, root, render, type);

		if(type != Common::Data::DUMMY){

			Map::CMap *map = Map::CMap::getInstance();
			if(!map->parseFile(filename)){
				log_error(LOG_CSCENE,"Failed while creating the scene\n");
				return nullptr;
			}

			Map::CMap::TEntityList entityList = map->getEntityList();
		
			CEntityFactory *factory = CEntityFactory::getInstance();

			Map::CMap::TEntityList::const_iterator it(entityList.begin());
			Map::CMap::TEntityList::const_iterator end(entityList.end());

            int i = 0;
			for(; it != end; ++it, ++i){
				CEntity *ent = factory->createEntity((*it),scene);
				if(!ent){
					log_error(LOG_CSCENE,"Error while creating the entity...\n");
					return nullptr;
				}
			}
		}
		

		log_trace(LOG_CSCENE,"Scene created...\n");
		return scene;
	}

	bool CScene::activate()
	{
		if(!m_isAlive)
		{
			activateOgreScene();

			TEntityMap::const_iterator it(m_entities.begin());
			TEntityMap::const_iterator end(m_entities.end());

			bool ok = true;

			for(; it != end; ++it){
				ok = ok && (*it).second->activate();
			}

			m_isAlive = ok;

			return ok;
		}

		return true;
	}

	void CScene::deactivate()
	{
		if(m_isAlive)
		{
			TEntityMap::const_iterator it(m_entities.begin());
			TEntityMap::const_iterator end(m_entities.end());

			for(; it != end; ++it){
				if((*it).second->isActivated()){
					(*it).second->deactivate();
				}
			}

			deactivateOgreScene();

			m_isAlive = false;
		}
	}

	void CScene::tick(unsigned int msecs)
	{
		Common::Physic::CPhysicManager::getInstance()->tick(msecs);


		TEntityMap::const_iterator it(m_entities.begin());
		TEntityMap::const_iterator end(m_entities.end());
		for(; it != end; ++it){
			(*it).second->tick(msecs);
		}

		onTickDeactivate();
		onTickActivate();
		onTickDelete();
		onTickCreate();
	}

	void CScene::addEntity(CEntity* entity)
	{
		if(m_entities.count(entity->getID()) == 0){
			m_entities.insert(std::pair<TEntityID,CEntity*>(entity->getID(),entity));
		}
	}

	void CScene::removeEntity(CEntity* entity)
	{
		if(m_entities.count(entity->getID()) != 0){
			if(entity->isActivated()){
				entity->deactivate();
			}
			entity->removeScene();
			m_entities.erase(entity->getID());
		}
	}

	void CScene::removeAllEntities()
	{
		CEntityFactory *factory = CEntityFactory::getInstance();

		TEntityMap::const_iterator it(m_entities.begin());
		TEntityMap::const_iterator end(m_entities.end());

		while(it != end){
			CEntity* entity = (*it).second;
			it++;
			factory->deleteEntity(entity);
		}

		m_entities.clear();
	}

	CEntity* CScene::getEntityByID(TEntityID entityID)
	{
		if(m_entities.count(entityID) == 0) return 0;

		return (*m_entities.find(entityID)).second;
	}

	CEntity* CScene::getEntityByName(const std::string &name,CEntity* start)
	{
		TEntityMap::const_iterator it(m_entities.begin());
		TEntityMap::const_iterator end(m_entities.end());

		if(start){
			it = m_entities.find(start->getID());
			if(it == end) return nullptr;
			it++;
		}

		for(; it != end; ++it){
			if(!(*it).second->getName().compare(name))
				return (*it).second;
		}

		return nullptr;
	}

	CEntity* CScene::getEntityByType(const std::string &type, CEntity* start)
	{
		TEntityMap::const_iterator it(m_entities.begin());
		TEntityMap::const_iterator end(m_entities.end());

		if(start){
			it = m_entities.find(start->getID());
			if(it == end) return nullptr;
			it++;
		}

		for(; it != end; ++it){
			if(!(*it).second->getType().compare(type))
				return (*it).second;
		}

		return nullptr;
	}

	void CScene::createEntity(CEntity* entity)
	{
		if(m_ent2create.count(entity->getID()) == 0){
			m_ent2create.insert(std::pair<TEntityID,CEntity*>(entity->getID(),entity));
		}
	}

	void CScene::activateEntity(CEntity* entity)
	{
		if(m_ent2activate.count(entity->getID()) == 0){
			m_ent2activate.insert(std::pair<TEntityID,CEntity*>(entity->getID(),entity));
		}
	}

	void CScene::deleteEntity(CEntity* entity)
	{
		if(m_ent2delete.count(entity->getID()) == 0){
			m_ent2delete.insert(std::pair<TEntityID,CEntity*>(entity->getID(),entity));
		}
	}

	void CScene::deactivateEntity(CEntity* entity)
	{
		if(m_ent2deactivate.count(entity->getID()) == 0){
			m_ent2deactivate.insert(std::pair<TEntityID,CEntity*>(entity->getID(),entity));
		}
	}

	void CScene::onTickActivate()
	{
		TEntityMap::const_iterator it(m_ent2activate.begin());
		TEntityMap::const_iterator end(m_ent2activate.end());

		while(it != end){
			(*it).second->activate();
			it++;
		}

		m_ent2activate.clear();
	}

	void CScene::onTickDeactivate()
	{
		TEntityMap::const_iterator it(m_ent2deactivate.begin());
		TEntityMap::const_iterator end(m_ent2deactivate.end());

		while(it != end){
			(*it).second->deactivate();
			it++;
		}

		m_ent2deactivate.clear();
	}

	void CScene::onTickCreate()
	{
		TEntityMap::const_iterator it(m_ent2create.begin());
		TEntityMap::const_iterator end(m_ent2create.end());

		while(it != end){
			addEntity((*it).second);
			it++;
		}

		m_ent2create.clear();
	}

	void CScene::onTickDelete()
	{
		TEntityMap::const_iterator it(m_ent2delete.begin());
		TEntityMap::const_iterator end(m_ent2delete.end());

		while(it != end){
			CEntity* entity = (*it).second;
			it++;
			removeEntity(entity);
		}

		m_ent2delete.clear();
	}

	void CScene::createOgreScene()
	{
		log_trace(LOG_CSCENE,"Creating the Ogre scene...\n");

		//Depending on the scene type we have to create differents sceneManagers
		switch(m_type)
		{
		case Common::Data::SceneType::BATTLE:
			{
				m_sceneMgr = m_root->createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, m_name);
				break;
			}
		case Common::Data::SceneType::SYSTEM:
			{
				m_sceneMgr = m_root->createSceneManager(Ogre::ST_EXTERIOR_FAR, m_name);
				break;
			}
		case Common::Data::SceneType::GALAXY:
			{
				m_sceneMgr = m_root->createSceneManager(Ogre::ST_EXTERIOR_FAR, m_name);
				break;
			}
		case Common::Data::SceneType::DUMMY:
			{
				m_sceneMgr = m_root->createSceneManager(Ogre::ST_INTERIOR,m_name);
				break;
			}
		case Common::Data::SceneType::UNASSIGNED:
			{
				log_fatal(LOG_CSCENE,"Unassigned Scene type!\n");
				break;
			}
		default:
			{
				log_fatal(LOG_CSCENE,"Unknow Scene type!\n");
				break;
			}
		}

		m_sceneMgr->addRenderQueueListener(m_StencilOpFrameListener);

	}

	void CScene::destroyOgreScene()
	{
		log_trace(LOG_CSCENE,"Destroying ogre scene...\n");
	
		using namespace Ogre;

		m_sceneMgr->destroyStaticGeometry(m_ogreStaticGeom);
		m_root->destroySceneManager(m_sceneMgr);
	}

	void CScene::activateOgreScene()
	{
		if(!m_ogreStaticGeom && !m_staticGeom.empty()){
			m_ogreStaticGeom = m_sceneMgr->createStaticGeometry("static");

			TOgreEnitiesList::const_iterator it (m_staticGeom.begin());
			for(; it != m_staticGeom.end(); ++it){
				m_ogreStaticGeom->addSceneNode(*it);
				(*it)->detachAllObjects();
			}
			m_ogreStaticGeom->build();
		}

		log_trace(LOG_CSCENE,"Activating ogre scene...\n");
	}

	void CScene::deactivateOgreScene()
	{
		log_trace(LOG_CSCENE,"Deactivating ogre scene...\n");
	}

	void CScene::addStaticOgreNode(Ogre::SceneNode* node)
	{
		m_staticGeom.push_back(node);
	}

	void CScene::addDynamicOgreNode(Ogre::SceneNode* node)
	{
		m_dynamicGeom.push_back(node);
	}

	void CScene::removeStaticOgreNode(Ogre::SceneNode* node)
	{
		m_staticGeom.remove(node);
	}

	void CScene::removeDynamicOgreNode(Ogre::SceneNode* node)
	{
		m_staticGeom.remove(node);
	}

}