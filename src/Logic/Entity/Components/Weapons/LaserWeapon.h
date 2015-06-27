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

#ifndef __LOGIC_LASERWEAPON_H
#define __LOGIC_LASERWEAPON_H

#include "Weapon.h"
#include <Common/Util/Math.h>
#include <Logic/Entity/Entity.h>

namespace Ogre
{
	class Entity;
	class SceneManager;
	class SceneNode;
}

namespace Map
{
    class CMapEntity;
}

namespace physx
{
    class PxScene;
}

namespace Common
{
    namespace Particles
    {
        class CParticleManager;
    }

    namespace Physic
    {
        class CPhysicManager;
    }
}

using namespace Common::Particles;
using namespace Common::Physic;

namespace Logic
{
    class CScene;

	namespace Component
	{
        class CLaserWeapon : public IWeapon
        {
        public:
            CLaserWeapon(CScene*, Ogre::SceneManager*, physx::PxScene*, const Map::CMapEntity*, CEntity*);
            ~CLaserWeapon();

            void shoot(const Vector3& src, const Vector3& dir);

			virtual void tick(unsigned int) override;

            /**
                Set parameters for a weapon.
            */
			void setWeapon( const float& damage, const float& cadence, const unsigned int& cost, const float& range, const float& speed, 
                int charger, const std::string& soundFile, bool triple = false, float beamDist=20.0f, Common::Data::Weapons_t type = Common::Data::Weapons_t::END);

        private:
		   Ogre::SceneManager*  m_sceneMngr;
           physx::PxScene*      m_pxScene;
           CScene*              m_scene;
           Vector3              m_currPos;
           CParticleManager*    m_particles;
           CPhysicManager*      m_phyMngr;
           Ogre::Ray            m_ray;
           CEntity*             m_player;
           unsigned             m_cost;
           unsigned*            m_energy;
		   bool					m_triple;
		   float				m_beamDist;
		   std::string			m_soundName;
		   std::string			m_soundExplosion;
		   float				m_time;
        };
    }
}

#endif