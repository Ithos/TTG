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

#include "KinematicMovement.h"

#include "../AI.h"

#include <Common/Data/TTG_Types.h>
#include <Logic/Entity/Components/Movement/Transform.h>

namespace AI 
{
	namespace Movement
	{
		using namespace Common::Data;
		using namespace Logic::Component;

		void CKinematicSeek::move(DynamicMovement& currentProperties)
		{
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
			currentProperties.linearSpeed = m_target - transf->getPosition();
			if (currentProperties.linearSpeed.length() > m_maxLinearSpeed) {
				currentProperties.linearSpeed.normalise();
				currentProperties.linearSpeed *= m_maxLinearSpeed;
			}
		}


		void CKinematicArrive::move(DynamicMovement &currentProperties)
		{
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
			currentProperties.linearSpeed = (m_target - transf->getPosition());
			currentProperties.linearSpeed /= IMovement::ATTENUATION;
			if (currentProperties.linearSpeed.squaredLength() > m_maxLinearSpeed2) {
				currentProperties.linearSpeed.normalise();
				currentProperties.linearSpeed *= m_maxLinearSpeed;
			}
		}


		void CKinematicAlignToSpeed::move(DynamicMovement& currentProperties)
		{
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
			currentProperties.angularSpeed = (atan2(-currentProperties.linearSpeed.x, -currentProperties.linearSpeed.z) - transf->getYaw());
			currentProperties.angularSpeed = AI::CAI::correctAngle(currentProperties.angularSpeed);
			//currentProperties.angularSpeed /= IMovement::ATTENUATION;
			if (abs(currentProperties.angularSpeed) > m_maxAngularSpeed) {
				currentProperties.angularSpeed = Ogre::Math::Sign(currentProperties.angularSpeed) * m_maxAngularSpeed;
			}
		}


		void CKinematicFlee::move(DynamicMovement& currentProperties)
		{
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
			currentProperties.linearSpeed = transf->getPosition() - m_target;
			currentProperties.linearAccel = currentProperties.linearAccel + currentProperties.linearAccel.crossProduct(Vector3(0.0f, 1.0f, 0.0f));
			currentProperties.linearSpeed.normalise();
			currentProperties.linearSpeed *= m_maxLinearSpeed;
		}

		void CKinematicAlignToTarget::move(DynamicMovement& currentProperties)
		{
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
			Vector3 vec(m_target - transf->getPosition());
			currentProperties.angularSpeed = (atan2(-vec.x, -vec.z) - transf->getYaw());//
			currentProperties.angularSpeed = AI::CAI::correctAngle(currentProperties.angularSpeed);
			//currentProperties.angularSpeed /= IMovement::ATTENUATION;
			if (abs(currentProperties.angularSpeed) > m_maxAngularSpeed) {
				currentProperties.angularSpeed = Ogre::Math::Sign(currentProperties.angularSpeed) * m_maxAngularSpeed;
			}
		}
	}
}
