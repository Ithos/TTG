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

#include "Shield.h"

#include "Logic/Entity/Entity.h"
#include "Common/Map/MapEntity.h"
#include "Common/Data/Spawn_Constants.h"
#include "Logic/EntityFactory.h"
#include "Logic/Scene/Scene.h"
#include "Logic/Entity/Entity.h"
#include "Application/Manager/GameManager.h"
#include "Common/Configure/Configure.h"

namespace Logic
{
	namespace Component
	{
        IMP_FACTORY(CShield)

        CShield::~CShield()
        {
            if (!m_player)
                delete m_value;
        }

        bool CShield::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            if (!IComponent::spawn(entity, scene, entityInfo))
		        return false;

            if (entity->isPlayer()) {
                m_player = true;
                m_value = Application::CGameManager::getInstance()->getShieldREF();
            }
            else {
                m_player = false;
                m_value = new unsigned int();
            }

            // get by config resistance value
            if (entityInfo->hasAttribute(Common::Data::Spawn::COMMON_SHIELD)) 
                *m_value = entityInfo->getFloatAttribute(Common::Data::Spawn::COMMON_SHIELD);

            if (entityInfo->hasAttribute(Common::Data::Spawn::COMMON_SHIELD_RESIS))
                m_resistance = entityInfo->getFloatAttribute(Common::Data::Spawn::COMMON_SHIELD_RESIS);

            Map::CMapEntity eInfo("Shield");

            using namespace Common::Data::Spawn;
		    using namespace Common::Configuration;
            //-- attributes...
            eInfo.setType(getDefaultValue(GEN_SHIELD_TYPE));
            eInfo.setAttribute(PHYSIC_ENTITY, getDefaultValue(GEN_SHIELD_TRIGGER_ENTITY));
            eInfo.setAttribute("physic_type", "kinematic");
            eInfo.setAttribute(PHYSIC_RADIUS, getDefaultValue(GEN_SHIELD_TRIGGER_RADIUS));
            eInfo.setAttribute(PHYSIC_TRIGGER, getDefaultValue(GEN_SHIELD_TRIGGER_ISTRIGGER));
            eInfo.setAttribute("physic_shape", "sphere");
            eInfo.setAttribute(GRAPHIC_MODEL, getDefaultValue(GEN_SHIELD_GRAPHIC_MODEL));
            eInfo.setAttribute("physic_mass", "1");

            m_subEntity = Logic::CEntityFactory::getInstance()->createEntity(&eInfo, nullptr);
            // spawn components
            m_subEntity->spawnEx(entity, scene, &eInfo);
            m_subEntity->activate();

         /*   ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
            m_particleSys = pManager->createParticleSystem("shield", "electroShield", scene->getSceneManager());
            scene->getSceneManager()->getSceneNode("Player_node")->attachObject(m_particleSys);
            m_particleSys->setScaleVelocity(10);
            m_particleSys->start();*/

            return true;
        }

        void CShield::tick(unsigned int msecs)
        {
           m_subEntity->tick(msecs);
        }

        /*
          factor could be a penetration or something like that
        */
        void CShield::decreaseShield(const unsigned& damage, const unsigned& factor)
        {
            // no shield active
            if (*m_value == 0) return;

            if (factor == 0 && (*m_value > damage)) {
                (*m_value) -= damage;
            }
            else if (*m_value > 0) {
                unsigned dmg = ((factor * m_resistance) * damage);
                if (*m_value > dmg)
                    (*m_value) -= dmg;
                else
                    *m_value = 0;
            }
            else
                *m_value = 0;
        }
    }
}