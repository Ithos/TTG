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

#include <fstream>
#include <sstream>
#include <cassert>

#include <log.h>

#include "../MapEntity.h"
#include "MapParser.h"

#include "../../Configure/Configure.h"

namespace Map {

	const char* const LOG_CMAPPARSER = "Map::CMapParser";

	CMapParser::CMapParser(): m_traceScanning(false),
							  m_traceParsing(false),
							  m_entityInProgress(0)
	{

	}

	CMapParser::~CMapParser()
	{
		emptyEntityList();

	}

	bool CMapParser::parseStream(std::istream& in, const std::string& name)
	{
		return false;
	}

	bool CMapParser::parseFile(const std::string &filename)
	{
		log_trace(LOG_CMAPPARSER,"Parsing file %s...\n",filename.c_str());

		Common::Configuration::setDefaultFile(CONFIGURE_FILE);
		std::string path =
			Common::Configuration::getDefaultValue(MAPS_PATH);
		
		std::string completePath = path + filename;

		std::ifstream in(completePath.c_str());
		if (!in.good()) return false;
		return parseStream(in, completePath);
	}

	bool CMapParser::parseString(const std::string &input, const std::string& name)
	{
		std::istringstream iss(input);
		return parseStream(iss, name);
	}

	void CMapParser::error(const class location& l,
			   const std::string& m)
	{
		log_error(LOG_CMAPPARSER,"Error\n");
	}

	void CMapParser::error(const std::string& m)
	{
		log_error(LOG_CMAPPARSER,"Error\n");
	}
		
	void CMapParser::emptyEntityList()
	{
		m_entityList.clear();
	}

}
