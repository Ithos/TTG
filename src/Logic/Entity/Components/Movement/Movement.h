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

#ifndef __LOGIC_MOVEMENT_H
#define __LOGIC_MOVEMENT_H

#include "../Component.h"
#include <Common/Util/Math.h>

namespace Logic
{
	namespace Component
	{
        class CTransform;

        class CMovement : public IComponent
	    {
		    DEC_FACTORY(CMovement)
	    public:
            CMovement() 
                : IComponent(), m_iniPos(.0f, .0f, .0f), m_endPos(.0f, .0f, .0f), m_lastTime(.0f),
                m_pitch(0.0), m_yaw(0.0), m_roll(0.0), m_speed(.0f), m_trans(nullptr), m_maxSpeed(0.0f),
                m_desaceleration(0.0), m_friction(0.0), m_rotSpeed(0.0), m_aceleration(0.0), m_nextPos(0.0, 0.0, 0.0),
				m_iniRot(0.0, 0.0, 0.0), m_nextRot(.0f, .0f, .0f), m_maxRoll(0.0), m_rollSpeed(0.0), m_endTf(Matrix4()), m_onContact(false),
				m_enginePos(0)
            { }

            ~CMovement() {}

            bool spawn(CEntity* entity, CScene *scene, const Map::CMapEntity* entityInfo);
            void tick(unsigned int);

            void moveForward(unsigned int msecs);
            void moveBackward(unsigned int msecs);
            void slowDownByFriction(unsigned int msecs); 
            // -size : hit object size. Bigger size, bigger slowing dowm.
            void slowDownByHit(unsigned size = 1);
            void turnLeft();
            void turnRight();
            void dontTurn();
            void boost(unsigned secs){};
     
            const Vector3& getIniPos()  { return m_iniPos; }
            const Vector3& getNextPos() { return m_nextPos; }
            const Vector3& getIniRot()  { return m_iniRot; }
            const Vector3& getNextRot() { return m_nextRot;}

            const float& getSpeed() { return m_speed; }

			Matrix4 getTransform() { return m_endTf; }

            void setLastStepTIme(float f) { m_lastTime = f;    }
            float getLastTimeStep()       { return m_lastTime; }

			void rotate(const float& yaw = 0.0f, const float& pitch = 0.0f, const float& roll = 0.0f);
			void move(const Vector3& dir);
			void setSpeed(float speed) { m_speed = speed; }

			bool m_onContact;

        private:
            Vector3     m_iniPos;
            Vector3     m_nextPos;
            Vector3     m_endPos;
            Vector3     m_iniRot;
            Vector3     m_nextRot;
			Matrix4     m_endTf;
            float       m_speed;
            float       m_pitch;
            float       m_yaw;
            float       m_roll;
            float       m_lastTime;
            float       m_maxRoll;
            float       m_rollSpeed;
            float       m_desaceleration;
            float       m_friction;
            float       m_rotSpeed;
            float       m_aceleration;
            float       m_maxSpeed;
            CTransform* m_trans;

			int			m_enginePos;
	    };

	    REG_FACTORY(CMovement);
    }
}

#endif