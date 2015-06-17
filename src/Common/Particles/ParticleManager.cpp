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
#include <OGRE\OgreRibbonTrail.h>

using namespace ParticleUniverse;

namespace Common
{
    namespace Particles
    {
        CParticleManager* CParticleManager::m_instance = nullptr;

        const char* const PARTCLE_NAME = "particle";
        const char* const STAR_GALAXY  = "starGalaxy";
        const char* const LASER_NAME   = "laserTrail_";

        CParticleManager::CParticleManager() 
            : m_index(0), m_mgr(nullptr), m_sceneMgr(nullptr), m_iExplosion(0), m_shieldNode(nullptr),
              m_iHits(0), MAX_EXPLOSIONS(20), MAX_HITS(100), MAX_SHOOTS(200), MAX_TRAILS(10), m_iRt(0)
        {
            for (unsigned i = 0; i < NUM_PART_TYPES_GALAXY; ++i)
                m_galaxyParticles[i] = STAR_GALAXY + std::to_string(i);

            for (unsigned i = 0; i < MAX_TRAILS; ++i) {
                m_rt.push_back(nullptr);
                m_node1.push_back(nullptr);
                m_node2.push_back(nullptr);
            }
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
            // delete trails
            for (unsigned i = 0; i < m_rt.size(); ++i) {
                m_sceneMgr->destroySceneNode(m_node1[i]);
                m_sceneMgr->destroySceneNode(m_node2[i]);
                m_sceneMgr->destroyRibbonTrail(m_rt[i]);
            }
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
            for (auto it = m_hits.begin(), end = m_hits.end() ; it != end; ++it)
                m_mgr->destroyParticleSystem(*it, m_sceneMgr);

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
                m_mgr->destroyParticleSystem(*it, m_sceneMgr);

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

        /*------------------ shield -------------------------*/
        void CParticleManager::initShield(Ogre::SceneNode* node)
        {
            m_shield = m_mgr->createParticleSystem(buildName(PARTCLE_NAME, m_index++), "shieldEffect", m_sceneMgr);
            if (node)
                m_shieldNode = node;
        }
        
        void CParticleManager::startShield()
        {
            if (m_shieldNode) {
                m_shieldNode->attachObject(m_shield);
                m_shield->start();
            }
        }

        void CParticleManager::changeQuota(unsigned value, unsigned max)
        {
            if (value > 0.00000f)  {
                ParticleSystem* pTmp = m_shield;
                initShield();
                m_shield->getTechnique(0)->setVisualParticleQuota((m_shield->getTechnique(0)->getVisualParticleQuota() * value) / max);
                m_shield->getTechnique(1)->setVisualParticleQuota((m_shield->getTechnique(1)->getVisualParticleQuota() * value) / max);
                startShield();
                pTmp->stop();
                m_sceneMgr->destroyParticleSystem(pTmp->getName());
            }
            else
                m_shield->stop();
        }
          
        void CParticleManager::releaseShield()
        {
            m_sceneMgr->destroyParticleSystem(m_shield->getName());
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
            if (m_shoots[type].size() < MAX_SHOOTS) {
                for (int i = 0; i < 20; ++i) {
                    ParticleSystem* pSys = m_mgr->createParticleSystem(buildName(PARTCLE_NAME, m_index++), getShootScript(type), m_sceneMgr);
                    m_shoots[type].push_back(pSys);
                    m_sceneMgr->getRootSceneNode()->attachObject(pSys);
                }
            }
        }

        void CParticleManager::startShoot(Weapons_t type, const ::Vector3& src, const ::Vector3& dir, float secs, Ogre::Node* node)
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
            
            secs == 0.000000 ? m_shoots[type][m_vShoots[type]]->start() : m_shoots[type][m_vShoots[type]]->start(secs);
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

                if (m_node1[0]) {
                    for (unsigned i = 0; i < MAX_TRAILS; ++i) {
                        m_sceneMgr->destroySceneNode(m_node1[i]);
                        m_sceneMgr->destroySceneNode(m_node2[i]);
                        if (m_rt[i])
                            m_sceneMgr->destroyRibbonTrail(m_rt[i]);

                        m_node1[i] = m_node2[i] = nullptr;
                        m_rt[i] =  nullptr;
                    }
                }

                m_shoots.clear();
            }
            else {
                if (type == LASER) {
                    for (unsigned i = 0; i < MAX_TRAILS; ++i) {
                        m_sceneMgr->destroySceneNode(m_node1[i]);
                        m_sceneMgr->destroySceneNode(m_node2[i]);
                        if (m_rt[i])
                            m_sceneMgr->destroyRibbonTrail(m_rt[i]);

                        m_node1[i] = m_node2[i] = nullptr;
                        m_rt[i] =  nullptr;
                    }
                }
                else if (m_shoots.count(type) > 0) {
                    for (auto it = m_shoots[type].begin(); it != m_shoots[type].end(); ++it)
                        m_sceneMgr->destroyParticleSystem((*it)->getName());

                    m_shoots[type].clear();
                    m_shoots.erase(type);
                }
            }

			for (unsigned i = 0; i < MAX_TRAILS; ++i) {
                m_rt[i] = nullptr;
                m_node1[i] = nullptr;
                m_node2[i] = nullptr;
            }
        }

        void CParticleManager::laserShot(const ::Vector3& src, const ::Vector3& dir, const float& range)
        {
            if (m_node1[m_iRt]) {
                m_node1[m_iRt]->detachAllObjects();
                m_node2[m_iRt]->detachAllObjects();
                m_sceneMgr->destroySceneNode(m_node1[m_iRt]);
                m_sceneMgr->destroySceneNode(m_node2[m_iRt]);
                if (m_rt[m_iRt])
                    m_sceneMgr->destroyRibbonTrail(m_rt[m_iRt]);
            }

            m_node1[m_iRt] = m_sceneMgr->getRootSceneNode()->createChildSceneNode();
            m_node2[m_iRt] = m_sceneMgr->getRootSceneNode()->createChildSceneNode();
            m_rt[m_iRt] = static_cast<Ogre::RibbonTrail*>(m_sceneMgr->createMovableObject(buildName(LASER_NAME, m_index++).c_str() , "RibbonTrail"));
            m_rt[m_iRt]->setMaterialName("LightRibbonTrail");
            m_rt[m_iRt]->setTrailLength(500);
            m_rt[m_iRt]->setMaxChainElements(500);
      //      m_rt[m_iRt]->setInitialColour(0, 0.58, 0.7, 0.88, 0.74);
            m_rt[m_iRt]->setColourChange(0, 1, 1, 1, 0.5);
            m_rt[m_iRt]->setInitialWidth(0, 10);
            m_rt[m_iRt]->setWidthChange(0, 3);
            m_rt[m_iRt]->addNode(m_node1[m_iRt]); // node to move
            m_node2[m_iRt]->attachObject(m_rt[m_iRt]);
            m_node2[m_iRt]->setPosition(src); // end
            m_node1[m_iRt]->setPosition(src + (dir * range)); // init

            if (m_iRt < MAX_TRAILS-1)
                ++m_iRt;            
            else 
                m_iRt = 0;             
        }

    } // Particles
}