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

#include "Controller.h"

#include <Common\data\TTG_Types.h>
#include <Common/Map/MapEntity.h>

#include "../../Entity.h"

#include "Transform.h"
#include "Movement.h"
#include "Interpolation.h"
#include "../Weapons/PlayerWeapons.h"
#include "../Weapons/Weapon.h"
#include "../Weapons/LaserWeapon.h"

namespace Logic
{
	namespace Component
	{

		IMP_FACTORY(CController);

		void CController::tick(unsigned int msecs)
		{
			IComponent::tick(msecs);

			// not in spawn function because controller component creates before movement
			using namespace Common::Data;
			if (!m_mov)
				m_mov = static_cast<CMovement*>(m_entity->getComponentByName(MOVEMENT_COMP));

			if (!m_weapons)
				m_weapons = static_cast<CWeapons*>(m_entity->getComponentByName(WEAPONS_COMP));

			// move
			if (m_moveForward)
				m_mov->moveForward(msecs);
			else if (m_braking)
				m_mov->moveBackward(msecs);
			else
				m_mov->slowDownByFriction(msecs);

			// turn
			if (m_turningRight && !m_turningLeft)
				m_mov->turnRight();
			else if (m_turningLeft && !m_turningRight)
				m_mov->turnLeft();
			else
				m_mov->dontTurn();

			// shots
			if (m_primaryShoot)
				m_weapons->shootPrimaryWeapon(msecs);
			else
				m_weapons->releasePrimaryTrigger();

			if (m_secondaryShoot)
				m_weapons->shootSecondaryWeapon();
			else
				m_weapons->releaseSecondayTrigger();
		}

		void CController::primaryShoot(bool b, const Vector3& vec){ 
			m_primaryShoot   = b; 
			m_weapons->setTargetPos(vec);
		}

		void CController::secondaryShoot(bool b, const Vector3& vec){ 
			m_secondaryShoot   = b; 
			m_weapons->setTargetPos(vec);
		}
	}
}
