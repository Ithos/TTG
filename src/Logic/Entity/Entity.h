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

#ifndef __LOGIC_ENTITY_H
#define __LOGIC_ENTITY_H

#include <string>
#include <vector>

namespace Map
{
	class CMapEntity;
}

namespace Logic
{
	class CScene;
	class CEntityFactory;
	class IMessage;
	namespace Component
	{
		class IComponent;
	}
}

namespace Logic
{
	typedef unsigned int TEntityID;
	enum 
	{
		UNASSIGNED = 0xFFFFFFFF,
		FIRST_ID = 0x00000000
	};
	const int MAX_FRAMES = 20;
}

namespace Logic
{
	class CEntity
	{
	friend class CEntityFactory;
	public:
		bool activate();
		void deactivate();
		void tick(unsigned int msecs);
		bool sendMessage(const IMessage &msg, Component::IComponent* emitter = 0);
		TEntityID getID() const {return m_id;}
		
		const std::string& getName() const {return m_name;}
		const std::string& getType() const {return m_type;}

		const CScene *getScene() const {return m_scene;}
		CScene * getScene() {return m_scene;}

		bool isActivated(){return m_activated;}
		bool isPlayer() {return m_isPlayer;}

		void removeScene(){m_scene = 0;}

		Component::IComponent* getComponentByName(const std::string &name);

		void createComponent(Component::IComponent* comp);
		void activateComponent(Component::IComponent* comp);
		void deleteComponent(Component::IComponent* comp);
		void deactivateComponent(Component::IComponent* comp);
        bool spawnEx(CEntity*, CScene*, const Map::CMapEntity *);

	//protected:
		friend class CScene;

		bool spawn(CScene *scene, const Map::CMapEntity *entity);

	private:

		void addComponent(Component::IComponent* comp);
        void addComponent(Component::IComponent* comp, unsigned int number, int priority);
		bool removeComponent(Component::IComponent* comp);
		void removeAllComponents();

		void onTickActivate();
		void onTickDeactivate();
		void onTickCreate();
		void onTickDelete();

		CEntity(TEntityID id);
		~CEntity();

		TEntityID m_id;
		std::string m_name;
		std::string m_type;
		bool m_isPlayer;

		typedef std::vector<Component::IComponent*> TComponentList;
		TComponentList m_components;
		TComponentList m_nonTickComponents;
		TComponentList m_tickComponents;

		TComponentList m_comp2activate;
		TComponentList m_comp2deactivate;
		TComponentList m_comp2create;
		TComponentList m_comp2delete;

		bool m_activated;
		CScene *m_scene;

		int m_frameCounter;
	};
}

#endif