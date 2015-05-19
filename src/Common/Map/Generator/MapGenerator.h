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


#ifndef __MAP_MAPGENERATOR_H
#define __MAP_MAPGENERATOR_H

#include <string>
#include <vector>
#include <list>
#include <Map>
#include <chrono>
#include <ctime>

namespace Map
{
	class CMapEntity;
}

namespace Ogre
{
	class Vector3;
}

namespace Map
{

	class CMapGenerator
	{
	public:

		CMapGenerator();

		~CMapGenerator();

		typedef std::list<Map::CMapEntity*> TEntityList;

		void resetMaps() {m_SeedMap.clear();}

		TEntityList generateGalaxy(const std::string& descriptor);

		TEntityList generateSystem(const std::string& descriptor);

		TEntityList generatePlanet(const std::string& descriptor);

	private:

		bool checkDist(const std::vector<Ogre::Vector3>& vec,const Ogre::Vector3& pos,float dist, int vecNum = -1);

		typedef std::map<std::string,std::time_t> TSeedMap;

		TSeedMap m_SeedMap;
	};

}

#endif