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
#include "../Movement/Transform.h"
#include "Common/Map/MapEntity.h"
#include "Common/Configure/Configure.h"
#include "Common/Data/Spawn_Constants.h"
#include "Logic/EntityFactory.h"
#include "Common/Sound/Sound.h"

#include <Application/Manager/GameManager.h>

using namespace Common::Data;

namespace Logic
{
	namespace Component
	{
        const int MAX_MISSILES = 20;
		const unsigned BASIC_MISSILE_COST = 50;

		CMissileWeapon_Linear::CMissileWeapon_Linear(CEntity* parent, CScene* scene) : m_iMissile(0),
			m_triple(false), m_beamDist(20.0f), m_rotate(false), m_soundName("") 
        {
			static unsigned int num(0);

            m_type = MISSILE_LINEAR;

            using namespace Common::Data::Spawn;
		    using namespace Common::Configuration;

			setDefaultFile(CONFIGURE_FILE);
			setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());

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

			m_range = Common::Configuration::defaultValue<float>(GEN_MISSILE_LINEAR_RANGE);

            m_maxCharger = atoi(getDefaultValue(GEN_MISSILE_LINEAR_MAXCHARGER).c_str());
            m_ammo       = atoi(getDefaultValue(GEN_MISSILE_LINEAR_AMMO).c_str());

            if (m_ammo > m_maxCharger)
                m_ammo = m_maxCharger;

			m_cost = BASIC_MISSILE_COST;

			if(m_parent->isPlayer())
				m_soundName = "defaultPlayerMissileSound";
			else{
				m_soundName = "defaultMissileSound" + std::to_string(num);
				++num;
			}

        	Common::Sound::CSound::getSingletonPtr()->add3dSound("Missile.wav", m_soundName);
        }

        CMissileWeapon_Linear::~CMissileWeapon_Linear()
        {
            for (unsigned i = 0; i < m_mapInfo.size(); ++i) {
                if (m_mapInfo[i]) {
                    Map::CMapEntity* me = m_mapInfo[i];
                    delete me;
                    m_mapInfo[i] = nullptr;
                }
            }

            while (!m_subEntity.empty()) {
                CEntityFactory::getInstance()->deleteEntityEx(m_subEntity[m_subEntity.size()-1]);
                m_subEntity.pop_back();
            }

			m_cost = BASIC_MISSILE_COST;

			Common::Sound::CSound::getSingletonPtr()->release3dSound(m_soundName);
        }

        void CMissileWeapon_Linear::tick(unsigned int msecs)
        {
            for (auto it = m_subEntity.begin(); it != m_subEntity.end(); ++it)
                (*it)->tick(msecs);
        }

        void CMissileWeapon_Linear::reload(int ammount)
        {
            m_ammo += ammount;
            if (m_ammo > m_maxCharger)
                m_ammo = m_maxCharger;
        }

        void CMissileWeapon_Linear::shoot(const Vector3& src, const Vector3& dir)
        {
            if (!m_trigger || !m_parent->isPlayer()) {
				if (m_parent->isPlayer()) {
					if (Application::CGameManager::getInstance()->getEnergyState() >= m_cost) {//m_ammo != 0
						m_trigger = true;
						Vector3 tmpSrc(src);
						if(m_speed <= 0)tmpSrc -= dir * 40.0;
						m_subEntity[m_iMissile]->spawnEx(m_parent, m_scene, m_mapInfo[m_iMissile]);
						m_subEntity[m_iMissile]->activate();
						static_cast<CMissileTrigger*>(m_subEntity[m_iMissile]->getComponentByName(MISSILE_TRIGGER))->shoot(tmpSrc, dir);

						if (m_iMissile < MAX_MISSILES-1 )
							++m_iMissile;
						else
							m_iMissile = 0;

						/*--m_ammo;*/
						Application::CGameManager::getInstance()->decreaseEnergyState(m_cost);

						Common::Sound::CSound::getSingletonPtr()->play3dSound(m_soundName,
					static_cast<CTransform*>(m_parent->getComponentByName(Common::Data::TRANSFORM_COMP))->getTransform());

						if(m_triple){

							tmpSrc += dir.normalisedCopy().crossProduct(Vector3(0.0f, 1.0, 0.0)) * m_beamDist;

							Vector3 tmpDir(dir);

							if(m_rotate){
								if(m_speed <= 0)
									tmpDir = dir.normalisedCopy() * 0.5 - dir.normalisedCopy().crossProduct(Vector3(0.0f, 1.0, 0.0)) * 0.5;
								else
									tmpDir = dir.normalisedCopy() * 0.5 + dir.normalisedCopy().crossProduct(Vector3(0.0f, 1.0, 0.0)) * 0.5;
							}

							m_subEntity[m_iMissile]->spawnEx(m_parent, m_scene, m_mapInfo[m_iMissile]);
							m_subEntity[m_iMissile]->activate();
							static_cast<CMissileTrigger*>(m_subEntity[m_iMissile]->getComponentByName(MISSILE_TRIGGER))->shoot(tmpSrc, tmpDir);

							if (m_iMissile < MAX_MISSILES-1 )
								++m_iMissile;
							else
								m_iMissile = 0;

							tmpSrc = src;
							if(m_speed <= 0)tmpSrc -= dir * 40.0;
							tmpSrc -= dir.normalisedCopy().crossProduct(Vector3(0.0f, 1.0, 0.0)) * m_beamDist;

							if(m_rotate){
								if(m_speed <= 0)
									tmpDir = dir.normalisedCopy() * 0.5 + dir.normalisedCopy().crossProduct(Vector3(0.0f, 1.0, 0.0)) * 0.5;
								else
									tmpDir = dir.normalisedCopy() * 0.5 - dir.normalisedCopy().crossProduct(Vector3(0.0f, 1.0, 0.0)) * 0.5;
							}

							m_subEntity[m_iMissile]->spawnEx(m_parent, m_scene, m_mapInfo[m_iMissile]);
							m_subEntity[m_iMissile]->activate();
							static_cast<CMissileTrigger*>(m_subEntity[m_iMissile]->getComponentByName(MISSILE_TRIGGER))->shoot(tmpSrc, tmpDir);

							if (m_iMissile < MAX_MISSILES-1 )
								++m_iMissile;
							else
								m_iMissile = 0;

						}
					}
					else {
						// sound empty weapon for example
					}
				}
                else {
					m_subEntity[m_iMissile]->spawnEx(m_parent, m_scene, m_mapInfo[m_iMissile]);
					m_subEntity[m_iMissile]->activate();
                	static_cast<CMissileTrigger*>(m_subEntity[m_iMissile]->getComponentByName(MISSILE_TRIGGER))->shoot(src, dir);

					Common::Sound::CSound::getSingletonPtr()->play3dSound(m_soundName,
					static_cast<CTransform*>(m_parent->getComponentByName(Common::Data::TRANSFORM_COMP))->getTransform());

                    if (m_iMissile < MAX_MISSILES-1 )
						++m_iMissile;
					else
						m_iMissile = 0;
				}
            }
        }

		void CMissileWeapon_Linear::setWeapon( const float& damage, const float& cadence, const unsigned int& cost, const float& range, const float& speed, 
                int charger, const std::string& soundFile, bool triple, float beamDist, bool rotate, Common::Data::Weapons_t type)
		{
			m_damage  = damage;
            m_cadence = cadence;
			m_cost	  = cost;
            m_range   = range;
            m_speed   = speed;
            m_maxCharger = charger;
			m_triple  = triple;
			m_beamDist = beamDist;
			m_rotate = rotate;
            if (type != END)
                m_type = type;

            while (!m_subEntity.empty()) {
                CEntityFactory::getInstance()->deleteEntityEx(m_subEntity[m_subEntity.size()-1]);
                m_subEntity.pop_back();
            }

			using namespace Common::Data::Spawn;
		    using namespace Common::Configuration;

			setDefaultFile(CONFIGURE_FILE);
			setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());

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
				m_mapInfo[i]->setAttribute(MISSILE_SPEED,  std::to_string(m_speed));
                m_mapInfo[i]->setAttribute(MISSILE_DAMAGE, std::to_string(m_damage));
                m_mapInfo[i]->setAttribute(MISSILE_RANGE,  std::to_string(m_range));
                
                CEntity* ent = CEntityFactory::getInstance()->createEntity(m_mapInfo[i], nullptr);
                m_subEntity.push_back(ent);
                static_cast<CMissileTrigger*>(ent->getComponentByName(MISSILE_TRIGGER))->setMove(); // no funtion->linear movement
            }

			Common::Sound::CSound::getSingletonPtr()->release3dSound(m_soundName);
			Common::Sound::CSound::getSingletonPtr()->add3dSound(soundFile, m_soundName);
		}

    }
}