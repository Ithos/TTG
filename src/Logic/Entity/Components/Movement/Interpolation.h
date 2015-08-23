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

#ifndef __LOGIC_INTERPOLATION_H
#define __LOGIC_INTERPOLATION_H

#include "../Component.h"
#include <Common/Util/Math.h>

namespace Logic
{
	namespace Component
	{
        class CMovement;

        class CInterpolation : public IComponent
        {
            DEC_FACTORY(CInterpolation);
	    public:
			CInterpolation() : IComponent(), m_iniPos(0.0f, 0.0f, 0.0f), m_endPos(0.0f, 0.0f, 0.0f), m_currPos(0.0f, 0.0f, 0.0f),
			m_iniRot(0.0f, 0.0f, 0.0f), m_endRot(0.0f, 0.0f, 0.0f), m_currRot(0.0f, 0.0f, 0.0f), m_mov(nullptr), m_freq(0.0f), m_factor(0.0f),
			m_renderTransform(Matrix4::IDENTITY){ }
            ~CInterpolation() { }

            bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);
            void tick(unsigned int);

            const Matrix4& getRenderTransform() { return m_renderTransform; }
            void resetFactor()                  { m_factor = 0.0f; }

        private:
            // posotion
            Vector3 m_iniPos;
            Vector3 m_endPos;
            Vector3 m_currPos;
            // rotation
            Vector3 m_iniRot;
            Vector3 m_endRot;
            Vector3 m_currRot;

            float m_freq;
            float m_factor;

            CMovement* m_mov;

            // Matrix just to render the movmement
            Matrix4 m_renderTransform;
        };

        REG_FACTORY(CInterpolation);
    }
}

#endif