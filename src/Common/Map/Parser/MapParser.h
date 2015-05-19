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


#ifndef __MAP_MAPPARSER_H
#define __MAP_MAPPARSER_H

#include <string>
#include <list>

namespace Map
{
	class CMapEntity;
}

namespace Map {

	class CMapParser
	{
	public:

		CMapParser();

		~CMapParser();

		bool parseStream(std::istream& in,
				  const std::string& name = "stream input");

		bool parseString(const std::string& input,
				  const std::string& name = "string stream");

		bool parseFile(const std::string& filename);

		void error(const class location& l, const std::string& m);

		void error(const std::string& m);

		typedef std::list<Map::CMapEntity*> TEntityList;

		const TEntityList getEntityList() {return m_entityList;}

		void emptyEntityList();

	private:

		bool m_traceScanning;

		bool m_traceParsing;

		std::string m_streamname;

		Map::CMapEntity *m_entityInProgress;

		//Do not store any entityList here. This variable should be trated as temporal, it´s a member of the class
		//just because the parser requires it.
		TEntityList m_entityList;
	};

}

#endif
