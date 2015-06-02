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

#include "PlayerWeapons.h"
#include "LaserWeapon.h"
#include "MissileWeapon_Linear.h"
#include "../Movement/Transform.h"
#include <log.h>

#include <Common/Map/MapEntity.h>
#include <Logic/Scene/Scene.h>
#include <Common/data/TTG_Types.h>
#include <Common/Data/Spawn_Constants.h>

namespace Logic 
{
    namespace Component
    {
        const char* const LOG_PWEAPON = "Logic::CWeapons";

        IMP_FACTORY(CWeapons);

        bool CWeapons::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            using namespace Common::Data;
            using namespace Common::Data::Spawn;

            if(!IComponent::spawn(entity, scene, entityInfo))
		        return false;

            if (entityInfo->hasAttribute(PHYSIC_RADIUS))
                m_shipRadius = entityInfo->getIntAttribute(PHYSIC_RADIUS) + 1;

            m_trans = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
            if (!m_trans)
                log_error(LOG_PWEAPON, "Error getting Transform, m_trans is null\n");

            m_weapons.push_back(new CLaserWeapon(scene, m_entity->getScene()->getSceneManager(), nullptr, entityInfo, entity));
            m_weapons.push_back(new CMissileWeapon_Linear(entity, scene));

            if (entityInfo->hasAttribute(COMMON_PRIMARY_WEAPON))
                m_primary = getWeapon(entityInfo->getStringAttribute(COMMON_PRIMARY_WEAPON));

            if (entityInfo->hasAttribute(COMMON_SECONDARY_WEAPON))
                m_secondary = getWeapon(entityInfo->getStringAttribute(COMMON_SECONDARY_WEAPON));

            return true;
        }

        CWeapons::~CWeapons()
        {
            for (auto it = m_weapons.begin(); it != m_weapons.end(); ++it)
                delete (*it);
            
            m_weapons.clear();
        }

        void CWeapons::tick(unsigned int msecs)
        {
            m_weapons[m_primary]->tick(msecs);
            m_weapons[m_secondary]->tick(msecs);
        }

        void CWeapons::shoot(int index)
        {
            Vector3 pos;
            using namespace Common::Data;
            log_trace(LOG_PWEAPON, "Shoot with secondary weapon\n");
            switch (m_weapons[index]->m_type)
            {
            case LASER:
                pos = m_trans->getPosition() + (m_shipRadius * Common::Util::Math::getDirection(m_trans->getTransform()));
                m_weapons[index]->shoot(pos, Common::Util::Math::getDirection(m_trans->getTransform()));
                break;
            case MISSILE_LINEAR:
                pos = m_trans->getPosition() + (m_shipRadius * Common::Util::Math::getDirection(m_trans->getTransform()));
                m_weapons[index]->shoot(pos, Common::Util::Math::getDirection(m_trans->getTransform()));
                break;
            }
        }

        /* For cadence */
        void CWeapons::releaseTrigger(int weapon)
        {
            using namespace Common::Data;
            log_trace(LOG_PWEAPON, "Release trigger primary weapon\n");
            switch (m_weapons[weapon]->m_type)
            {
            case LASER:
                static_cast<CLaserWeapon*>(m_weapons[weapon])->releaseTrigger(); break;
            case MISSILE_LINEAR:
                static_cast<CMissileWeapon_Linear*>(m_weapons[weapon])->releaseTrigger(); break;
            }
        }

        unsigned CWeapons::getWeapon(const std::string& weapon)
        {
            if (weapon == "laser")
                return 0;
            else if (weapon == "missile_linear")
                return 1;
            else
                return 0;
        }
    }
}