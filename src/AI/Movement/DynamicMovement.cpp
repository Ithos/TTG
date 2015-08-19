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

#include "DynamicMovement.h"

#include "../AI.h"

#include <Common/Data/TTG_Types.h>
#include <Logic/Entity/Components/Movement/Transform.h>

namespace AI 
{
	namespace Movement
	{
		using namespace Common::Data;
		using namespace Logic::Component;

		void CDynamicSeek::move(DynamicMovement& currentProperties)
		{
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
			currentProperties.linearAccel = m_target - transf->getPosition();
			if (currentProperties.linearAccel.squaredLength() > m_maxLinearAccel2) {
				currentProperties.linearAccel.normalise();
				currentProperties.linearAccel *= m_maxLinearAccel;
			}
		}


		void CDynamicArrive::move(DynamicMovement &currentProperties)
		{
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
			Vector3 targetSpeed = m_target - transf->getPosition();
			float distance = targetSpeed.length();
			float targetMagnitude;
			if (distance >= IMovement::SLOW_RADIUS)
				targetMagnitude = m_maxLinearSpeed;
			else
				targetMagnitude = distance * m_maxLinearSpeed / IMovement::SLOW_RADIUS;
			
			targetSpeed /= distance;
			targetSpeed *= targetMagnitude;
			currentProperties.linearAccel = targetSpeed - currentProperties.linearSpeed;
			
			currentProperties.linearAccel/= IMovement::ATTENUATION;
			if (currentProperties.linearAccel.squaredLength() > m_maxLinearAccel2) {
				currentProperties.linearAccel.normalise();
				currentProperties.linearAccel *= m_maxLinearAccel;
			}
		}


		void CDynamicFlee::move(DynamicMovement& currentProperties)
		{
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
			currentProperties.linearAccel = transf->getPosition() - m_target;
			currentProperties.linearAccel = currentProperties.linearAccel + currentProperties.linearAccel.crossProduct(Vector3(0.0f, 1.0f, 0.0f));
			currentProperties.linearAccel.normalise();
			currentProperties.linearAccel *= m_maxLinearAccel;
		}
	}
}