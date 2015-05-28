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

#ifndef __LOGIC_MISSILETRIGGER_H
#define __LOGIC_MISSILETRIGGER_H

#include "../Physic/PhysicEntity.h"

#ifndef _InOut
#define _InOut
#endif

namespace Common
{
    namespace Particles
    {
        class CParticleManager;
    }
}

using namespace Common::Particles;

namespace Logic
{
	namespace Component
	{
        class CTransform;

        class CMissileTrigger : public CPhysicEntity
        {
            DEC_FACTORY(CMissileTrigger)
        public:
            CMissileTrigger() 
                : m_parent(nullptr), moveFunc(nullptr), m_shooted(false), m_speed(.0f), m_damage(.0f), m_range(.0f), m_parentTrans(nullptr),
                m_particles(nullptr), m_node(nullptr), m_bb(nullptr)
            {  }

            ~CMissileTrigger();

            bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

            void tick(unsigned int);

            bool activate();

            void onOverlapBegin(IPhysic* otherComponent);

			void onOverlapEnd(IPhysic* otherComponent);

            // movFunc = define how to move the trigger, linear by default.
            void setMove(void (*movFunc)(_InOut Vector3&/*src*/, const Vector3&/*dir*/, float/*speed*/) = nullptr) { moveFunc = movFunc; }

            void shoot(const Vector3& src, const Vector3& dir);

        private:
            void (*moveFunc)(_InOut Vector3&, const Vector3&, float);

            CEntity*    m_parent;
            CTransform* m_parentTrans;
            Matrix4     m_trans;
            Vector3     m_dir;
            Vector3     m_pos;
            float       m_speed;
            float       m_damage;
            float       m_range;
            bool        m_shooted;
            CParticleManager* m_particles;
            Ogre::RibbonTrail* m_rt;
            Ogre::SceneNode*   m_node;
            static Ogre::SceneManager* m_sceneMgr;
            static Ogre::BillboardSet* m_set;
            Ogre::Billboard*  m_bb;
        };

        REG_FACTORY(CMissileTrigger)
    }
}

#endif