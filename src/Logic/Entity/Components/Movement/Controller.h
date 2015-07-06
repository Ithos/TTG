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

#ifndef __LOGIC_CONTROLLER_H
#define __LOGIC_CONTROLLER_H

#include "../Component.h"
#include <common/Util/Math.h>

namespace Logic
{
	namespace Component
	{
        class CMovement;
        class CWeapons;

		class CController : public IComponent
		{
			DEC_FACTORY(CController);

		public:
			CController() : IComponent(), m_moveForward(false), m_braking(false), m_turningLeft(false), 
                m_turningRight(false), m_primaryShoot(false), m_secondaryShoot(false), m_mov(nullptr),
                m_weapons(nullptr)
            { }

			~CController() { }
            void tick(unsigned int msecs);

            // inlines moves 
            void moveForward(bool b)    { m_moveForward    = b; }
            void slowDown(bool b)       { m_braking        = b; }
            void primaryShoot(bool b)   { m_primaryShoot   = b; }
			void primaryShoot(bool b, const Vector3& vec);
            void secondaryShoot(bool b) { m_secondaryShoot = b; }
			void secondaryShoot(bool b, const Vector3& vec);
            void turnRight(bool b)      { m_turningRight   = b; }
            void turnLeft(bool b)       { m_turningLeft    = b; }
            //---
			bool isTurningRight()  { return m_turningRight; }
			bool isTurningLeft()   { return m_turningLeft; }
			bool isMovingForward() { return m_moveForward; }
			bool isBraking()       { return m_braking; }

		private:
			bool         m_moveForward;
			bool         m_braking;
			bool         m_turningRight;
			bool         m_turningLeft;
            bool         m_primaryShoot;
            bool         m_secondaryShoot;
            CMovement*   m_mov;
            CWeapons*    m_weapons;

		};

	REG_FACTORY(CController);
	}
}
#endif