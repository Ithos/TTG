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

#ifndef __LOGIC_STEERINGMOVEMENT_H
#define __LOGIC_STEERINGMOVEMENT_H

#include "../Component.h"
#include <AI/Movement/Movement.h>
#include <AI/AI.h>

#include "Logic/Entity/Components/Weapons/PlayerWeapons.h"

namespace Logic 
{
	namespace Component
	{
		class CSteeringMovement : public IComponent
		{
			DEC_FACTORY(CSteeringMovement);

		public:
			CSteeringMovement() : IComponent(), m_arrived(true), m_maxLinearSpeed(0.05f), m_maxAngularSpeed(0.01f),
				m_maxLinearAccel(0.1f), m_maxAngularAccel(0.1f), m_currentMovement(0), 
				m_movType(AI::Movement::IMovement::MOVEMENT_DYNAMIC_ARRIVE), m_yaw(NULL), m_evade(NULL),
				m_playerTarget(true), m_fixedTime(0.0f), m_frequency(0){ };
			~CSteeringMovement();
			void setTarget(Vector3 target);
			void setPlayerAsTarget() { m_playerTarget = true; };

			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);
			virtual void tick(unsigned int msecs);

		private:
			float m_maxLinearSpeed, m_maxAngularSpeed, m_maxLinearAccel, m_maxAngularAccel, m_tolerance;
			bool m_arrived;
			Vector3 m_currentTarget, m_target;
			int m_currentMovementType, m_movType;
			AI::Movement::IMovement* m_currentMovement;
			AI::Movement::IMovement* m_yaw;
			AI::Movement::IMovement* m_evade;
			float m_evadeDistance;
			AI::Movement::IMovement::DynamicMovement m_currentProperties;

			float m_fixedTime;

			bool m_playerTarget;

			Logic::Component::CWeapons* m_weapons;
			unsigned m_frequency;
		};

	REG_FACTORY(CSteeringMovement);
	}
}

#endif
