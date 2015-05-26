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

#include "Movement.h"
#include "Transform.h"

#include <log.h>
#include <Application/Clock.h>

#include "Logic\Entity\Entity.h"

#include <Common\data\TTG_Types.h>
#include <Common/Data/Spawn_Constants.h>
#include <Common/Map/MapEntity.h>

using namespace Common::Data;

namespace Logic
{
	namespace Component
    {
        const char* const LOG_MOV = "Logic::CMovement";
        IMP_FACTORY(CMovement)

        bool CMovement::spawn(CEntity* entity, CScene *scene, const Map::CMapEntity* entityInfo)
        {
            using namespace Common::Data::Spawn;

            if (!IComponent::spawn(entity,scene,entityInfo))
			    return false;

            m_trans = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
            if (!m_trans)
                log_error(LOG_MOV, "Error getting Transform, m_trans is null");

            if (entityInfo->hasAttribute(COMMON_SPEED))
                m_aceleration = entityInfo->getFloatAttribute(COMMON_SPEED);

            if (entityInfo->hasAttribute(COMMON_ROTATION_SPEED))
                m_rotSpeed = entityInfo->getFloatAttribute(COMMON_ROTATION_SPEED);

            if (entityInfo->hasAttribute(COMMON_MAXROLL))
                m_maxRoll = entityInfo->getFloatAttribute(COMMON_MAXROLL);

            if (entityInfo->hasAttribute(COMMON_ROLLSPEED))
                m_rollSpeed = entityInfo->getFloatAttribute(COMMON_ROLLSPEED);

            if (entityInfo->hasAttribute(COMMON_FRICTION))
                m_friction = entityInfo->getFloatAttribute(COMMON_FRICTION);

            if (entityInfo->hasAttribute(COMMON_DESACELERATION))
                m_desaceleration = entityInfo->getFloatAttribute(COMMON_DESACELERATION);

            if (entityInfo->hasAttribute(COMMON_MAX_SPEED))
                m_maxSpeed = entityInfo->getFloatAttribute(COMMON_MAX_SPEED);

            return true;
        }

        void CMovement::tick(unsigned int msecs)
        {

			if(m_onContact){ m_onContact = false; return; }

            using namespace Common::Util::Math;
            m_lastTime = (float)Application::CClock::getInstance().getTime();
            // move
            m_iniPos = m_trans->getPosition();
            m_nextPos = m_iniPos + (m_endPos * m_speed * msecs);
            m_trans->setPosition(m_nextPos);

            //rotate
            //getRotation(m_trans->getTransform(), m_iniRot);
			float secs = msecs * 0.001;
			m_iniRot    = m_nextRot;
			m_nextRot.x = m_pitch * secs;
			m_nextRot.y = m_yaw * secs;
			m_nextRot.z = m_roll * secs;
			m_trans->rotateRad(m_yaw * secs, m_pitch * secs, m_roll * secs);
        }

        void CMovement::move(const Vector3& dir /*normalised*/)
        {
            m_endPos = dir;
        }

        void CMovement::rotate(const float& yaw, const float& pitch, const float& roll)
        {
            m_pitch = pitch;
            m_yaw   = yaw;
            m_roll  = roll; 
        }

        void CMovement::moveForward(unsigned int msecs)
        {
            Vector3 vecx, vecy, vecz;
            m_trans->getOrientation().ToAxes(vecx, vecy, vecz);
            m_speed = (m_speed < m_maxSpeed) ? m_speed + (m_aceleration * msecs * 0.001) : m_maxSpeed;
            move(-vecz.normalisedCopy());
        }

        void CMovement::moveBackward(unsigned int msecs)
        {
            Vector3 vecx, vecy, vecz;
            m_trans->getOrientation().ToAxes(vecx, vecy, vecz);
            m_speed = (m_speed > -m_maxSpeed) ? m_speed - (m_desaceleration * msecs * 0.001) : -m_maxSpeed;
            move(-vecz.normalisedCopy());
        }

        void CMovement::slowDownByFriction(unsigned int msecs)
        {
            Vector3 vecx, vecy, vecz;
            m_trans->getOrientation().ToAxes(vecx, vecy, vecz);
            m_speed = (m_speed > 0.005) ? m_speed - m_friction * 0.1 * msecs * 0.001 : (m_speed < -0.005) ? m_speed + m_friction * 0.1 * msecs * 0.001 : 0.0f;
            move(-vecz.normalisedCopy());
        }

        void CMovement::slowDownByHit(unsigned size)
        {
            Vector3 vX, vY, vZ;
            m_trans->getOrientation().ToAxes(vX, vY, vZ);
            m_speed = (m_speed > 0) ? m_speed - (m_friction * size) : m_speed + (m_friction * size);
            move(-vZ.normalisedCopy());
        }

        void CMovement::turnLeft()
        {
            float currRoll = Common::Util::Math::getRoll(m_trans->getTransform());
            float deltaRotation = m_maxSpeed / (std::abs(m_speed) + m_maxSpeed);
            rotate(m_rotSpeed * deltaRotation, .0f, (currRoll < m_maxRoll && currRoll > -m_maxRoll)? m_rollSpeed : .0f);
        }

        void CMovement::turnRight()
        {
            float currRoll = Common::Util::Math::getRoll(m_trans->getTransform());
            float deltaRotation = m_maxSpeed / (std::abs(m_speed) + m_maxSpeed);
            rotate(-m_rotSpeed * deltaRotation, .0f, (currRoll < m_maxRoll && currRoll > -m_maxRoll)? -m_rollSpeed : .0f);
        }

        // stabilizing roll efect
        void CMovement::dontTurn() 
        {
            float currRoll = Common::Util::Math::getRoll(m_trans->getTransform());

            // set a precision of 4 decimals
			if ((int)(currRoll*10000) < 0)
                rotate(.0f, .0f, m_rollSpeed-0.01);
			else if ((int)(currRoll*10000) > 0)
				rotate(.0f, .0f, -m_rollSpeed+0.01);
            else {
                m_nextRot = 0.0f;
                rotate(); // do not rotate
            }
        }

    }
}