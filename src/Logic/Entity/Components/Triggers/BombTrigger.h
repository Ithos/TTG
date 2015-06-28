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

#ifndef __LOGIC_BOMBTRIGGER_H
#define __LOGIC_BOMBTRIGGER_H

#include "../Physic/PhysicEntity.h"
#include <vector>

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
	class CScene;

	namespace Component
	{
        class CBombTrigger : public CPhysicEntity
        {
			DEC_FACTORY(CBombTrigger);
		public:
			CBombTrigger():m_shooted(false),m_explode(false),m_damage(0),m_particles(nullptr),m_scene(nullptr){}

			~CBombTrigger();

			bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

			bool activate();

			void tick(unsigned int);

            void onOverlapBegin(IPhysic* otherComponent);

			void onOverlapEnd(IPhysic* otherComponent);

			void setPosition(const Vector3& pos);

			bool m_explode;

		private:
			std::vector<CEntity*> m_entitiesOnRange;

			unsigned int m_damage;
			bool m_shooted;

			CParticleManager*          m_particles;
            CScene*                    m_scene;

        };
		REG_FACTORY(CBombTrigger);

    }
}

#endif