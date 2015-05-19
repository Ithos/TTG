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

#include "MapEntity.h"

namespace Map
{
	typedef std::pair<std::string, std::string> TSSPar;

	void CMapEntity::setAttribute(const std::string &attr, const std::string &value)
	{
		TSSPar elem(attr,value);
		if(m_attr.count(attr))
			m_attr.erase(attr);
		m_attr.insert(elem);
	}
		
	bool CMapEntity::hasAttribute(const std::string &attr) const
	{
		// Special checks for name & type
		if(!attr.compare("name"))
			return true;
		if(!attr.compare("type"))
			return true;
		if(m_attr.count(attr))
			return true;
		return false;
	}
		
	const std::string &CMapEntity::getStringAttribute(const std::string &attr) const
	{
		// Special checks for name & type
		if(!attr.compare("name"))
			return m_name;
		if(!attr.compare("type"))
			return m_type;
		return (*m_attr.find(attr)).second;
	}
		
	int CMapEntity::getIntAttribute(const std::string &attr) const
	{
		return atoi((*m_attr.find(attr)).second.c_str());
	}
		
	float CMapEntity::getFloatAttribute(const std::string &attr) const
	{
		return (float)atof((*m_attr.find(attr)).second.c_str());
	}
		
	double CMapEntity::getDoubleAttribute(const std::string &attr) const
	{
		return atof((*m_attr.find(attr)).second.c_str());
	}
		
	bool CMapEntity::getBoolAttribute(const std::string &attr) const
	{
		if(!(*m_attr.find(attr)).second.compare("true"))
			return true;
		else if(!(*m_attr.find(attr)).second.compare("false"))
			return false;
		throw new std::exception("Boolean attribute is not true nor false.");
	}
		
	const Ogre::Vector3 CMapEntity::getVector3Attribute(const std::string &attr) const
	{
		// Parse the string  "x y z"
		std::string position = (*m_attr.find(attr)).second;
		int space1 = position.find(' ');
		float x = (float)atof(position.substr(1,space1).c_str());
		int space2 = position.find(' ',space1+1);
		float y = (float)atof(position.substr(space1+1,space2-(space1+1)).c_str());
		float z = (float)atof(position.substr(space2+1,position.size()-(space2+1)).c_str());

		return Ogre::Vector3(x,y,z);
	}
}