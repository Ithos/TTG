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
#include <Common/Particles/ParticleManager.h>
#include "Logic/EntityFactory.h"

#include "../Triggers/BombTrigger.h"
#include "../Triggers/ExplosionTrigger.h"
#include "../Movement/Transform.h"
#include "../../../ComponentFactory.h"

#include <Application/Manager/GameManager.h>

namespace Logic
{
    namespace Component
    {
        const int MAX_BOMS = 20; 

        CBombWeapon::CBombWeapon(CEntity* parent, CScene* scene)
            : m_parent(nullptr), m_scene(nullptr), m_cost(0), m_timeToExplode(.0f)
			,m_currBomb(0),m_shootDelay(2000),m_canShoot(true)
        {
            using namespace Common::Data::Spawn;
		    using namespace Common::Configuration;

			m_type = Common::Data::Weapons_t::STATIC_BOMB;
            m_parent = parent;
            m_scene  = scene;
			m_particles = Common::Particles::CParticleManager::getInstance();
			/*
			setDefaultFile(CONFIGURE_FILE);
			setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());

            //init pool of missiles (
            for ( int i = 0; i < MAX_BOMS; ++i) {
                m_mapInfo[i] = new Map::CMapEntity("Static_bomb" + std::to_string(i));
                m_mapInfo[i]->setType(getDefaultValue(GEN_STATICBOMB_TYPE));
                m_mapInfo[i]->setAttribute(PHYSIC_ENTITY,  getDefaultValue(GEN_STATICBOMB_TRIGGER_ENTITY));
                m_mapInfo[i]->setAttribute("physic_type", "dynamic");
                m_mapInfo[i]->setAttribute("physic_shape", "sphere");
                m_mapInfo[i]->setAttribute("physic_mass",  "1");
                m_mapInfo[i]->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_STATICBOMB_TRIGGER_RADIUS));
                m_mapInfo[i]->setAttribute(PHYSIC_TRIGGER, getDefaultValue(GEN_STATICBOMB_TRIGGER_ISTRIGGER));
                m_mapInfo[i]->setAttribute(BOMB_DAMAGE, getDefaultValue(GEN_STATICBOMB_DAMAGE));
                m_mapInfo[i]->setAttribute(BOMB_COST,  getDefaultValue(GEN_STATICBOMB_COST));
                m_mapInfo[i]->setAttribute(BOMB_RANGE, getDefaultValue(GEN_STATICBOMB_RANGE));
				/*
				Map::CMapEntity* entinf = new Map::CMapEntity("..");
				entinf->setAttribute(PHYSIC_ENTITY,  getDefaultValue(GEN_STATICBOMB_TRIGGER_ENTITY));
                entinf->setAttribute("physic_type", "dynamic");
                entinf->setAttribute("physic_shape", "sphere");
                entinf->setAttribute("physic_mass",  "1");
                entinf->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_STATICBOMB_RANGE));
                entinf->setAttribute(PHYSIC_TRIGGER, getDefaultValue(GEN_STATICBOMB_TRIGGER_ISTRIGGER));

				IComponent* bombTrigger = Logic::CComponentFactory::getInstance()->create("CBombTrigger");
				bombTrigger->setPosition(2); bombTrigger->setPriority(1);
                
                CEntity* ent = CEntityFactory::getInstance()->createEntity(m_mapInfo[i], nullptr);
				//bombTrigger->spawn(ent,m_scene,entinf);//FIXME
				
				//delete entinf;

				//ent->createComponent(bombTrigger);
                m_subEntity.push_back(ent);
            }
			*/
        }
        
        CBombWeapon::~CBombWeapon()
        {
			for(unsigned int i = 0; i < m_mapInfo.size(); ++i){
				if(Map::CMapEntity* aux = m_mapInfo[i]){
					delete aux;
					m_mapInfo[i] = nullptr;
				}
			}
        }

        void CBombWeapon::shoot(const Vector3& src)
        {
			if(!m_canShoot) return;

			if(m_parent->isPlayer()){
				if(Application::CGameManager::getInstance()->getEnergy() < m_cost) return;
				Application::CGameManager::getInstance()->decreaseEnergyState(m_cost);
			}

			if(m_currBomb < MAX_BOMS - 1){
				m_currBomb++;
			} else {
				m_currBomb = 0;
			}
			//m_subEntity[m_currBomb]->spawnEx(m_parent,m_scene,m_mapInfo[m_currBomb]);
			m_subEntity[m_currBomb]->spawn(m_scene,m_mapInfo[m_currBomb]);
			m_subEntity[m_currBomb]->activate();
			static_cast<CTransform*>(m_subEntity[m_currBomb]->getComponentByName("CTransform"))->setPosition(src);
			static_cast<CBombTrigger*>(m_subEntity[m_currBomb]->getComponentByName("CBombTrigger"))->setPosition(src,m_parent);
			static_cast<CExplosionTrigger*>(m_subEntity[m_currBomb]->getComponentByName("CExplosionTrigger"))->setPosition(src,m_parent);

			m_canShoot = false;
			m_shootDelay = 2000;

			//FIXME
			//m_particles->startBombEffect(src);
        }

        void CBombWeapon::tick(unsigned int msecs)
        {
			m_canShoot = ((m_shootDelay-=msecs) <= 0);
			for(auto it = m_subEntity.begin(); it != m_subEntity.end(); ++it){
				(*it)->tick(msecs);
			}
		}

        /**
            Set parameters for a weapon.
        */
		void CBombWeapon::setWeapon(const float& damage, const unsigned int& cost, const float& range, const unsigned int& time, 
			const unsigned int& delay, Common::Data::Weapons_t type)
        {
			using namespace Common::Data::Spawn;
		    using namespace Common::Configuration;

			m_subEntity.clear();

			m_type = type;
			m_cost = cost;
			m_damage = damage;
			m_timeToExplode = time;
			m_delay = delay;

			setDefaultFile(CONFIGURE_FILE);
			setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());

            for ( int i = 0; i < MAX_BOMS; ++i) {
                m_mapInfo[i] = new Map::CMapEntity("Static_bomb" + std::to_string(i));
                m_mapInfo[i]->setType(getDefaultValue(GEN_STATICBOMB_TYPE));
                m_mapInfo[i]->setAttribute(PHYSIC_ENTITY,  getDefaultValue(GEN_STATICBOMB_TRIGGER_ENTITY));
				m_mapInfo[i]->setAttribute(COMMON_POSITION,"{0.0, -300.0, 4700.0}");
                m_mapInfo[i]->setAttribute("physic_type", "dynamic");
                m_mapInfo[i]->setAttribute("physic_shape", "sphere");
                m_mapInfo[i]->setAttribute("physic_mass",  "1");
                m_mapInfo[i]->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_STATICBOMB_TRIGGER_RADIUS));
                m_mapInfo[i]->setAttribute(PHYSIC_TRIGGER, getDefaultValue(GEN_STATICBOMB_TRIGGER_ISTRIGGER));
				m_mapInfo[i]->setAttribute(BOMB_DAMAGE, std::to_string(damage));
                m_mapInfo[i]->setAttribute(BOMB_COST,  std::to_string(cost));
				m_mapInfo[i]->setAttribute(BOMB_RANGE, std::to_string(range));
				m_mapInfo[i]->setAttribute("bomb_time", std::to_string(time));
				m_mapInfo[i]->setAttribute("bomd_delay", std::to_string(delay));
				m_mapInfo[i]->setAttribute(BOMB_FF, getDefaultValue(GEN_STATICBOMB_FF));
				
                CEntity* ent = CEntityFactory::getInstance()->createEntity(m_mapInfo[i], nullptr);
				
                m_subEntity.push_back(ent);
			}
        }

    }
}