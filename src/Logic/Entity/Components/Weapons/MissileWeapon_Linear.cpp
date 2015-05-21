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

#include "common/Particles/ParticleManager.h"
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

             Map::CMapEntity eInfo("Missile_Linear");

            using namespace Common::Data::Spawn;
		    using namespace Common::Configuration;

             //-- attributes...
            eInfo.setType(getDefaultValue(GEN_MISSILE_LINEAR_TYPE));
            eInfo.setAttribute(PHYSIC_ENTITY,  getDefaultValue(GEN_MISSILE_LINEAR_TRIGGER_ENTITY));
            eInfo.setAttribute("physic_type", "kinematic");
            eInfo.setAttribute("physic_shape", "sphere");
            eInfo.setAttribute("physic_mass", "1");
            eInfo.setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_MISSILE_LINEAR_TRIGGER_RADIUS));
            eInfo.setAttribute(PHYSIC_TRIGGER, getDefaultValue(GEN_MISSILE_LINEAR_TRIGGER_ISTRIGGER));
            eInfo.setAttribute(MISSILE_SPEED,  getDefaultValue(GEN_MISSILE_LINEAR_SPEED));
            eInfo.setAttribute(MISSILE_DAMAGE, getDefaultValue(GEN_MISSILE_LINEAR_DAMAGE));
            eInfo.setAttribute(MISSILE_RANGE,  getDefaultValue(GEN_MISSILE_LINEAR_RANGE));

            //init pool of missiles (
            for ( int i = 0; i < MAX_MISSILES; ++i) {
                CEntity* ent = CEntityFactory::getInstance()->createEntity(&eInfo, nullptr);
                ent->spawnEx(parent, scene, &eInfo);
                ent->activate();
                m_subEntity.push_back(ent);
                static_cast<CMissileTrigger*>(ent->getComponentByName(MISSILE_TRIGGER))->setMove(); // no funtion->linear movement
            }

            // Particles code ----
             m_particles = Common::Particles::CParticleManager::getInstance();
             m_particles->addShootType(MISSILE_LINEAR);
        }

        void CMissileWeapon_Linear::tick(unsigned int msecs)
        {
            for (auto it = m_subEntity.begin(); it != m_subEntity.end(); ++it)
                (*it)->tick(msecs);
        }

        void CMissileWeapon_Linear::shoot(const Vector3& src, const Vector3& dir)
        {
            // todo: gestion el pool de entidades.
            static_cast<CMissileTrigger*>(m_subEntity[m_iMissile]->getComponentByName(MISSILE_TRIGGER))->shoot(src, dir);
            if (m_iMissile < MAX_MISSILES )
                m_iMissile = 0;
            else
                ++m_iMissile;
        }

    }
}