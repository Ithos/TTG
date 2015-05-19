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

#include "ParticleManager.h"
#include <OgreSceneManager.h>
#include <Ogre/ParticleUniverse\ParticleUniverseSystemManager.h>
#include <OGRE/ParticleUniverse/ParticleEmitters/ParticleUniverseLineEmitter.h>

using namespace ParticleUniverse;

namespace Common
{
    namespace Particles
    {
        CParticleManager* CParticleManager::m_instance = nullptr;

        const char* const PARTCLE_NAME = "particle";
        const char* const STAR_GALAXY  = "starGalaxy";

        CParticleManager::CParticleManager() 
            : m_index(0), m_mgr(nullptr), m_sceneMgr(nullptr), m_iExplosion(0),
              m_iHits(0), MAX_EXPLOSIONS(20), MAX_HITS(100), MAX_SHOOTS(200)
        {
            for (unsigned i = 0; i < NUM_PART_TYPES_GALAXY; ++i)
                m_galaxyParticles[i] = STAR_GALAXY + std::to_string(i);
        }

        bool CParticleManager::init(Ogre::SceneManager* sceneMgr)
        {
            m_mgr      = ParticleSystemManager::getSingletonPtr();
            m_sceneMgr = sceneMgr;

            return true;
        }

        void CParticleManager::release()
        {
            //Delete the particle systems
		    ParticleSystemManager::getSingletonPtr()->destroyAllParticleSystems(m_sceneMgr);
        }

        //----------- hits ------------------//
        void CParticleManager::initHits()
        {
            for (unsigned i = 0; i < MAX_EXPLOSIONS; ++i) {
                ParticleSystem* pSys = m_mgr->createParticleSystem(buildName(PARTCLE_NAME, m_index++), "wave", m_sceneMgr);
                m_hits.push_back(pSys);
                pSys->setScaleTime(10);
                m_sceneMgr->getRootSceneNode()->attachObject(pSys);
            }
        }
        
        void CParticleManager::startHit(const ::Vector3& pos)
        {
            m_hits[m_iHits]->getTechnique(0)->position = pos;
            m_hits[m_iHits]->start(2);
            m_iHits = (m_iHits < MAX_EXPLOSIONS-1) ? m_iHits + 1 : 0;
        }

        void CParticleManager::releaseHits()
        {
            for (auto it = m_hits.begin(); it != m_hits.end(); ++it)
                m_sceneMgr->destroyParticleSystem((*it)->getName());

            m_hits.clear();
            m_iHits = 0;
        }

        //----------- Explosions -------------//
        void CParticleManager::initExplosions()
        {
            // create explosions
            for (unsigned i = 0; i < MAX_EXPLOSIONS; ++i) {
                ParticleSystem* pSys = m_mgr->createParticleSystem(buildName(PARTCLE_NAME, m_index++), "explosion", m_sceneMgr);
                m_explosions.push_back(pSys);
                m_sceneMgr->getRootSceneNode()->attachObject(pSys);
            }
        }

        void CParticleManager::startNextExplosion( const ::Vector3& pos)
        {
            m_explosions[m_iExplosion]->getTechnique(0)->position = pos;
            m_explosions[m_iExplosion]->start(2);
            m_iExplosion = (m_iExplosion < MAX_EXPLOSIONS-1) ? m_iExplosion + 1 : 0;
        }

        void CParticleManager::releaseExplosions()
        {
            for (auto it = m_explosions.begin(); it != m_explosions.end(); ++it)
                m_sceneMgr->destroyParticleSystem((*it)->getName());

            m_explosions.clear();
            m_iExplosion = 0;
        }

        //----------- stars  ----------------//
        void CParticleManager::addStar(star_t type, Ogre::SceneNode* node)
        {
            ParticleSystem* particle;
            switch (type) {
            case GALAXY_STAR:
        //        srand((unsigned)time(0));
       //         particle = m_mgr->createParticleSystem(buildName(PARTCLE_NAME, m_index++), m_galaxyParticles[rand() % NUM_PART_TYPES_GALAXY], m_sceneMgr);
                break;
            case SYSTEM_STAR:
                particle = m_mgr->createParticleSystem(buildName(PARTCLE_NAME, m_index++), "starSystem", m_sceneMgr);
                break;
            case PLANET_STAR:
      //          particle = m_mgr->createParticleSystem(buildName(PARTCLE_NAME, m_index++), "starPlanet", m_sceneMgr);
                break;
            default:
                break;
            }

            m_stars.push_back(particle);
            if (node)
                node->attachObject(particle);
            else
                m_sceneMgr->getRootSceneNode()->attachObject(particle);
        }

        void CParticleManager::startStarEffect()
        {
            for (auto it = m_stars.begin(); it != m_stars.end(); ++it)
                (*it)->start();
        }


        void CParticleManager::releaseStars()
        {
            for (auto it = m_stars.begin(); it != m_stars.end(); ++it)
                m_sceneMgr->destroyParticleSystem((*it)->getName());

            m_stars.clear();
        }

        /*----------------- shoots -------------------- */
        std::string CParticleManager::getShootScript(Weapons_t type)
        {
            switch (type) {
            case LASER:
                return "laser";
            case MISSILE_LINEAR:
//todo                return "missile";
            default: 
                return "laser";
            }
        }

        void CParticleManager::initShoots()
        {
            for (int i = 0; i < Weapons_t::END; ++i)
                m_vShoots.push_back(0);
        }

        void CParticleManager::addShootType(Weapons_t type)
        {
            // add every shoot into their own pool
            if (m_shoots[type].size() < MAX_SHOOTS) {
                for (int i = 0; i < 20; ++i) {
                    ParticleSystem* pSys = m_mgr->createParticleSystem(buildName(PARTCLE_NAME, m_index++), getShootScript(type), m_sceneMgr);
                    m_shoots[type].push_back(pSys);
                    m_sceneMgr->getRootSceneNode()->attachObject(pSys);
                }
            }
        }

        void CParticleManager::startShoot(Weapons_t type, const ::Vector3& src, const ::Vector3& dir, unsigned secs, Ogre::Node* node)
        { 
            LineEmitter* emitter = nullptr;
            switch (type) {
            case LASER:
                m_shoots[type][m_vShoots[type]]->setScaleTime(10);
                emitter = static_cast<LineEmitter*>(m_shoots[type][m_vShoots[type]]->getTechnique(0)->getEmitter(0));
                emitter->position = src;
                emitter->setParticleDirection(dir);
                break;
            case MISSILE_LINEAR:
                m_shoots[type][m_vShoots[type]]->getTechnique(0)->position = src;
            }
            
            secs == 0 ? m_shoots[type][m_vShoots[type]]->start() : m_shoots[type][m_vShoots[type]]->start(secs);
            m_vShoots[type] = (m_vShoots[type] < m_shoots[type].size()-1) ? m_vShoots[type] + 1 : 0;
        }

        void CParticleManager::releaseShoots(Weapons_t type)
        {
            if (type == ALL) {
                for (auto it = m_shoots.begin(); it != m_shoots.end(); ++it) {
                    for (auto p = it->second.begin(); p != it->second.end(); ++p) {
                        m_sceneMgr->destroyParticleSystem((*p)->getName());
                    }
                    it->second.clear();
                }

                m_shoots.clear();
            }
            else {
                if (m_shoots.count(type) > 0) {
                    for (auto it = m_shoots[type].begin(); it != m_shoots[type].end(); ++it)
                        m_sceneMgr->destroyParticleSystem((*it)->getName());

                    m_shoots[type].clear();
                    m_shoots.erase(type);
                }
            }
        }



    } // Particles
}