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
/**
@file EntityFactory.h

Contains the declaration of the entities factory class.

@see Logic::CEntityFactory

@author Luis Valero Martin
@date January, 2015
*/

#ifndef __LOGIC_ENTITYFACTORY_H
#define __LOGIC_ENTITYFACTORY_H

#include <string>
#include <list>
#include <map>

#include "Entity/Entity.h"

#include <Common/Data/TTG_Types.h>

namespace Map
{
	class CMapEntity;
}

namespace Logic
{
	class CScene;
	class CEntity;
}

namespace Logic
{
	/**
	The CEntityFactory is a factory that creates and destroys the game entities. It is a singleton.
	Uses the CComponentFactory to assemble the entities from the descriptions of the blueprints.

	@ingroup logicGroup
	@ingroup mapGroup

	@author Luis Valero Martin
	@date January, 2015
	*/
	class CEntityFactory
	{
	public:
		static bool init();
		static void release();
		static CEntityFactory *getInstance() {return m_instance;}
		bool loadBluePrints(const std::string &filename);
		void unloadBluePrints();
		CEntity* createEntity(const Map::CMapEntity *entityInfo, CScene *scene = nullptr);
		void deleteEntity(CEntity* entity);
        void deleteEntityEx(CEntity* ent)
        { 
            delete ent; ent = nullptr;
        }
		void deferredDeleteEntity(CEntity* entity);
		void deleteDeferredEntities();

		struct TBluePrint
		{
			std::string type;
			std::list<std::string> components;
		};

        

	private:
		CEntityFactory();
		~CEntityFactory();

		bool open();
		void close();

		CEntity* assembleEntity(const std::string &type);

		static CEntityFactory *m_instance;

		static TEntityID m_nextId;
        static TEntityID nextID();

		typedef std::list<Logic::CEntity*> TEntityList;
		TEntityList m_pendingEntities;

		typedef std::map<std::string,TBluePrint> TBluePrintMap;
		TBluePrintMap m_blueprints;

		Common::Data::Blueprints::TBluePrintsMap m_xmlBlueprints;
	};
}

#endif