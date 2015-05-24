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

#include "MissileWeapon_Linear.h"
#include "../Triggers/MissileTrigger.h"
#include "Common/Map/MapEntity.h"
#include "Common/Configure/Configure.h"
#include "Common/Data/Spawn_Constants.h"
#include "Logic/EntityFactory.h"

using namespace Common::Data;

namespace Logic
{
	namespace Component
	{
        const int MAX_MISSILES = 20;

        CMissileWeapon_Linear::CMissileWeapon_Linear(CEntity* parent, CScene* scene) : m_iMissile(0)
        {
            m_type = MISSILE_LINEAR;

            using namespace Common::Data::Spawn;
		    using namespace Common::Configuration;
            m_parent = parent;
            m_scene  = scene; 

            //init pool of missiles (
            for ( int i = 0; i < MAX_MISSILES; ++i) {
                m_mapInfo[i] = new Map::CMapEntity("Missile_linear" + std::to_string(i));
                m_mapInfo[i]->setType(getDefaultValue(GEN_MISSILE_LINEAR_TYPE));
                m_mapInfo[i]->setAttribute(PHYSIC_ENTITY,  getDefaultValue(GEN_MISSILE_LINEAR_TRIGGER_ENTITY));
                m_mapInfo[i]->setAttribute("physic_type", "kinematic");
                m_mapInfo[i]->setAttribute("physic_shape", "sphere");
                m_mapInfo[i]->setAttribute("physic_mass",  "1");
                m_mapInfo[i]->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_MISSILE_LINEAR_TRIGGER_RADIUS));
                m_mapInfo[i]->setAttribute(PHYSIC_TRIGGER, getDefaultValue(GEN_MISSILE_LINEAR_TRIGGER_ISTRIGGER));
                m_mapInfo[i]->setAttribute(MISSILE_SPEED,  getDefaultValue(GEN_MISSILE_LINEAR_SPEED));
                m_mapInfo[i]->setAttribute(MISSILE_DAMAGE, getDefaultValue(GEN_MISSILE_LINEAR_DAMAGE));
                m_mapInfo[i]->setAttribute(MISSILE_RANGE,  getDefaultValue(GEN_MISSILE_LINEAR_RANGE));
                
                CEntity* ent = CEntityFactory::getInstance()->createEntity(m_mapInfo[i], nullptr);
                m_subEntity.push_back(ent);
                static_cast<CMissileTrigger*>(ent->getComponentByName(MISSILE_TRIGGER))->setMove(); // no funtion->linear movement
            }
            
        }

        void CMissileWeapon_Linear::tick(unsigned int msecs)
        {
            for (auto it = m_subEntity.begin(); it != m_subEntity.end(); ++it)
                (*it)->tick(msecs);
        }

        void CMissileWeapon_Linear::shoot(const Vector3& src, const Vector3& dir)
        {
            // todo: gestion el pool de entidades.
            if (!m_trigger) {
                m_trigger = true;
                m_subEntity[m_iMissile]->spawnEx(m_parent, m_scene, m_mapInfo[m_iMissile]);
                m_subEntity[m_iMissile]->activate();
                static_cast<CMissileTrigger*>(m_subEntity[m_iMissile]->getComponentByName(MISSILE_TRIGGER))->shoot(src, dir);
                if (m_iMissile < MAX_MISSILES-1 )
                    ++m_iMissile;
                else
                    m_iMissile = 0;
            }
        }

    }
}