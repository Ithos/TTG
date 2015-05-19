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

#ifndef __AI_MOVEMENT_H
#define __AI_MOVEMENT_H

#include "Logic\Entity\Entity.h"
#include "Common\Util\Math.h"

namespace AI 
{
	namespace Movement 
	{
		class IMovement 
		{
		public:
			const static float ATTENUATION;
			const static float SLOW_RADIUS;

			struct DynamicMovement {
				Vector3 linearSpeed;
				double angularSpeed;
				Vector3 linearAccel;
				double angularAccel;

				DynamicMovement() : linearSpeed(Vector3::ZERO), angularSpeed(0), 
					linearAccel(Vector3::ZERO), angularAccel(0) {};
			};

			enum EnmMovementType {
				MOVEMENT_NONE = 0,
				MOVEMENT_KINEMATIC_SEEK,
				MOVEMENT_KINEMATIC_ARRIVE,
				MOVEMENT_KINEMATIC_ALIGN_TO_SPEED,
				MOVEMENT_KINEMATIC_FLEE,
				MOVEMENT_DYNAMIC_SEEK,
				MOVEMENT_DYNAMIC_ARRIVE,
				MOVEMENT_DYNAMIC_FLEE
			};

			IMovement(float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel) : 
				m_entity(0), m_maxLinearSpeed(maxLinearSpeed), m_maxAngularSpeed(maxAngularSpeed), 
				m_maxLinearAccel(maxLinearAccel), m_maxAngularAccel(maxAngularAccel), 
				m_maxLinearSpeed2(maxLinearSpeed * maxLinearSpeed), m_maxLinearAccel2(maxLinearAccel * maxLinearAccel) {};
			~IMovement() {};

			void setEntity(Logic::CEntity* entity) { m_entity = entity; };
			void setTarget(Vector3 &target) { m_target = target; };
			Vector3 getTarget() { return m_target; };
			virtual void move(DynamicMovement& currentProperties) = 0;
			static IMovement* getMovement(int type, float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel);

		protected:
			Logic::CEntity* m_entity;
			Vector3 m_target;
			float m_maxLinearSpeed, m_maxAngularSpeed, m_maxLinearAccel, m_maxAngularAccel;
			float m_maxLinearSpeed2, m_maxLinearAccel2;
			
		};
	}
}

#endif