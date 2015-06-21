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

#include "LaserWeapon.h"
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

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Gameplay/Life.h"
#include "Logic/Scene/Scene.h"

#include "log.h"

#include "../Physic/IPhysic.h"

namespace Logic
{
	namespace Component
	{
        const char* const RIBBON_NAME = "ribbontrail_";

        CLaserWeapon::CLaserWeapon(CScene* scene, Ogre::SceneManager* sceneMngr, physx::PxScene* pxScene, const Map::CMapEntity* entityInfo, CEntity* player)
            : IWeapon(), m_scene(scene), m_sceneMngr(sceneMngr), m_pxScene(pxScene), m_player(player), m_cost(0), m_energy(nullptr)
        { 
             m_type = LASER;

             using namespace Common::Data::Spawn;

             if (entityInfo->hasAttribute(LASER_DAMAGE))
                 m_damage = entityInfo->getFloatAttribute(LASER_DAMAGE);

             if (entityInfo->hasAttribute(LASER_RANGE))
                 m_range = (unsigned) entityInfo->getIntAttribute(LASER_RANGE);

             if (entityInfo->hasAttribute(LASER_COST))
                 m_cost = entityInfo->getIntAttribute(LASER_COST);

             if (m_player->isPlayer()) {
                 m_energy = &Application::CGameManager::getInstance()->m_curEnergy;
             }

             // Particles code ----
             m_particles = Common::Particles::CParticleManager::getInstance();
             m_particles->addShootType(LASER);

             m_phyMngr = Common::Physic::CPhysicManager::getInstance();
        }

        CLaserWeapon::~CLaserWeapon()
        {
        }

#pragma warning (push)
#pragma warning (disable:4244)
        void CLaserWeapon::shoot(const ::Vector3& src, const ::Vector3& dir)
        {
            if (m_trigger)
                return;
            else
                m_trigger = true;

            using namespace Common::Data;
            using namespace Logic::Component;
			
       //     m_particles->startShoot(LASER, src, dir, 2);

            if (m_player->isPlayer()) {
                if (*m_energy > m_cost) {
                    unsigned aux = *m_energy - m_cost;
                    if (aux >= 0)
                        *m_energy = aux;
                    else
                        *m_energy = 0;
                }
                else
                    return; // if dont have energy, cannot shoot
            }

            CEntity* hitEntity = nullptr;
            m_ray.setOrigin(src);
            m_ray.setDirection(dir);

            hitEntity = m_phyMngr->raycastClosest(m_ray, m_range, PGROUPS::DAMAGEABLE); // DEFAULT group                       

            if (hitEntity) {
                std::string type = hitEntity->getType();
                if (type == "Asteroid" || type == "Enemy") {
                    m_currPos = static_cast<CTransform*>(hitEntity->getComponentByName(TRANSFORM_COMP))->getPosition();
                    float distance = src.distance(m_currPos);
                    m_particles->laserShot(src - (81 * dir), dir, distance);
                        

					if (static_cast<CLife*>(hitEntity->getComponentByName(LIFE_COMP))->decreaseLife(m_damage)) {
							m_scene->deactivateEntity(hitEntity);
							m_scene->deleteEntity(hitEntity);
                            m_particles->startNextExplosion(m_currPos);
                        }
                        else {
                            m_particles->startHit(m_currPos + (-dir * (((CGraphics*)(hitEntity->getComponentByName(GRAPHICS_COMP)))->getScale() >= 30.0 ? 20 : 0) ));
                        }
                   /* }*/
                } // hit asteroid or enemy
                else {
                    m_particles->laserShot(src - (81 * dir), dir, m_range);
                }
            }
            else { //no hit
                  m_particles->laserShot(src - (81 * dir), dir, m_range);
            }

        } // shoot
#pragma warning (pop)

        void CLaserWeapon::setWeapon(const float& damage, const float& cadence, const float& range, const float& speed, int charger, Common::Data::Weapons_t type)
        {
            m_damage  = damage;
            m_cadence = cadence;
            m_range   = range;
            m_speed   = speed;
            m_maxCharger = charger;
            if (type != END)
                m_type = type;
        }

    } // Component
} 