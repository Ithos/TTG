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

#ifndef __LOGIC_COMPONENT_H
#define __LOGIC_COMPONENT_H

#include <string>

#include "Logic/ComponentFactory.h"
#include "../MessageManager.h"

namespace Map
{
	class CMapEntity;
}

namespace Logic
{
	class CEntity;
	class CScene;
}

namespace Logic
{
	namespace Component
	{
		class IComponent : public CMessageManager
		{
		public:
			IComponent() : m_entity(0),m_priority(-1),m_name(""),m_num(0),m_activate(false){}
			virtual ~IComponent(){}

			virtual bool spawn(CEntity* entity, CScene *scene, const Map::CMapEntity *entityInfo);

			virtual bool activate(){ return m_activate = true;}
			virtual void deactivate() {m_activate = false;}


			virtual void tick(unsigned int msec);
			CEntity *getEntity()const {return m_entity;}
			const std::string& getName() const {return m_name;}
			void setName(const std::string &name){m_name = name;}

			void setPriority(int p){m_priority = p;}
			int getPriority(){return m_priority;}

			void setPosition(unsigned int p){m_num = p;}
			unsigned int getPosition(){return m_num;}

			bool isActivated() { return m_activate; }

		protected:
			friend class CEntity;
			void setEntity(CEntity *entity){m_entity = entity;}
			CEntity *m_entity;
			std::string m_name;
			int m_priority;
			unsigned int m_num;

			bool m_activate;
		};

#define DEC_FACTORY(Class) \
public: \
    static IComponent* create(); \
	static bool regist(); \

#define IMP_FACTORY(Class) \
IComponent* Class::create() \
{ \
	IComponent* res = new Class(); \
	return res; \
} \
bool Class::regist() \
{ \
	if (!CComponentFactory::getInstance()->has(#Class)) \
	{ \
		CComponentFactory::getInstance()->add(Class::create, #Class); \
	} \
	return true; \
}

#define REG_FACTORY(Class) \
static bool RegisteredFactory_##Class = Class::regist();

	}
}

#endif