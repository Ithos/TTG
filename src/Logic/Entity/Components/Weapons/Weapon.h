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

#ifndef __LOGIC_WEAPON_H
#define __LOGIC_WEAPON_H

#include <Common/Util/Math.h>
#include <Common/data/TTG_Types.h>

namespace Logic
{
	namespace Component
	{
        struct IWeapon
        {
            IWeapon() 
                : m_damage(.0f), m_cadence(.0f), m_range(0), m_speed(.0f), m_trigger(false) 
            { }

            virtual ~IWeapon() { }
			
			virtual void tick(unsigned int) {} // para los misiles

            /*
            @param src : source position from where throwing the ray
            @param dir : ray direction
            */
            virtual void shoot( const Vector3& src, const Vector3& dir ) { }

            virtual void releaseTrigger() { m_trigger = false; }

			virtual float getRange() {return m_range;}

            float    m_damage;
            float    m_cadence;
            unsigned m_range;   // dintance before disappear
            float    m_speed;
            int      m_ammo; // current bullets in changers
            int      m_maxCharger; // max capacity
            Vector3  m_position;
            bool     m_trigger;

            Common::Data::Weapons_t m_type;
        };
    }
}

#endif