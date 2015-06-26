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

#include "BombWeapon.h"
#include "../Triggers/MissileTrigger.h"
#include "Common/Map/MapEntity.h"
#include "Common/Configure/Configure.h"
#include "Common/Data/Spawn_Constants.h"
#include "Logic/EntityFactory.h"

#include <Application/Manager/GameManager.h>

namespace Logic
{
    namespace Component
    {
        const int MAX_BOMS = 20; 

        CBombWeapon::CBombWeapon(CEntity* parent, CScene* scene)
            : m_parent(nullptr), m_scene(nullptr), m_cost(0), m_timeToExplode(.0f)
        {
            using namespace Common::Data::Spawn;
		    using namespace Common::Configuration;

            m_type = STATIC_BOMB;
            m_parent = parent;
            m_scene  = scene; 


            //init pool of missiles (
            for ( int i = 0; i < MAX_BOMS; ++i) {
                m_mapInfo[i] = new Map::CMapEntity("Static_bomb" + std::to_string(i));
                m_mapInfo[i]->setType(getDefaultValue(GEN_STATICBOMB_TYPE));
                m_mapInfo[i]->setAttribute(PHYSIC_ENTITY,  getDefaultValue(GEN_STATICBOMB_TRIGGER_ENTITY));
                m_mapInfo[i]->setAttribute("physic_type", "static");
                m_mapInfo[i]->setAttribute("physic_shape", "sphere");
                m_mapInfo[i]->setAttribute("physic_mass",  "1");
                m_mapInfo[i]->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_STATICBOMB_TRIGGER_RADIUS));
                m_mapInfo[i]->setAttribute(PHYSIC_TRIGGER, getDefaultValue(GEN_STATICBOMB_TRIGGER_ISTRIGGER));
                m_mapInfo[i]->setAttribute(BOMB_DAMAGE, getDefaultValue(GEN_STATICBOMB_DAMAGE));
                m_mapInfo[i]->setAttribute(BOMB_COST,  getDefaultValue(GEN_STATICBOMB_COST));
                m_mapInfo[i]->setAttribute(BOMB_RANGE, getDefaultValue(GEN_STATICBOMB_RANGE));
                
                CEntity* ent = CEntityFactory::getInstance()->createEntity(m_mapInfo[i], nullptr);
                m_subEntity.push_back(ent);
            }

        }
        
        CBombWeapon::~CBombWeapon()
        {
        }

        void CBombWeapon::shoot(const Vector3& src)
        {
            if (m_trigger)
                return;
            else
                m_trigger = true;





        }

        void CBombWeapontick(unsigned int msecs)
        {}

        /**
            Set parameters for a weapon.
        */
		void CBombWeapon::setWeapon(const float& damage, const unsigned int& cost, const float& range, Common::Data::Weapons_t type)
        {
            
        }

    }
}