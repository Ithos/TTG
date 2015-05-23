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

#ifndef __LOGIC_SHIELDTRIGGER_H
#define __LOGIC_SHIELDTRIGGER_H

#include "../Physic/PhysicEntity.h"

namespace Ogre
{
    class SceneNode;
}

namespace Logic
{
	namespace Component
	{
		class CTransform;
        class CShieldTrigger : public CPhysicEntity        
        {
            DEC_FACTORY(CShieldTrigger)
			
        public:
            CShieldTrigger() : m_parent(nullptr), m_trans(nullptr) 
            {
                ++m_numTriggers;
            }

            ~CShieldTrigger() { m_parent =  nullptr; }

            bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

            void tick(unsigned int);

            void onOverlapBegin(IPhysic* otherComponent);

			void onOverlapEnd(IPhysic* otherComponent);

        private:
            CEntity*         m_parent;
            CTransform*      m_trans;
            Ogre::SceneNode* m_sceneNode;
            static int       m_numTriggers;
            std::string      m_nameChildNode;
        };

        REG_FACTORY(CShieldTrigger)
    }
}

#endif