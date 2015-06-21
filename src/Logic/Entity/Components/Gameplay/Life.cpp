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

#include "Life.h"

#include <Application/Manager/GameManager.h>
#include <Application/Manager/MissionManager.h>

#include <Logic/Entity/Entity.h>
#include <Common/Data/Spawn_Constants.h>
#include <Common/Map/MapEntity.h>
#include <Common/Data/TTG_Types.h>
#include "../Graphic/Graphics.h"

namespace Logic
{
	namespace Component
	{
        IMP_FACTORY(CLife)

        CLife::CLife() : IComponent(), m_life(0), m_enemType(0)
        { }

        CLife::~CLife()
        {
            if ( !m_player )
                delete m_life;
        }

        bool CLife::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
        {
            if (!IComponent::spawn(entity, scene, entityInfo))
		        return false;

            if (entity->isPlayer()) {
                m_player = true;
                m_life   = Application::CGameManager::getInstance()->getRefLife();
            }
            else {
                m_player = false;
                m_life   = new int();
			
				if (entityInfo->hasAttribute(Common::Data::Spawn::COMMON_LIFE)) {

					*m_life = static_cast<unsigned>(entityInfo->getIntAttribute(Common::Data::Spawn::COMMON_LIFE));

					if (entity->getType() == "Asteroid") {
						// smaller size, less life
						float scale = static_cast<CGraphics*>(entity->getComponentByName(Common::Data::GRAPHICS_COMP))->getScale();
						if (scale == 10)
							(*m_life) /= 3;
						else if (scale == 20)
							*m_life /= 2;
					}
				}
			}
			if (entityInfo->hasAttribute(Common::Data::Spawn::MISSION_ENEMY_TYPE) && entity->getType() == "Enemy"){
				m_enemType = static_cast<unsigned int>(entityInfo->getIntAttribute(Common::Data::Spawn::MISSION_ENEMY_TYPE));
			}
            
            return true;
        }

		void CLife::deactivate()
		{
			IComponent::deactivate();
			if(*m_life <= 0){
				Application::CMissionManager::getInstance()->enemyDefeated(m_enemType);
			}
		}

		bool CLife::decreaseLife(unsigned int num)
		{
			if (m_player)
				Application::CGameManager::getInstance()->decreaseLife(num);
			else {
				if (*m_life > num)  *m_life -= num;
				else                 *m_life = 0;
			}

			return *m_life <= 0;
		}
    }
}