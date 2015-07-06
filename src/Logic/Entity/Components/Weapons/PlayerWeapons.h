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

#ifndef __LOGIC_PLAYERWEAPONS_H
#define __LOGIC_PLAYERWEAPONS_H

#include "..\Component.h"
#include <vector>
#include <Common/data/TTG_Types.h>
#include <common/Util/Math.h>

namespace Logic
{
	namespace Component
	{
        struct IWeapon;
        class CTransform;

		class CWeapons : public IComponent
		{
			DEC_FACTORY(CWeapons);
		public:
            CWeapons() : IComponent() { }
            ~CWeapons();

            bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

            void tick(unsigned int);

            const int& getNumPrimaryWeapon()   const { return m_primary;   }
            const int& getNumSecondaryWeapon() const { return m_secondary; }

            void setPrimaryWeapon(Common::Data::Weapons_t weapon);
            void setSecondaryWeapon(Common::Data::Weapons_t weapon);

            void shootPrimaryWeapon(unsigned int msecs)   { shoot(m_primary, msecs);   }
            void shootSecondaryWeapon() { shoot(m_secondary, 0); }

            void releasePrimaryTrigger()  { releaseTrigger(m_primary); }
            void releaseSecondayTrigger() { releaseTrigger(m_secondary); }

            IWeapon* getPrimaryWeapon()   { return m_weapons[m_primary];   }
            IWeapon* getSecondaryWeapon() { return m_weapons[m_secondary]; }

			void setTargetPos(const Vector3& pos) {m_targetPos = pos;}

            const std::vector<IWeapon*>& getAllWeapons() { return m_weapons; }

        private:
            int                   m_primary;
            int                   m_secondary;
            std::vector<IWeapon*> m_weapons;
            CTransform*           m_trans;
            int                   m_shipRadius;
			Vector3				  m_targetPos;

            unsigned getWeapon(Common::Data::Weapons_t weapon);
            void releaseTrigger(int weapon);
            void shoot(int index, unsigned int msecs);
		};

	    REG_FACTORY(CWeapons);
	}
}

#endif