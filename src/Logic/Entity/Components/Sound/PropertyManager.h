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

#ifndef __LOGIC_PROPERTY_MANAGER_H
#define __LOGIC_PROPERTY_MANAGER_H

#include "../Component.h"
#include <vector>

namespace Logic
{
	namespace Component
	{
		class CPropertyManager : IComponent
		{
			DEC_FACTORY(CPropertyManager);
			public:
			CPropertyManager(){};
			~CPropertyManager();
			virtual bool spawn(CEntity* entity, CScene *scene, const Map::CMapEntity *entityInfo);
			virtual bool activate();
			virtual void deactivate();
			virtual void tick(unsigned int msec);
		private:
			int m_numBanks;
			std::vector<std::string> m_properties;
			std::vector<std::string> m_entities;
			std::vector<std::string> m_eventNames;
			std::vector<std::string> m_paramNames;
			std::vector<std::vector<int>> m_ranges;
		};
		REG_FACTORY(CPropertyManager);
	}
}

#endif