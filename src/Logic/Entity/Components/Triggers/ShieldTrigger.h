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

namespace Common 
{ 
    namespace Particles 
    {
        class CParticleManager;
    }
}

namespace Logic
{
    class CScene;

	namespace Component
	{
		class CTransform;
        class CShield;

        class CShieldTrigger : public CPhysicEntity        
        {
            DEC_FACTORY(CShieldTrigger)
			
        public:
            CShieldTrigger() :
                m_parent(nullptr), m_trans(nullptr), m_particleMngr(nullptr), m_activateShield(true), m_secsNoShield(0),
                m_timeOut(0), m_sceneNode(nullptr), m_nameChildNode(""), m_compShield(nullptr), m_scene(nullptr), 
                m_msecsToReload(0), m_timeOutReload(0), m_firstTime(true), m_timesChangeQuota(0), m_soundName("")
            { }

            ~CShieldTrigger();

            bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

            void tick(unsigned int);

			bool activate() override;

			void deactivate() override;

            void onOverlapBegin(IPhysic* otherComponent);

			void onOverlapEnd(IPhysic* otherComponent);

            void destroyShield();

        private:
            CEntity*         m_parent;
            CTransform*      m_trans;
            Ogre::SceneNode* m_sceneNode;
            std::string      m_nameChildNode;
            Common::Particles::CParticleManager* m_particleMngr;
            unsigned         m_secsNoShield;
            unsigned         m_timeOut;
            bool             m_activateShield;
            CShield*         m_compShield;
            CScene*          m_scene;
            unsigned         m_msecsToReload;
            unsigned         m_timeOutReload;
            unsigned         m_timesChangeQuota;
            bool             m_firstTime;
			std::string		 m_soundName;
        };

        REG_FACTORY(CShieldTrigger)
    }
}

#endif