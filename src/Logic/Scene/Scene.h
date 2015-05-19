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

#ifndef __LOGIC_SCENE_H
#define __LOGIC_SCENE_H

#include <string>
#include <map>
#include <list>

#include "../Entity/Entity.h"

#include <Common/Data/TTG_Types.h>

#include <OgreRenderQueueListener.h>
#include <OgreRoot.h>

// render queues
#define RENDER_QUEUE_OUTLINE_GLOW_OBJECTS	Ogre::RENDER_QUEUE_MAIN + 1
#define RENDER_QUEUE_OUTLINE_GLOW_GLOWS		Ogre::RENDER_QUEUE_MAIN + 2
#define RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW	Ogre::RENDER_QUEUE_MAIN + 3
#define RENDER_QUEUE_FULL_GLOW_GLOW			Ogre::RENDER_QUEUE_MAIN + 4
#define LAST_STENCIL_OP_RENDER_QUEUE		RENDER_QUEUE_FULL_GLOW_GLOW

// stencil values
#define STENCIL_VALUE_FOR_OUTLINE_GLOW 1
#define STENCIL_VALUE_FOR_FULL_GLOW 2
#define STENCIL_FULL_MASK 0xFFFFFFFF
#define STENCIL_NULL_MASK 0x00000000

namespace Logic
{
	class CEntity;
}

namespace Ogre
{
	class Root;
	class RenderWindow;
	class SceneManager;
	class StaticGeometry;
	class SceneNode;
	class Camera;
	class TestureManager;
}

namespace Logic
{
	// a Render queue listener to change the stencil mode
	class StencilOpQueueListener : public Ogre::RenderQueueListener 
	{ 
	public: 
		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation); 
		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation); 
	};

	class CScene
	{
	friend class CEntityFactory;
	public:
		CScene(const std::string &name,Ogre::Root* root, Ogre::RenderWindow* render,
			Common::Data::SceneType type);

		~CScene();

		static CScene *createSceneFromFile(const std::string &filename,
			Ogre::Root* root, Ogre::RenderWindow* render, Common::Data::SceneType type);

		bool activate();
		void deactivate();

		void tick(unsigned int msecs);

		void physxTick(unsigned int msecs);

		void createEntity(CEntity* entity);
		void activateEntity(CEntity* entity);
		void deleteEntity(CEntity* entity);
		void deactivateEntity(CEntity* entity);

		CEntity* getEntityByID(TEntityID entityID);
		CEntity* getEntityByName(const std::string &name, CEntity* start = 0);
		CEntity* getEntityByType(const std::string &type, CEntity* start = 0);

		Ogre::SceneManager* getSceneManager() {return m_sceneMgr;}
		const Ogre::SceneManager* getConstSceneManager() const {return m_sceneMgr;}

        Ogre::RenderWindow* getRenderWindow() { return m_renderWindow; }

        void setSceneCamera(Ogre::Camera* cam) { m_camera = cam; }
        Ogre::Camera* getSceneCamera() const { return m_camera; }

		Ogre::TextureManager* getRootTextureManager() const {return m_root->getTextureManager();}

		void addStaticOgreNode(Ogre::SceneNode* node);
		void addDynamicOgreNode(Ogre::SceneNode* node);

		void removeStaticOgreNode(Ogre::SceneNode* node);
		void removeDynamicOgreNode(Ogre::SceneNode* node);

	protected:
	private:

		void addEntity(CEntity* entity);
		void removeEntity(CEntity* entity);
		void removeAllEntities();

		void onTickActivate();
		void onTickDeactivate();
		void onTickCreate();
		void onTickDelete();

		void createOgreScene();
		void destroyOgreScene();
		void activateOgreScene();
		void deactivateOgreScene();

		typedef std::map<TEntityID,CEntity*> TEntityMap;
		TEntityMap m_entities;
		TEntityMap m_ent2deactivate;
		TEntityMap m_ent2delete;
		TEntityMap m_ent2activate;
		TEntityMap m_ent2create;
		std::string m_name;

		Ogre::Root*             m_root;
		Ogre::RenderWindow*     m_renderWindow;
		Ogre::SceneManager*     m_sceneMgr;
		Ogre::StaticGeometry*   m_ogreStaticGeom;
        Ogre::Camera*           m_camera;
		StencilOpQueueListener* m_StencilOpFrameListener;

		typedef std::list<Ogre::SceneNode*> TOgreEnitiesList;
		
		TOgreEnitiesList m_staticGeom;
		//not sure if needed
		TOgreEnitiesList m_dynamicGeom;

		Common::Data::SceneType m_type;

		bool m_isAlive;
	};
}

#endif