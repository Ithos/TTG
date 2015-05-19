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

#include "PropertyManager.h"
#include "../../Entity.h"
#include "../../../../Common/Sound/Sound.h"
#include "../Movement/Transform.h"
#include "../../../../Logic/Logic.h"
#include "../../../../Logic/Scene/Scene.h"

#include <Common/Map/MapEntity.h>

namespace Logic
{
	namespace Component
	{
		IMP_FACTORY(CPropertyManager);

		CPropertyManager::~CPropertyManager()
		{
			for(int i=0;i<m_paramNames.size();++i){
				Common::Sound::CSound::getSingletonPtr()->releaseParam(m_paramNames[i]);
			}
			for(int i=0;i<m_eventNames.size();++i){
				Common::Sound::CSound::getSingletonPtr()->releaseEvent(m_eventNames[i]);
			}
			for(int i=0;i<m_numBanks;++i){
				Common::Sound::CSound::getSingletonPtr()->releaseBank("bank"+std::to_string(i));
			}
		}

		bool CPropertyManager::spawn(CEntity* entity, CScene *scene, const Map::CMapEntity *entityInfo)
		{
			if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

			//This component may handle many banks, properties, events and parameters,
			//as long as the appropiate name convention is followed 
			//there are examples of the name convention in the MapGenerator

			int i=0;
			while(entityInfo->hasAttribute("property" + std::to_string(i))){
				m_properties.push_back(entityInfo->getStringAttribute("property" + std::to_string(i)));
				++i;
			}
			if(m_properties.empty())
				return false;

			i=0;
			while(entityInfo->hasAttribute("entities"+ std::to_string(i))){
				m_entities.push_back(entityInfo->getStringAttribute("entities"+ std::to_string(i)));
				++i;
			}
			if(m_entities.size() != m_properties.size())
				return false;

			i=0;
			while(entityInfo->hasAttribute("range" + std::to_string(i) + '0')){
				std::vector<int> tmpVec;
				int j=0;
				while(entityInfo->hasAttribute("range" + std::to_string(i) + std::to_string(j))){
					tmpVec.push_back(entityInfo->getIntAttribute("range" + std::to_string(i)+ std::to_string(j)));
					++j;
				}
				m_ranges.push_back(tmpVec);
				++i;
			}

			if(m_ranges.size() != m_properties.size())
				return false;

			i=0;
			while(entityInfo->hasAttribute("bank" + std::to_string(i))){
				Common::Sound::CSound::getSingletonPtr()->addBank(entityInfo->getStringAttribute("bank" + std::to_string(i)),"bank" + std::to_string(i));
				++i;
			}

			if(!i)
				return false;

			m_numBanks = i;

			i=0;
			while(entityInfo->hasAttribute("event"+std::to_string(i)) && entityInfo->hasAttribute("eventName"+std::to_string(i)))
			{
				m_eventNames.push_back(entityInfo->getStringAttribute("eventName"+std::to_string(i)));
				Common::Sound::CSound::getSingletonPtr()->addEvent(entityInfo->getStringAttribute("event"+std::to_string(i)),m_eventNames[i]);
				++i;
			}
			if(m_eventNames.size() != m_properties.size()) 
				return false;

			i=0;
			while(entityInfo->hasAttribute("parameter"+std::to_string(i)) && entityInfo->hasAttribute("parameterName"+std::to_string(i)))
			{
				m_paramNames.push_back(entityInfo->getStringAttribute("parameterName"+std::to_string(i)));
				Common::Sound::CSound::getSingletonPtr()->addParameter(m_eventNames[i],entityInfo->getStringAttribute("parameter"+std::to_string(i)), m_paramNames[i]);
				++i;
			}
			if(m_paramNames.size() != m_properties.size())
				return false;

			return true;
		}

		void CPropertyManager::tick(unsigned int msec)
		{
			for(int i=0;i<m_properties.size();++i){
				///TODO --Hadle more properties--///
				if(m_properties[i] == "distance")
				{
					assert(Logic::CLogic::getInstance()->getScene()->getEntityByName(m_entities[i]+'0')!=nullptr && "Designated entity type not found.");

					Ogre::Vector3 entityPos(static_cast<CTransform*>(Logic::CLogic::getInstance()->getScene()->
							getEntityByName(m_entities[i]+'0')->getComponentByName("CTransform"))->getPosition());
					Ogre::Vector3 playerPos(static_cast<CTransform*>(m_entity->getComponentByName("CTransform"))->getPosition());
					float Dist = (entityPos - playerPos).squaredLength();
					int num=1;
					while(Logic::CLogic::getInstance()->getScene()->getEntityByName(m_entities[i]+std::to_string(num)))
					{
						entityPos = static_cast<CTransform*>(Logic::CLogic::getInstance()->getScene()->
							getEntityByName(m_entities[i]+std::to_string(num))->getComponentByName("CTransform"))->getPosition();
						playerPos = static_cast<CTransform*>(m_entity->getComponentByName("CTransform"))->getPosition();
					float tmpDist = (entityPos - playerPos).squaredLength();
						if(tmpDist<Dist)
							Dist=tmpDist;
						++num;
					}

					Dist = std::sqrt(Dist);

					if((int)Dist>m_ranges[i][0])
						Common::Sound::CSound::getSingletonPtr()->setParamValue(m_paramNames[i],0.0f);

					for(int j=0;j<m_ranges[i].size();++j)
					{
						///TODO --differentiation between continuous and discrete parameters--///
						if((int)Dist<=m_ranges[i][j])
							Common::Sound::CSound::getSingletonPtr()->setParamValue(m_paramNames[i],j+1);
					}
				}
			}
		}

		bool CPropertyManager::activate()
		{
            m_activate = true;

			for(int i=0;i<m_eventNames.size();++i)
				Common::Sound::CSound::getSingletonPtr()->startEvent(m_eventNames[i]);

			return true;
		}

		void CPropertyManager::deactivate()
		{
            m_activate = false;

			for(int i=0;i<m_eventNames.size();++i)
				Common::Sound::CSound::getSingletonPtr()->stopEvent(m_eventNames[i]);
		}
	}
}