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

#ifndef __COMMON_PARTICLES
#define __COMMON_PARTICLES

#include <vector>
#include <map>
#include "Common/Util/Math.h"
#include "common/Data/TTG_Types.h"

using namespace Common::Data;

namespace ParticleUniverse
{
    class ParticleSystem;
    class ParticleSystemManager;
}

typedef enum star_t { GALAXY_STAR = 0x000, SYSTEM_STAR, PLANET_STAR };

namespace Common
{
    namespace Particles
    {
        const int NUM_PART_TYPES_GALAXY = 5;

        typedef enum ribbonTrail_t { LASER_BLUE, LASER_RED, LASER_GREEN };

        /**
            Contain the management of generic particle effects shaed for several entities, like
            explosions, hits, star effect, etc.
        */
        class CParticleManager
        {
        public:
            static CParticleManager* getInstance() 
            {
                return (!m_instance) ? m_instance = new CParticleManager() : m_instance;
            }

            // Fill all particle pools.
            bool init(Ogre::SceneManager*);

            /*-------- Explosions --------*/
            void initExplosions();
            void startNextExplosion( const Vector3& pos);
            void releaseExplosions();

            void initBomExplosions();
            void startBombExplosion(const Vector3& pos);
            void releaseBombExplosion();

            void initBomEffect();
            void startBombEffect(const Vector3& pos);
            void releaseBombEffect();

            /*-------- Hits -----------*/
            void initHits();
            void startHit(const Vector3& pos);
            void releaseHits();

            /******** Shield **********/
            void initShield(Ogre::SceneNode* = nullptr);
            void startShield();
            void changeQuota(unsigned value, unsigned max);
            void releaseShield();

            /*--------- Shoots --------*/
            void initShoots();
            void addShootType(Weapons_t);
            void startShoot(Weapons_t type, const Vector3& src, const Vector3& dir, float = 0.0f, Ogre::Node* = nullptr);
            void releaseShoots(Weapons_t = ALL);

            void laserShot(Vector3 init, Vector3 dir, float range, ribbonTrail_t trail = LASER_BLUE);

            /********** Stars **********/
            void addStar(star_t , Ogre::SceneNode* = nullptr);
            void startStarEffect();
            void releaseStars();

            void release(); // call it in realse states;

            ParticleUniverse::ParticleSystemManager* m_mgr;

        private:
            CParticleManager();

			 ~CParticleManager()
            { 
               
            }

            std::string getShootScript(Weapons_t);

            Ogre::SceneManager* m_sceneMgr;

            typedef std::vector<ParticleUniverse::ParticleSystem*> vPU;

            vPU m_explosions;
            vPU m_bombExplosion;
            vPU m_bomEffect;
            vPU m_hits;
            vPU m_stars;
            std::map<Weapons_t, vPU> m_shoots;
            ParticleUniverse::ParticleSystem* m_shield;

            std::string m_galaxyParticles[NUM_PART_TYPES_GALAXY];

            // shield 
            Ogre::SceneNode* m_shieldNode;

            // laser
            std::vector<Ogre::SceneNode*>   m_node1; // node for creating ribbontrail
            std::vector<Ogre::SceneNode*>   m_node2; // node for moving ribbontrail
            std::vector<Ogre::RibbonTrail*> m_rt; 
			std::vector<std::string> m_names;
            unsigned m_iRt;
            
            // index
            unsigned m_ibombEffect;
            unsigned m_index;
            unsigned m_iExplosion;
            unsigned m_iBombExplosion;
            std::vector<unsigned> m_vShoots;
            unsigned m_iHits;

            static CParticleManager* m_instance;

            static std::string getRibbontrailName(ribbonTrail_t t);
        };
    }

    template <typename T, typename T2>
    static std::string buildName(const T& param1, const T2& param2) 
    {
        std::stringstream str;
        str << param1 << param2;
        return str.str();
    }
}

#endif