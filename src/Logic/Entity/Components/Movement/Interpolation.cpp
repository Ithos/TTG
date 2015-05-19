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

#include "Interpolation.h"
#include "Movement.h"
#include "Transform.h"
#include "Logic\Entity\Entity.h"
#include <Common\data\TTG_Types.h>
//#include "Controller.h"
//#include "SteeringMovement.h"

#include <Application/Clock.h>

#include <OgreSceneManager.h>
#include "Logic\Scene\Scene.h"

using namespace Common::Data;

namespace Logic 
{
    namespace Component
    {
        IMP_FACTORY(CInterpolation)

        bool CInterpolation::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            if(!IComponent::spawn(entity,scene,entityInfo))
			    return false;

            // get pointer to movement (if a component is gonna move, must to have CMovement)
            m_mov = static_cast<CMovement*>(m_entity->getComponentByName(MOVEMENT_COMP));
            m_freq = m_mov->getPriority();
            m_renderTransform = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP))->getTransform();

            return true;
        }

        void CInterpolation::tick(unsigned int msecs)
        {
            // interpolation value [0..1] ( tNow - lasStepTime / freqComp )
            m_factor = m_freq > 1 ? (((Application::CClock::getInstance().getTime() - m_mov->getLastTimeStep()) / m_freq) / 1000) : 1.0f;

            // Position
            m_iniPos = m_mov->getIniPos();
            m_endPos = m_mov->getNextPos();
            // rotation
            m_iniRot = m_mov->getIniRot();
            m_endRot = m_mov->getNextRot();

            // interpolated movement
            if (m_factor < 1.0f) {
                m_renderTransform.setTrans((m_iniPos * (1.0f - m_factor)) + (m_endPos * m_factor));
                Common::Util::Math::rotate((m_iniRot * (1.0f - m_factor)) + (m_endRot * m_factor), m_renderTransform);
            }
            else {
                m_renderTransform.setTrans(m_endPos);
                Common::Util::Math::rotate(m_endRot, m_renderTransform);
            }
        }
    }
}