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

#ifndef __MAP_MAP_H
#define __MAP_MAP_H

#include "Parser/MapParser.h"
#include "Generator/MapGenerator.h"

#include <string>
#include <list>

namespace Map
{
	class CMapEntity;
}

namespace Map
{
	class CMap
	{
	public:
		static CMap* getInstance(){return m_instance;}

		static bool init();

		static void release();

		typedef std::list<Map::CMapEntity*> TEntityList;

		const TEntityList getEntityList() {return m_entityList;}

		void releaseEntityList();

		bool parseFile(const std::string &filename);

		void resetMaps() {m_mapGenerator.resetMaps();}

	private:
		static CMap* m_instance;

		TEntityList m_entityList;

		CMapGenerator m_mapGenerator;

		CMapParser m_mapParser;

		CMap();
		virtual ~CMap();
	};
}

#endif