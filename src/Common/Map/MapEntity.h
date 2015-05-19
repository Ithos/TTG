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

#ifndef __MAP_MAPENTITY_H
#define __MAP_MAPENTITY_H

#include <string>
#include <map>
#include <OgreVector3.h>

namespace Map
{
	class CMapEntity
	{
	public:
		CMapEntity(const std::string &name): m_type(""), m_name(name){}
		~CMapEntity(){}

		void setAttribute(const std::string &attr, const std::string &value);
		
		bool hasAttribute(const std::string &attr) const;
		
		const std::string &getStringAttribute(const std::string &attr) const;
		
		int getIntAttribute(const std::string &attr) const;
		
		float getFloatAttribute(const std::string &attr) const;
		
		double getDoubleAttribute(const std::string &attr) const;
		
		bool getBoolAttribute(const std::string &attr) const;
		
		const Ogre::Vector3 getVector3Attribute(const std::string &attr) const;
		
		const std::string &getType() const {return m_type;}
		
		void setType(const std::string &type) {m_type = type;}
		
		const std::string &getName() const {return m_name;}
		
		void setName(const std::string &name) {m_name = name;}
	
	private:
		std::string m_type;
		std::string m_name;

		typedef std::map<std::string,std::string> TAttrList;
		TAttrList m_attr;
	};
}

#endif