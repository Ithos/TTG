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
#include <Application/Manager/GameManager.h>
#include <random>
#include <chrono>
#include <ctime>

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
				m_playerTarget(false), m_fixedTime(0.0f), m_frequency(0.0f), m_freq (0.0f, 2.0f), m_seed(std::chrono::system_clock::now().time_since_epoch().count()),
				m_generator(m_seed), m_evading(false), m_stopEvadeDistance(0.0f), m_evadeDistance(0.0f), m_primary(false), m_shootingBeam(false)
			{ if(m_freq(m_generator) < 1.0f){ m_primary = true; }};
			~CSteeringMovement();
			void setTarget(Vector3 target);
			void setPlayerAsTarget();

			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);
			virtual void tick(unsigned int msecs);

			void setEvadeObstacle(Vector3 pos);
			void setEvadePlayer(Vector3 pos);

		private:
			float m_maxLinearSpeed, m_maxAngularSpeed, m_maxLinearAccel, m_maxAngularAccel, m_tolerance;
			bool m_arrived;
			Vector3 m_currentTarget, m_target;
			int m_currentMovementType, m_movType;
			AI::Movement::IMovement* m_currentMovement;
			AI::Movement::IMovement* m_yaw;
			AI::Movement::IMovement* m_evade;
			float m_evadeDistance, m_stopEvadeDistance;
			AI::Movement::IMovement::DynamicMovement m_currentProperties;
			std::time_t m_seed;
			std::default_random_engine m_generator;
			std::uniform_real_distribution<float> m_freq;

			float m_fixedTime;

			bool m_playerTarget, m_shootingBeam;

			Logic::Component::CWeapons* m_weapons;
			float m_frequency;
			bool m_evading;
			bool m_primary;

		};

	REG_FACTORY(CSteeringMovement);
	}
}

#endif
