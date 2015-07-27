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
#include "Logic/Entity/Components/Weapons/Weapon.h"

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

			m_weapons = static_cast<Logic::Component::CWeapons*>(m_entity->getComponentByName(Common::Data::WEAPONS_COMP));

			// Creación del comportamiento para esquivar 
			// OJO: todo esto está muy cableado. En condiciones normales habría que decidir junto a la percepción
			// qué entidades hay que esquivar
			m_evade = new AI::Movement::CDynamicFlee(m_maxLinearSpeed, m_maxAngularSpeed, m_maxLinearAccel, m_maxAngularAccel);
			if (m_evade != NULL) 
			{
				m_evade->setEntity(entity);
				// Ponemos el target a esquivar
				m_evade->setTarget(Vector3(10000,10000,10000));
				m_evadeDistance = 200;
				m_stopEvadeDistance = 300;
			}

			return true;

		}

		void CSteeringMovement::setEvadeObstacle(Vector3 pos)
		{
			float dist1(static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP))->getPosition().squaredDistance(m_evade->getTarget()));
			float dist2(static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP))->getPosition().squaredDistance(pos));

			if(dist2 < dist1)
				m_evade->setTarget(pos);
		}
		
		void CSteeringMovement::setEvadePlayer(Vector3 pos)
		{
			float dist1(static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP))->getPosition().squaredDistance(m_evade->getTarget()));
			float dist2(static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP))->getPosition().squaredDistance(pos));

			if(dist2 < 9000 && dist2 < dist1)
				m_evade->setTarget(pos);

		}


		void CSteeringMovement::tick(unsigned int msecs)
		{
			IComponent::tick(msecs);

			using namespace Common::Data;

			//setting the target
			if (m_playerTarget) {
				CEntity* player_target = CLogic::getInstance()->getScene()->getEntityByName("Player");
				if(!player_target) return;

				m_target = static_cast<CTransform*>(player_target->getComponentByName(TRANSFORM_COMP))->getPosition();
				m_yaw->setTarget(m_target);
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
					if (distanceMagnitude <= m_evadeDistance){
						m_currentProperties.linearAccel += (10.0f * evadeProperties.linearAccel * (msecs * 0.001));
						m_evading = true;
					} else if(m_evading &&  distanceMagnitude <= m_stopEvadeDistance){
						m_currentProperties.linearAccel += (10.0f * evadeProperties.linearAccel  * (msecs * 0.001));
					}else if(distanceMagnitude > m_stopEvadeDistance)
						m_evading = false;
				}else{
					m_evading = false;
				}
	
				// Calculamos la rotación
				if(m_fixedTime >= 0.1f){
					m_yaw->move(m_currentProperties);
					m_fixedTime = 0.0f;
				}else{
					m_fixedTime += msecs * 0.001;
				}

				// Mover
				float speedValue = m_currentProperties.linearSpeed.length();
				movement->setSpeed(speedValue);
				movement->move(m_currentProperties.linearSpeed);

				// Aplicar la rotación
				float angConst( 0.7/(msecs * 0.01));//5.1
				

				movement->rotate((float)m_currentProperties.angularSpeed * angConst);

				// Acelerar
				m_currentProperties.linearSpeed += m_currentProperties.linearAccel;

				// Clipping
				if (speedValue > m_maxLinearSpeed)
					m_currentProperties.linearSpeed *= (m_maxLinearSpeed / speedValue);

				m_currentProperties.angularSpeed += m_currentProperties.angularAccel;
				if (m_currentProperties.angularSpeed > m_maxAngularSpeed) 
					m_currentProperties.angularSpeed = Ogre::Math::Sign((float)m_currentProperties.angularSpeed) * m_maxAngularSpeed;
			}

			float minTime(1.0f);

			minTime +=(Application::CGameManager::getInstance()->getTotalObjectives() - 
				Application::CGameManager::getInstance()->getObjectivesAquired()) * 0.5f;

			if (m_playerTarget) {
				if ( !m_primary ? m_frequency >= minTime + m_freq(m_generator) :  
					(m_weapons->getPrimaryWeaponType() == Common::Data::Weapons_t::LASER_BEAM ? m_frequency >= minTime + m_freq(m_generator) :
					m_frequency >= ((minTime + m_freq(m_generator))/2.0f)) ) {

					if(m_primary){
						if(m_weapons->getPrimaryWeaponType() == Common::Data::Weapons_t::LASER_BEAM){

							if(((m_weapons->getPrimaryWeapon()->getRange() * m_weapons->getPrimaryWeapon()->getRange()) * 1.1) >= 
						static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->getPosition().squaredDistance(m_target) ){
								m_shootingBeam = !m_shootingBeam;
								if(m_shootingBeam)
									m_weapons->shootPrimaryWeapon(msecs);
								else
									m_weapons->getPrimaryWeapon()->releaseTrigger();
							}else{
								m_shootingBeam = false;
								m_weapons->getPrimaryWeapon()->releaseTrigger();
							}

						}else{
							if(((m_weapons->getPrimaryWeapon()->getRange() * m_weapons->getPrimaryWeapon()->getRange()) * 1.1) >= 
						static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->getPosition().squaredDistance(m_target) )
							m_weapons->shootPrimaryWeapon(msecs);
						}
					}else{
						if(((m_weapons->getSecondaryWeapon()->getRange() * m_weapons->getSecondaryWeapon()->getRange()) * 1.1) >= 
					static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->getPosition().squaredDistance(m_target) )
						m_weapons->shootSecondaryWeapon();
					}

					m_frequency = 0.0f;

				}else{
					m_frequency += (msecs/1000.0f);

					if(m_primary && m_weapons->getPrimaryWeaponType() == Common::Data::Weapons_t::LASER_BEAM && m_shootingBeam){
						if(((m_weapons->getPrimaryWeapon()->getRange() * m_weapons->getPrimaryWeapon()->getRange()) * 1.1) >= 
						static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->getPosition().squaredDistance(m_target) ){
							m_weapons->shootPrimaryWeapon(msecs);
						}else{
							m_weapons->getPrimaryWeapon()->releaseTrigger();
						}
					}
				}
			}
		}


		void CSteeringMovement::setTarget(Vector3 target)
		{
			if(!m_playerTarget){
				m_target = target;
				m_playerTarget = false;
			}
		}

		void CSteeringMovement::setPlayerAsTarget()
		{
			 m_playerTarget = true;
			 delete m_yaw;
			 m_yaw = AI::Movement::IMovement::getMovement(AI::Movement::IMovement::MOVEMENT_KINEMATIC_ALIGN_TO_TARGET, 
				m_maxLinearSpeed, m_maxAngularSpeed, m_maxLinearAccel, m_maxAngularAccel); 
			 m_yaw->setEntity(m_entity);
		}
	}
}