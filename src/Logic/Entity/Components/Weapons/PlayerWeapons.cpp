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
#include "BombWeapon.h"
#include "../Movement/Transform.h"
#include <log.h>

#include <Common/Map/MapEntity.h>
#include <Logic/Scene/Scene.h>
#include <Common/Data/Spawn_Constants.h>
#include <Common/Data/Game_Constants.h>
#include <Application/Manager/GameManager.h>

#include <random>
#include <chrono>
#include <ctime>

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
			m_weapons.push_back(new CBombWeapon(entity,scene));

            // by default laser and missile
            m_primary   = 0;
            m_secondary = 1;

			if(entity->isPlayer()){

				std::string primaryWeapon(Application::CGameManager::getInstance()->getPlayerActiveEquipment().find(Common::Data::Game::GAME_PRIMARY_WEAPON)->second.first);
				std::string secondaryWeapon(Application::CGameManager::getInstance()->getPlayerActiveEquipment().find(Common::Data::Game::GAME_SECONDARY_WEAPON)->second.first);

				float damageProp(Application::CGameManager::getInstance()->getDamageProportion());

				if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[0][0]){
					setPrimaryWeapon(LASER); // Laser weapon
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(35.0f * damageProp, 1.0f, 25, 600, 1.0f, -1, Common::Util::Math::PI/4, "Laser1.wav");

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[1][0]){
					setPrimaryWeapon(LASER);
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(10.0f * damageProp, 1.0f, 5, 400, 1.0f, -1, Common::Util::Math::PI/4, "Laser2.wav");

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[2][0]){
					setPrimaryWeapon(LASER_BEAM);
					static_cast<CLaserBeam*>(m_weapons[m_primary])->setWeapon(4.0f * damageProp, 1.0f, 600, 1.0f, -1, 20, "TeslaBeam.wav");

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[3][0]){
					setPrimaryWeapon(LASER);
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(60.0f * damageProp, 1.0f, 40, 1000, 1.0f, -1, Common::Util::Math::PI/3, "Laser5.wav");

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[4][0]){
					setPrimaryWeapon(LASER_BEAM);
					static_cast<CLaserBeam*>(m_weapons[m_primary])->setWeapon(20.0f * damageProp, 1.0f, 300, 1.0f, -1, 30, "EnergyBeam.wav");

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[5][0]){
					setPrimaryWeapon(LASER);
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(35.0f * damageProp, 1.0f, 40, 600, 1.0f, -1, Common::Util::Math::PI/6, "Laser3.wav", true);

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[6][0]){
					setPrimaryWeapon(LASER_BEAM);
					static_cast<CLaserBeam*>(m_weapons[m_primary])->setWeapon(45.0f * damageProp, 1.0f, 1000, 1.0f, -1, 40, "GravityBeam.wav");

				}else if(primaryWeapon == Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[7][0]){
					setPrimaryWeapon(LASER);
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(50.0f * damageProp, 1.0f, 70, 1000, 1.0f, -1, Common::Util::Math::PI/6, "Laser4.wav", true, 5.0f);

				}


				//Secondary weapon
				if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[0][0]){
					setSecondaryWeapon(MISSILE_LINEAR);
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(80.0f * damageProp, 1.0f, 70, 1000, 0.6f, -1, "Missile.wav");
					/*setSecondaryWeapon(STATIC_BOMB);
					static_cast<CBombWeapon*>(m_weapons[m_secondary])->setWeapon(80.0f * damageProp, 50, 100, 3000, 3000, Common::Data::Weapons_t::STATIC_BOMB);*/

				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[1][0]){

					setSecondaryWeapon(MISSILE_LINEAR);
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(80.0f * damageProp, 1.0f, 40, 1000, -0.1f, -1, "Mine.wav");

					/*setSecondaryWeapon(STATIC_BOMB);
					static_cast<CBombWeapon*>(m_weapons[m_secondary])->setWeapon(80.0f * damageProp, 50, 100, 3000, 3000, Common::Data::Weapons_t::STATIC_BOMB);*/

				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[2][0]){

					setSecondaryWeapon(MISSILE_LINEAR);
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(100.0f * damageProp, 1.0f, 70, 1000, 0.0f, -1, "Mine.wav");

				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[3][0]){
					setSecondaryWeapon(MISSILE_LINEAR);
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(50.0f * damageProp, 1.0f, 50, 1000, 0.3f, -1, "TriMissile.wav", true, 20.0f, true);

				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[4][0]){
					setSecondaryWeapon(MISSILE_LINEAR);
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(60.0f * damageProp, 1.0f, 40, 1000, -0.1f, -1, "TriMines.wav", true, 20.0f, true);
				}else if(secondaryWeapon == Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[5][0]){
					setSecondaryWeapon(MISSILE_LINEAR);
					static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(50.0f * damageProp, 1.0f, 100, 1000, 1.0f, -1, "Missile.wav", true, 5.0f);
				}
			}else{

				// Pick a primary weapon for the enemy
				std::time_t seed(std::chrono::system_clock::now().time_since_epoch().count());
				std::default_random_engine generator(seed);

				std::uniform_int_distribution<int> coinToss(0,3);

				if(coinToss(generator) <= 2){
					setPrimaryWeapon(LASER); // Laser weapon
					static_cast<CLaserWeapon*>(m_weapons[m_primary])->setWeapon(20.0f, 1.0f, 25, 600, 1.0f, -1, Common::Util::Math::PI/4, "Laser1.wav");
				}else{
					setPrimaryWeapon(LASER_BEAM);
					static_cast<CLaserBeam*>(m_weapons[m_primary])->setWeapon(75.0f, 1.0f, 800, 1.0f, -1, 20, "EnergyBeam.wav");
				}

				setSecondaryWeapon(MISSILE_LINEAR);
				static_cast<CMissileWeapon_Linear*>(m_weapons[m_secondary])->setWeapon(40.0f, 1.0f, 40, 1200, 0.6f, -1, "Mine.wav");
			}

            return true;
        }

        CWeapons::~CWeapons()
        {
            for (auto it = m_weapons.begin(); it != m_weapons.end(); ++it)
                delete (*it);
            
            m_weapons.clear();
        }

		void CWeapons::deactivate()
		{
			IComponent::deactivate();

			m_weapons[m_primary]->releaseTrigger();
            m_weapons[m_secondary]->releaseTrigger();
		}

        void CWeapons::setPrimaryWeapon(Common::Data::Weapons_t weapon)
        {
            m_primary = getWeapon(weapon);
        }

        void CWeapons::setSecondaryWeapon(Common::Data::Weapons_t weapon)
        {
            m_secondary = getWeapon(weapon);
        }

        void CWeapons::tick(unsigned int msecs)
        {
            m_weapons[m_primary]->tick(msecs);
            m_weapons[m_secondary]->tick(msecs);
        }

        void CWeapons::shoot(int index, unsigned int msecs)
        {
            Vector3 pos = m_trans->getPosition() + (m_shipRadius * Common::Util::Math::getDirection(m_trans->getTransform()));
            using namespace Common::Data;
            log_trace(LOG_PWEAPON, "Shoot with secondary weapon\n");
            switch (m_weapons[index]->m_type)
            {
            case LASER:
                static_cast<CLaserWeapon*>(m_weapons[index])->shoot(pos, Common::Util::Math::getDirection(m_trans->getTransform()), m_targetPos);
                break;
            case LASER_BEAM:
                static_cast<CLaserBeam*>(m_weapons[index])->shoot(pos, Common::Util::Math::getDirection(m_trans->getTransform()), msecs);
                break;
            case MISSILE_LINEAR:
                pos = m_trans->getPosition() + (m_shipRadius * Common::Util::Math::getDirection(m_trans->getTransform()));
                m_weapons[index]->shoot(pos, Common::Util::Math::getDirection(m_trans->getTransform()));
                break;
			case STATIC_BOMB:
                pos = m_trans->getPosition();
				static_cast<CBombWeapon*>(m_weapons[index])->shoot(pos); 
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
			case LASER_BEAM:
				static_cast<CLaserBeam*>(m_weapons[weapon])->releaseTrigger(); break;
            }
        }

        unsigned CWeapons::getWeapon(Common::Data::Weapons_t weapon)
        {
            for (unsigned i = 0; i < m_weapons.size(); ++i)
                if (m_weapons[i]->m_type == weapon)
                    return i;
        }

		Common::Data::Weapons_t CWeapons::getPrimaryWeaponType() 
		{
			return m_weapons[m_primary]->m_type;
		}

		Common::Data::Weapons_t CWeapons::getSecondaryWeaponType() 
		{
			return m_weapons[m_secondary]->m_type;
		}
    }
}