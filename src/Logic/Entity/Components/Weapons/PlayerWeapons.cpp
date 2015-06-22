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
#include "LaserBeamWeapon.h"
#include "MissileWeapon_Linear.h"
#include "../Movement/Transform.h"
#include <log.h>

#include <Common/Map/MapEntity.h>
#include <Logic/Scene/Scene.h>
#include <Common/Data/Spawn_Constants.h>
#include <Common/Data/Game_Constants.h>
#include <Application/Manager/GameManager.h>

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
            m_weapons.push_back(new CLaserBeam(scene, m_entity->getScene()->getSceneManager(), nullptr, entityInfo, entity));

            if (entityInfo->hasAttribute(COMMON_PRIMARY_WEAPON))
                m_primary = getWeapon(entityInfo->getStringAttribute(COMMON_PRIMARY_WEAPON));

            if (entityInfo->hasAttribute(COMMON_SECONDARY_WEAPON))
                m_secondary = getWeapon(entityInfo->getStringAttribute(COMMON_SECONDARY_WEAPON));

			if(entity->isPlayer()){

				std::string primaryWeapon(Application::CGameManager::getInstance()->getPlayerActiveEquipment().find(Common::Data::Game::GAME_PRIMARY_WEAPON)->second.first);
				std::string secondaryWeapon(Application::CGameManager::getInstance()->getPlayerActiveEquipment().find(Common::Data::Game::GAME_SECONDARY_WEAPON)->second.first);

				if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[0][0]){
					m_primary = getWeapon("laser"); // Laser weapon
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(35.0f, 1.0f, 25, 600, 1.0f, -1);

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[1][0]){
					m_primary = getWeapon("laser");
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(10.0f, 1.0f, 5, 400, 1.0f, -1);

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[2][0]){

					/// TODO ///

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[3][0]){
					m_primary = getWeapon("laser");
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(60.0f, 1.0f, 40, 1000, 1.0f, -1);

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[4][0]){

					/// TODO ///

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[5][0]){
					m_primary = getWeapon("laser");
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(35.0f, 1.0f, 40, 600, 1.0f, -1, true);

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[6][0]){

					/// TODO ///

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[7][0]){
					m_primary = getWeapon("laser");
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(50.0f, 1.0f, 70, 1000, 1.0f, -1, true, 5.0f);

				}

				if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[0][0]){
					m_secondary = getWeapon("missile_linear");
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(80.0f, 1.0f, 70, 1000, 0.6f, -1);

				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[1][0]){
					m_secondary = getWeapon("missile_linear");
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(80.0f, 1.0f, 40, 1000, -0.1f, -1);

				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[2][0]){

					/// TODO ///

				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[3][0]){
					m_secondary = getWeapon("missile_linear");
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(50.0f, 1.0f, 50, 1000, 0.3f, -1, true, 20.0f, true);

				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[4][0]){
					m_secondary = getWeapon("missile_linear");
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(60.0f, 1.0f, 40, 1000, -0.1f, -1, true, 20.0f, true);
				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[5][0]){
					m_secondary = getWeapon("missile_linear");
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(50.0f, 1.0f, 100, 1000, 1.0f, -1, true, 5.0f);
				}
			}

            return true;
        }

        CWeapons::~CWeapons()
        {
            for (auto it = m_weapons.begin(); it != m_weapons.end(); ++it)
                delete (*it);
            
            m_weapons.clear();
        }

        void CWeapons::setPrimaryWeapon(Common::Data::Weapons_t weapon)
        {
            for (unsigned i = 0; i < m_weapons.size(); ++i) {
                if (weapon == m_weapons[i]->m_type)
                    m_primary = i;
            }
        }

        void CWeapons::setSecondaryWeapon(Common::Data::Weapons_t weapon)
        {
            for (unsigned i = 0; i < m_weapons.size(); ++i) {
                if (weapon == m_weapons[i]->m_type)
                    m_secondary = i;
            }
        }

        void CWeapons::tick(unsigned int msecs)
        {
            m_weapons[m_primary]->tick(msecs);
            m_weapons[m_secondary]->tick(msecs);
        }

        void CWeapons::shoot(int index)
        {
            Vector3 pos = m_trans->getPosition() + (m_shipRadius * Common::Util::Math::getDirection(m_trans->getTransform()));
            using namespace Common::Data;
            log_trace(LOG_PWEAPON, "Shoot with secondary weapon\n");
            switch (m_weapons[index]->m_type)
            {
            case LASER:
                m_weapons[index]->shoot(pos, Common::Util::Math::getDirection(m_trans->getTransform()));
                break;
            case LASER_BEAM:
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