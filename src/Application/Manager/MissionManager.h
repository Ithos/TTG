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

#ifndef __APPLICATION_MISSIONMANAGER_H
#define __APPLICATION_MISSIONMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>

namespace Application
{
	class CMissionManager
	{
	public:
		static CMissionManager* getInstance() {return m_instance;}

		static bool init();

		static void release();

		typedef std::pair<unsigned int,std::pair<std::string,std::string>> TMissionDescriptor;
		TMissionDescriptor generateMission();

		typedef std::pair<std::string, std::pair<std::string,std::string>> TItemDescriptor;
		TItemDescriptor getTmpItemReward(){return m_tmpItemReward;};

		typedef std::pair<std::string, unsigned int> TResourceDescriptor;
		TResourceDescriptor getTmpResourceReward() {return m_tmpResourceReward;};

		typedef std::pair<std::string, int> TReward;
		TReward getStringReward();

		void acceptMission();

		void rejectMission();

		void abandonMission();

		unsigned int getMissionType() {return m_missionType;};

		void setSystemNumber(unsigned int num){m_totalSystemNum = num;};

		void setPlanets(const std::vector<int>& vec) {m_planetsPos = vec;std::sort(m_planetsPos.begin(), m_planetsPos.end());}

		void checkSystem();

		void checkPlanet();

		void enemyDefeated(unsigned int type);

	private:
		static CMissionManager* m_instance;

		unsigned int m_targetSystem, m_missionType, m_targetsDefeated, m_totalSystemNum;
		int  m_targetPlanet;
		std::string m_desiredItem;
		std::vector<int> m_planetsPos;

		typedef std::pair<unsigned int, unsigned int> TTargetsDescriptor; //type, number
		TTargetsDescriptor m_targets, m_tmpTargets;

		TMissionDescriptor m_tmpMission;

		unsigned int m_tmpTargetSystem, m_tmpTotalTargets, m_tmpPlanet;
		std::string m_tmpDesiredItem;

		TItemDescriptor m_itemReward, m_tmpItemReward;
		TResourceDescriptor m_resourceReward, m_tmpResourceReward;

		CMissionManager();
		~CMissionManager();

		void getReward(bool window = true);
		void generateReward(unsigned int difficulty);

	};
}

#endif