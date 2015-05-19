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

#include "Entity.h"

#include <log.h>

#include "Components/Component.h"

#include <GUI/Controllers/PlayerController.h>
#include <GUI/GUI.h>
#include <GUI/InputManager.h>

#include <Common/Map/MapEntity.h>

namespace Logic
{
	const char* const LOG_CENTITY = "Logic::CEntity";

	CEntity::CEntity(TEntityID id): m_id(id),
									m_name(""),
									m_type(""),
									m_components(0),
									m_activated(false),
									m_scene(0),
									m_isPlayer(false),
									m_frameCounter(0){}
	CEntity::~CEntity()
	{
		removeAllComponents();
		GUI::CGUI::getInstance()->getPlayerController()->setControllerAvatar(nullptr);
	}

    bool CEntity::spawnEx(CEntity* parent, CScene* scene, const Map::CMapEntity *entity) 
    {
        m_type = entity->getType();

        if (entity->hasAttribute("name"))
			m_name = entity->getStringAttribute("name");

        bool ok = true;
        for (auto it = m_components.begin(); it != m_components.end() && ok; ++it){
            ok = ok && (*it)->spawn(parent, scene, entity);
		}

        return ok;
    }

	bool CEntity::spawn(CScene *scene, const Map::CMapEntity *entity)
	{
		m_scene = scene;
		
		m_type = entity->getType();

		if(entity->hasAttribute("name"))
			m_name = entity->getStringAttribute("name");

		if(entity->hasAttribute("isPlayer")){
			m_isPlayer = entity->getBoolAttribute("isPlayer");
			GUI::CGUI::getInstance()->getPlayerController()->setControllerAvatar(this);
			GUI::CInputManager::getInstance()->addKeyListener(GUI::CGUI::getInstance()->getPlayerController());
			GUI::CInputManager::getInstance()->addMouseListener(GUI::CGUI::getInstance()->getPlayerController());
		}

		TComponentList::const_iterator it = m_components.begin();
		bool fine  = true;
		for (;it != m_components.end() && fine; ++it){
			fine = fine && (*it)->spawn(this,scene,entity);
		}

		return fine;
	}

	bool CEntity::activate()
	{
		m_activated = true;

		if(m_isPlayer){
			GUI::CGUI::getInstance()->getPlayerController()->setControllerAvatar(this);
		}

		TComponentList::const_iterator it = m_components.begin();
		for (; it != m_components.end() && m_activated; ++it){
			m_activated = m_activated && (*it)->activate();
		}
		return m_activated;
	}

	void CEntity::deactivate()
	{
		TComponentList::const_iterator it = m_components.begin();
		for(; it != m_components.end(); ++it){
			(*it)->deactivate();
		}
		m_activated = false;
		if(m_isPlayer) GUI::CGUI::getInstance()->getPlayerController()->setControllerAvatar(nullptr);
	}

	void CEntity::tick(unsigned int msecs)
	{
		if(!m_activated) return;
		if (m_frameCounter > MAX_FRAMES) 
            m_frameCounter = 0;

		TComponentList::const_iterator it (m_tickComponents.begin());
		for(; it != m_tickComponents.end(); ++it) {
			if (*it && (m_frameCounter % (*it)->getPriority() == 0) && (*it)->isActivated()) 
				(*it)->tick(msecs);
		}
		++m_frameCounter;

		onTickDeactivate();
		onTickActivate();
		onTickDelete();
		onTickCreate();
	}

	void CEntity::addComponent(Component::IComponent* comp)
	{
		m_components.push_back(comp);
		comp->setEntity(this);
	}

	void CEntity::addComponent(Component::IComponent* comp, unsigned int number, int priority)
	{
		m_components.push_back(comp);
		comp->setEntity(this);
		comp->setPosition(number);
		if(priority <= 0){
		//	log_trace(LOG_CENTITY,"Non tick Component\n");
			m_nonTickComponents.push_back(comp);
		} else {
			//log_trace(LOG_CENTITY,"Tick Component\n");
			// Adds the component in 'number' position
			comp->setPriority(priority);
            if (m_tickComponents.size() <= number)
                m_tickComponents.resize(number+1);
 			m_tickComponents[number] = comp;
		}
	}

	bool CEntity::removeComponent(Component::IComponent* comp)
	{
		TComponentList::const_iterator it = m_components.begin();
		bool find = false;
		while( it != m_components.end() && !find){
			if(*it == comp){
				it = m_components.erase(it);
				find = true;
			}else{
				++it;
			}
		}
		if(find) comp->setEntity(0);
		return find;
	}

	void CEntity::removeAllComponents()
	{
		Component::IComponent* comp;
		while(!m_components.empty()){
			comp = m_components.back();
			m_components.pop_back();
			delete comp;
		}
	}

	bool CEntity::sendMessage(const IMessage &msg, Component::IComponent *emitter)
	{
		bool received = false;
		TComponentList::const_iterator it = m_components.begin();
		for(; it != m_components.end(); ++it){
			if(emitter != *it){
				received = received || (*it)->set(msg);
			}
		}
		return received;
	}

	Component::IComponent* CEntity::getComponentByName(const std::string &name)
	{
		TComponentList::const_iterator it (m_components.begin());
		TComponentList::const_iterator end (m_components.end());
		while((it != end) && ((*it)->getName().compare(name))){
			++it;
		}
		return (it == end) ? nullptr : (*it);
	}

	void CEntity::createComponent(Component::IComponent* comp)
	{
		m_comp2create.push_back(comp);
	}

	void CEntity::activateComponent(Component::IComponent* comp)
	{
		m_comp2activate.push_back(comp);
	}

	void CEntity::deleteComponent(Component::IComponent* comp)
	{
		m_comp2delete.push_back(comp);
	}

	void CEntity::deactivateComponent(Component::IComponent* comp)
	{
		m_comp2deactivate.push_back(comp);
	}

	void CEntity::onTickActivate()
	{
		TComponentList::const_iterator it(m_comp2activate.begin());
		TComponentList::const_iterator end(m_comp2activate.end());

		while(it != end){
			(*it)->activate();
			++it;
		}

		m_comp2activate.clear();
	}

	void CEntity::onTickDeactivate()
	{
		TComponentList::const_iterator it(m_comp2deactivate.begin());
		TComponentList::const_iterator end(m_comp2deactivate.end());

		while(it != end){
			(*it)->deactivate();
			++it;
		}

		m_comp2deactivate.clear();
	}

	void CEntity::onTickCreate()
	{
		int size = m_components.size() + m_comp2create.size();
		//m_components.resize(size);

		TComponentList::const_iterator it(m_comp2create.begin());
		TComponentList::const_iterator end(m_comp2create.end());

		while(it != end){
			if(((*it)->getPriority() > 0) && ((*it)->getPosition() > m_tickComponents.back()->getPosition())){
				m_tickComponents.push_back((*it));
				m_components.push_back((*it));
			} else {
				//FIXME: Try to avoid this
				m_tickComponents.resize(m_tickComponents.size() + 1);
				m_components.resize(m_components.size() + 1);
				for(unsigned int i = (m_tickComponents.size() - 1); i > (*it)->getPosition(); i--){
					m_tickComponents[i] = m_tickComponents[i--];
					m_tickComponents[i]->setPosition(i/*m_tickComponents[i]->getPosition() + 1*/);
				}
				m_tickComponents[(*it)->getPosition()] = (*it);
			}

			if((*it)->getPriority() <= 0){
				m_nonTickComponents.push_back(*it);
				m_components.push_back(*it);
			}
			++it;
		}

		m_comp2create.clear();
	}

	void CEntity::onTickDelete()
	{
		TComponentList::const_iterator it(m_comp2delete.begin());
		TComponentList::const_iterator end(m_comp2delete.end());

		while(it != end){
			TComponentList::iterator it2(std::find(m_components.begin(),m_components.end(),(*it)));
			if(it2 != m_components.end()){
				m_components.erase(it2);
			}

			it2 = (std::find(m_nonTickComponents.begin(),m_nonTickComponents.end(),(*it)));
			if(it2 != m_nonTickComponents.end()){
				m_nonTickComponents.erase(it2);
			}

			it2 = (std::find(m_tickComponents.begin(),m_tickComponents.end(),(*it)));
			if(it2 != m_tickComponents.end()){
				m_tickComponents.erase(it2);
			}
			delete (*it);
			++it;
		}

		m_comp2delete.clear();
	}
}