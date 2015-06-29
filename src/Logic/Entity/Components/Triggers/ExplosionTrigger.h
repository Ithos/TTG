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

#ifndef __LOGIC_EXPLOSIONTRIGGER_H
#define __LOGIC_EXPLOSIONTRIGGER_H

#include "../Physic/PhysicEntity.h"

namespace Common
{
    namespace Particles
    {
        class CParticleManager;
    }
}

using namespace Common::Particles;

namespace Logic
{
	namespace Component
	{
        class CExplosionTrigger : public CPhysicEntity
        {
            DEC_FACTORY(CExplosionTrigger)
        public:
            CExplosionTrigger() :
                m_shooted(false), m_pos(Vector3(.0f)), m_parent(nullptr), m_stillActive(false),
                m_particles(nullptr), m_damage(.0f), m_range(.0f), m_isPlayer(false),
				m_time(0), m_acumT(0),m_delay(0)
            {}

			~CExplosionTrigger(){}

            bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

            bool activate();

			void tick(unsigned int);

            void onOverlapBegin(IPhysic* otherComponent);

			void onOverlapEnd(IPhysic* otherComponent);

            void shoot(const Vector3&);

			void setPosition(const Vector3& pos);

        private:
            bool                m_shooted;
            Vector3             m_pos;
            bool                m_stillActive;
            CEntity*            m_parent;
            CParticleManager*   m_particles;
            float               m_damage;
            float               m_range;
            bool                m_isPlayer;
			unsigned int        m_time;
			int					m_delay;
			unsigned int		m_acumT;
        };

        REG_FACTORY(CExplosionTrigger)
    }
}

#endif