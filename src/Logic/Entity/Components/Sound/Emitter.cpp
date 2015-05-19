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

#include "Emitter.h"
#include "../../Entity.h"
#include "../../../../Common/Sound/Sound.h"
#include <Common/Map/MapEntity.h>
#include "../Movement/Transform.h"

namespace Logic
{
	namespace Component
	{
		IMP_FACTORY(CEmitter);

		CEmitter::~CEmitter()
		{
			for(int i=0;i<m_soundName.size();++i)
				Common::Sound::CSound::getSingletonPtr()->release3dSound(m_soundName[i]);
		}

		bool CEmitter::spawn(CEntity* entity, CScene *scene, const Map::CMapEntity *entityInfo)
		{
			//This component may handle many sounds,
			//as long as the appropiate name convention is followed. 
			//There are examples of the name convention in the MapGenerator

			if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

			int i=0;
			while(entityInfo->hasAttribute("loop"+std::to_string(i))){
				m_loop.push_back(entityInfo->getBoolAttribute("loop"+std::to_string(i)));
				++i;
			}
			if(m_loop.empty())
				return false;

			i=0;
			while(entityInfo->hasAttribute("autoPlay"+std::to_string(i))){
				m_startPaused.push_back(!entityInfo->getBoolAttribute("autoPlay"+std::to_string(i)));
				++i;
			}
			if(m_startPaused.size() != m_loop.size())
				return false;

			i=0;
			while(entityInfo->hasAttribute("minDist"+std::to_string(i))){
				m_minDist.push_back(entityInfo->getFloatAttribute("minDist"+std::to_string(i)));
				++i;
			}
			if(m_minDist.size() != m_loop.size())
				return false;

			i=0;
			while(entityInfo->hasAttribute("maxDist"+std::to_string(i))){
				m_maxDist.push_back(entityInfo->getFloatAttribute("maxDist"+std::to_string(i)));
				++i;
			}
			if(m_maxDist.size() != m_loop.size())
				return false;

			i=0;
			while(entityInfo->hasAttribute("soundFile"+std::to_string(i))){
				m_soundFile.push_back(entityInfo->getStringAttribute("soundFile"+std::to_string(i)));
				++i;
			}
			if(m_soundFile.size() != m_loop.size())
				return false;

			i=0;
			while(entityInfo->hasAttribute("soundName"+std::to_string(i))){
				m_soundName.push_back(entityInfo->getStringAttribute("soundName"+std::to_string(i)));
				++i;
			}
			if(m_soundName.size() != m_loop.size())
				return false;

			for(int j=0;j<m_soundName.size();++j)
				Common::Sound::CSound::getSingletonPtr()->add3dSound(m_soundFile[j],m_soundName[j],m_loop[j]);

			return true;
		}

		bool CEmitter::activate()
		{
            m_activate = true;

			for(int i=0;i<m_soundName.size();++i){
				if(!m_startPaused[i])
				{
					Common::Sound::CSound::getSingletonPtr()->play3dSound(
					m_soundName[i],static_cast<CTransform*>(m_entity->getComponentByName("CTransform"))->getTransform());
					Common::Sound::CSound::getSingletonPtr()->set3DMinMaxDistance(m_soundName[i],m_minDist[i],m_maxDist[i]);
				}
			}

			return true;
		}

		void CEmitter::deactivate()
		{
            m_activate = false;

			for(int i=0;i<m_soundName.size();++i){
				Common::Sound::CSound::getSingletonPtr()->stop3dSound(m_soundName[i]);
			}
		}

		void CEmitter::tick(unsigned int msec)
		{
			for(int i=0;i<m_soundName.size();++i){
				Common::Sound::CSound::getSingletonPtr()->update3dSound(
					m_soundName[i],static_cast<CTransform*>(m_entity->getComponentByName("CTransform"))->getTransform());
			}
		}

		void CEmitter::play(int i)
		{
			Common::Sound::CSound::getSingletonPtr()->play3dSound(
				m_soundName[i],static_cast<CTransform*>(m_entity->getComponentByName("CTransform"))->getTransform());
			Common::Sound::CSound::getSingletonPtr()->set3DMinMaxDistance(m_soundName[i],m_minDist[i],m_maxDist[i]);
		}

		void CEmitter::pauseResume(int i)
		{
			Common::Sound::CSound::getSingletonPtr()->pauseResume3dSound(m_soundName[i]);
		}
	}

}