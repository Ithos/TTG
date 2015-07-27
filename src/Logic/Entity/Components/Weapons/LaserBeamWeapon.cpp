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

#include "LaserBeamWeapon.h"
#include "../Movement/Transform.h"
#include "../Graphic/Graphics.h"
#include "Application/Manager/GameManager.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <common/Particles/ParticleManager.h>

#include "Common/Map/MapEntity.h"
#include "Common/Data/Spawn_Constants.h"
#include "Common/data/TTG_Types.h"
#include "Common/Physic/PhysicManager.h"
#include "Common/Sound/Sound.h"

#include "Logic/Entity/Components/Gameplay/Shield.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Gameplay/Life.h"
#include "Logic/Scene/Scene.h"
#include "../Physic/IPhysic.h"

namespace Logic
{
	namespace Component
	{
        const char* const RIBBON_NAME = "ribbontrail_";

        CLaserBeam::CLaserBeam(CScene* scene, Ogre::SceneManager* sceneMngr, physx::PxScene* pxScene, const Map::CMapEntity* entityInfo, CEntity* player)
            : IWeapon(), m_scene(scene), m_sceneMngr(sceneMngr), m_pxScene(pxScene), m_player(player), m_cost(0), m_energy(nullptr), m_interval(1), m_auxInterval(0),
			m_time(0.0f),m_soundName(""), m_soundExplosion("beamExplosion"), m_shooting(false), m_acumCost(0.0f), m_acumDamage(0.0f)
        { 

			static unsigned int num(0);

             m_type = LASER_BEAM;

             using namespace Common::Data::Spawn;

             if (entityInfo->hasAttribute(LASERBEAM_DAMAGE))
                 m_damage = entityInfo->getFloatAttribute(LASERBEAM_DAMAGE);

             if (entityInfo->hasAttribute(LASERBEAM_RANGE))
                 m_range = (unsigned) entityInfo->getIntAttribute(LASERBEAM_RANGE);

             if (entityInfo->hasAttribute(LASERBEAM_COST))
                 m_cost = entityInfo->getIntAttribute(LASERBEAM_COST);

             if (entityInfo->hasAttribute(LASERBEAM_INTERVAL))
                 m_interval = entityInfo->getIntAttribute(LASERBEAM_INTERVAL);

             if (m_player->isPlayer()) 
                 m_energy = &Application::CGameManager::getInstance()->m_curEnergy;

             // Particles code ----
             m_particles = Common::Particles::CParticleManager::getInstance();
             m_particles->addShootType(LASER);

             m_phyMngr = Common::Physic::CPhysicManager::getInstance();

			 //Sounds
			 if(player->isPlayer())
				 m_soundName = "defaultPlayerBeamSound";
			 else{
				 m_soundName = "defaultEnemyBeamSound" + std::to_string(num);
				 m_soundExplosion += std::to_string(num);
				 ++num;
			 }

			Common::Sound::CSound::getSingletonPtr()->add3dSound("EnergyBeam.wav", m_soundName);

			 Common::Sound::CSound::getSingletonPtr()->add3dSound("Explosion.wav", m_soundExplosion);
        }

        CLaserBeam::~CLaserBeam()
        {

			Common::Sound::CSound::getSingletonPtr()->release3dSound(m_soundName);

			Common::Sound::CSound::getSingletonPtr()->release3dSound(m_soundExplosion);
        }

        void CLaserBeam::shoot(const Vector3& src, const Vector3& dir, unsigned int msecs)
        {
            /*if (m_auxInterval < m_interval) {
                ++m_auxInterval;
                return;
            }
            
            m_auxInterval = 0;*/

            using namespace Common::Data;
            using namespace Logic::Component;

            if (m_player->isPlayer()) {
				if (*m_energy > m_acumCost) {

					m_acumCost += m_cost * msecs* 0.001f;
					int tmpCost = m_acumCost;

					if(tmpCost >= 1.0f){
						unsigned aux = *m_energy - 1;
						if (aux >= 0)
							*m_energy = aux;
						else
							*m_energy = 0;

						m_acumCost -= tmpCost;
					}
                }
                else{
					releaseTrigger();
                    return; // if dont have energy, cannot shoot
				}

            }

			if(!m_shooting){
				Common::Sound::CSound::getSingletonPtr()->play3dSound(m_soundName,
						static_cast<CTransform*>(m_player->getComponentByName(Common::Data::TRANSFORM_COMP))->getTransform());

				m_shooting = true;
			}

            CEntity* hitEntity = nullptr;
            m_ray.setOrigin(src);
            m_ray.setDirection(dir);

            hitEntity = m_phyMngr->raycastClosest(m_ray, m_range, PGROUPS::DAMAGEABLE); // DEFAULT group                                           

            if (hitEntity) {
                std::string type = hitEntity->getType();
				if(m_player->isPlayer()){
					if (type == "Asteroid" || type == "Enemy") {
						m_currPos = static_cast<CTransform*>(hitEntity->getComponentByName(TRANSFORM_COMP))->getPosition();
						float distance = src.distance(m_currPos);
						m_particles->laserShot(src, dir, distance, LASER_RED);
                        
						m_acumDamage += m_damage * msecs * 0.001;
						int tmpDamage = (int)m_acumDamage;
						if (tmpDamage >= 1.0f && static_cast<CLife*>(hitEntity->getComponentByName(LIFE_COMP))->decreaseLife(tmpDamage)) {
							m_acumDamage -= tmpDamage;
							Common::Sound::CSound::getSingletonPtr()->play3dSound(m_soundExplosion,
						static_cast<CTransform*>(hitEntity->getComponentByName(Common::Data::TRANSFORM_COMP))->getTransform());
								m_scene->deactivateEntity(hitEntity);
								m_scene->deleteEntity(hitEntity);
								m_particles->startNextExplosion(m_currPos);
							}
							else {
								m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));
							}
					} // hit asteroid or enemy
					else {
						m_particles->laserShot(src - (81 * dir), dir, m_range, LASER_GREEN);
					}
				}else{


					if (type == "Player" || type == "Asteroid") {
						m_currPos = static_cast<CTransform*>(hitEntity->getComponentByName(TRANSFORM_COMP))->getPosition();
						float distance = src.distance(m_currPos);
						m_particles->laserShot(src, dir, distance, LASER_RED);

					m_acumDamage += m_damage * msecs * 0.001;
					int tmpDamage = (int)m_acumDamage;

					if(hitEntity->isPlayer() && static_cast<CShield*>(hitEntity->getComponentByName(SHIELD_COMP))->hasShield() && tmpDamage >= 1){

							static_cast<CShield*>(hitEntity->getComponentByName(SHIELD_COMP))->decreaseShield(tmpDamage);
							m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));

							m_acumDamage -= tmpDamage;

						}else if (tmpDamage >= 1 && static_cast<CLife*>(hitEntity->getComponentByName(LIFE_COMP))->decreaseLife((int)(tmpDamage/10.0f))) {
							Common::Sound::CSound::getSingletonPtr()->play3dSound(m_soundExplosion,
								static_cast<CTransform*>(hitEntity->getComponentByName(Common::Data::TRANSFORM_COMP))->getTransform());
							m_scene->deactivateEntity(hitEntity);
							m_scene->deleteEntity(hitEntity);
							m_particles->startNextExplosion(m_currPos);
							m_acumDamage -= tmpDamage;
						}
						else {
							m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));
						}
					} // hit asteroid or enemy
					else {
						m_particles->laserShot(src - (81 * dir), dir, m_range, LASER_GREEN);
					}




				}
            }
            else { //no hit
                m_particles->laserShot(src, dir, m_range, LASER_GREEN);
            }

        }

		void CLaserBeam::tick(unsigned int msecs)
		{
			IWeapon::tick(msecs);

			if(m_time >= 0.3f)
			{
				m_time = 0.0f;
				
				if(m_shooting){
					Common::Sound::CSound::getSingletonPtr()->update3dSound(m_soundName, 
					static_cast<CTransform*>(m_player->getComponentByName(Common::Data::TRANSFORM_COMP))->getTransform());
				}
			}else{
				m_time += msecs/1000.0f;
			}

		}

        void CLaserBeam::setWeapon(const float& damage, const float& cadence, const float& range,
			const float& speed, int charger, int cost, const std::string& soundFile, Common::Data::Weapons_t type)
        {
            m_damage  = damage;
            m_cadence = cadence;
            m_range   = range;
            m_speed   = speed;
            m_maxCharger = charger;
			m_cost = cost;
            if (type != END)
                m_type = type;

			Common::Sound::CSound::getSingletonPtr()->release3dSound(m_soundName);
			Common::Sound::CSound::getSingletonPtr()->add3dSound(soundFile, m_soundName, true);
        }

		void CLaserBeam::releaseTrigger()
		{
			m_shooting = false;

			Common::Sound::CSound::getSingletonPtr()->stop3dSound(m_soundName);
		}

    } //Component
}