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

#include "SteeringMovement.h"

#include "../../../Logic.h"
#include "../../../Scene/Scene.h"
#include "../Movement/Transform.h"
#include "../Movement/Movement.h"

#include <Common/Map/MapEntity.h>
#include <AI/Movement/Movement.h>
#include <AI/Movement/DynamicMovement.h>

#include <Application/Clock.h>
#include <Common/Data/TTG_Types.h>
#include <Common/Data/Spawn_Constants.h>

namespace Logic 
{
	namespace Component
	{

		IMP_FACTORY(CSteeringMovement);

		CSteeringMovement::~CSteeringMovement()
		{
			if (m_currentMovement != 0)
				delete m_currentMovement;
			if (m_yaw != 0)
				delete m_yaw;
			if (m_evade != NULL)
				delete m_evade;
		}


		bool CSteeringMovement::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo) 
		{
			using namespace Common::Data::Spawn;

			if(!IComponent::spawn(entity,scene,entityInfo))
				return false;

			if(entityInfo->hasAttribute(COMMON_MAX_SPEED))
				m_maxLinearSpeed = entityInfo->getFloatAttribute(COMMON_MAX_SPEED);

			if(entityInfo->hasAttribute(COMMON_ROTATION_SPEED))
				m_maxAngularSpeed = entityInfo->getFloatAttribute(COMMON_ROTATION_SPEED);

			if(entityInfo->hasAttribute(COMMON_ACCEL))
				m_maxLinearAccel = entityInfo->getFloatAttribute(COMMON_ACCEL);

			if(entityInfo->hasAttribute(COMMON_ROTATION_ACCEL))
				m_maxAngularAccel = entityInfo->getFloatAttribute(COMMON_ROTATION_ACCEL);

			if (entityInfo->hasAttribute(AI_TOLERANCE))
				m_tolerance = entityInfo->getFloatAttribute(AI_TOLERANCE);

			m_yaw = AI::Movement::IMovement::getMovement(AI::Movement::IMovement::MOVEMENT_KINEMATIC_ALIGN_TO_SPEED, 
				m_maxLinearSpeed, m_maxAngularSpeed, m_maxLinearAccel, m_maxAngularAccel); 
			m_yaw->setEntity(entity);

			// Creación del comportamiento para esquivar 
			// OJO: todo esto está muy cableado. En condiciones normales habría que decidir junto a la percepción
			// qué entidades hay que esquivar
			m_evade = new AI::Movement::CDynamicFlee(m_maxLinearSpeed, m_maxAngularSpeed, m_maxLinearAccel, m_maxAngularAccel);
			if (m_evade != NULL) 
			{
				m_evade->setEntity(entity);
				// Ponemos el target a esquivar
				m_evade->setTarget(Vector3(100,-300,1250));
				m_evadeDistance = 100;
			}

			return true;

		}


		void CSteeringMovement::tick(unsigned int msecs)
		{
			IComponent::tick(msecs);

			using namespace Common::Data;

			//setting the target
			if (m_playerTarget) {
				CEntity* player_target = CLogic::getInstance()->getScene()->getEntityByName("Player");
				m_target = static_cast<CTransform*>(player_target->getComponentByName(TRANSFORM_COMP))->getPosition();
			}

			//getting info from the movable entity
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP)); 
			CMovement* movement = static_cast<CMovement*>(m_entity->getComponentByName(MOVEMENT_COMP));

			// Comprobamos si hay que cambiar de movimiento o destino
			if (m_movType != m_currentMovementType || m_currentTarget != m_target) {
				if (m_currentMovement != NULL) {
					delete m_currentMovement;
				}
				m_currentMovementType = m_movType;
				m_currentTarget = m_target;
				m_currentMovement = AI::Movement::IMovement::getMovement(m_currentMovementType, m_maxLinearSpeed, m_maxAngularSpeed, m_maxLinearAccel, m_maxAngularAccel);
				// Si hay un nuevo movimiento
				if (m_currentMovement != NULL) {
					// Ajustamos el target
					m_arrived = false;
					m_currentMovement->setEntity(m_entity);
					m_currentMovement->setTarget(m_currentTarget);
				}
			}

			bool justArrived = !m_arrived && transf->getPosition().positionEquals(m_target, m_tolerance);
			if (justArrived)
				m_arrived = true;

			// Si nos estamos desplazando calculamos la próxima posición
			if (!m_arrived && m_currentMovement != NULL) {
				m_currentMovement->move(m_currentProperties);
			
				// Calculamos la evitación
				if (m_evade != NULL && m_evadeDistance > 0)
				{
					AI::Movement::IMovement::DynamicMovement evadeProperties;
					m_evade->move(evadeProperties);
					// Distancia al objeto a evadir
					Vector3 position = transf->getPosition();
					Vector3 distance = m_evade->getTarget() - position;
					float distanceMagnitude = distance.length();
					if (distanceMagnitude <= m_evadeDistance)
						m_currentProperties.linearAccel += 0.15f * evadeProperties.linearAccel;
				}
	
				// Calculamos la rotación
				m_yaw->move(m_currentProperties);

				// Mover
				float speedValue = m_currentProperties.linearSpeed.length();
				movement->setSpeed(speedValue);
				movement->move(m_currentProperties.linearSpeed);

				// Aplicar la rotación
				movement->rotate((float)m_currentProperties.angularSpeed);

				// Acelerar
				m_currentProperties.linearSpeed += m_currentProperties.linearAccel;

				// Clipping
				if (speedValue > m_maxLinearSpeed)
					m_currentProperties.linearSpeed *= (m_maxLinearSpeed / speedValue);

				m_currentProperties.angularSpeed += m_currentProperties.angularAccel  * msecs * 0.01;
				if (m_currentProperties.angularSpeed > m_maxAngularSpeed) 
					m_currentProperties.angularSpeed = Ogre::Math::Sign((float)m_currentProperties.angularSpeed) * m_maxAngularSpeed;
			}
		}


		void CSteeringMovement::setTarget(Vector3 target)
		{
			m_target = target;
			m_playerTarget = false;
		}
	}
}