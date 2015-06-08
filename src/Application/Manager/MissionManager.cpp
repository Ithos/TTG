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

#include "MissionManager.h"
#include "GameManager.h"

#include <log.h>
#include <cassert>
#include <random>
#include <chrono>
#include <ctime>

#include "../States/State GUI/MissionNotification.h"
#include "../../Common/Data/Game_Constants.h"
#include <Common/Configure/Configure.h>

namespace Application
{
	const char* const LOG_CMISSIONMGR = "Application::CMissionManager";
	CMissionManager* CMissionManager::m_instance = nullptr;

	CMissionManager::CMissionManager():m_targetSystem(0), m_targetPlanet(-1), m_missionType(0), m_targetsDefeated(0),
										m_desiredItem(""), m_totalSystemNum(0), m_tmpTargetSystem(-1)
	{
		m_instance = this;
		
		m_tmpMission.first = 0;
		m_tmpMission.second.first = "";
		m_tmpMission.second.second = "";

		m_targets.first = 0;
		m_targets.second = 0;
		m_tmpTargets.first = 0;
		m_tmpTargets.second = 0;

		m_itemReward.first = "";
		m_itemReward.second.first = "";
		m_itemReward.second.first = "";

		m_tmpItemReward.first = "";
		m_tmpItemReward.second.first = "";
		m_tmpItemReward.second.second = "";

		m_tmpResourceReward.first = "";
		m_tmpResourceReward.second = 0;

		m_resourceReward.first = "";
		m_resourceReward.second = 0;
	}

	CMissionManager::~CMissionManager()
	{
		m_instance = nullptr;
	}

	bool CMissionManager::init()
	{
		assert(!m_instance && "Second initialization of Application::CMissionManager is not allowed!");

		log_trace(LOG_CMISSIONMGR,"Init\n");

		new CMissionManager();
		CMissionNotification::getInstance()->init();

		return true;
	}

	void CMissionManager::release()
	{
		assert(m_instance && "Application::CMissionManager hasn't been initialized!");

		log_trace(LOG_CMISSIONMGR,"Release\n");

		CMissionNotification::getInstance()->release();

		if(m_instance)
		{
			delete m_instance;
		}
	}

#pragma warning( push )
#pragma warning( disable : 4244 )

	typedef std::pair<unsigned int,std::pair<std::string,std::string>> TMissionDescriptor;
	TMissionDescriptor CMissionManager::generateMission()
	{
		std::time_t seed(std::chrono::system_clock::now().time_since_epoch().count());

		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution(1,5);//TODO --More Missions--//

		{
			using namespace Common::Configuration;
			setDefaultFile(CONFIGURE_FILE);
			setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());
		}

		m_tmpMission.first = distribution(generator);

		if(m_tmpMission.first == 1){

			/// TODO -- The generation of rewards may need some changes -- ///

			//Go to some planet
			std::uniform_int_distribution<int> systemDistribution(0,m_totalSystemNum-1);

			int tmpSystem = std::atoi(CGameManager::getInstance()->getSystem().c_str());

			do{
				m_tmpTargetSystem = systemDistribution(generator);
			}while(m_tmpTargetSystem == tmpSystem);

			/// TODO -- Internationalization -- /// Begin{
			m_tmpMission.second.first = "Go to " + std::string(Common::Data::Game::GAME_GALAXY_SYSTEM_NAME[m_tmpTargetSystem]);
			/// TODO --Description--///
			m_tmpMission.second.second = "Go to " + std::string(Common::Data::Game::GAME_GALAXY_SYSTEM_NAME[m_tmpTargetSystem]);
			/// TODO -- Internationalization -- /// }End

			generateReward(4);


			return m_tmpMission;


		}else if(m_tmpMission.first == 2){
			//Defeat some number of enemies

			std::uniform_int_distribution<int> enemyTypeDist(1,2);///TODO --Use constants and config files for this limits--///
			std::uniform_int_distribution<int> numTargetsDist(1,5);

			m_tmpTargets.first = enemyTypeDist(generator);
			m_tmpTargets.second = numTargetsDist(generator);
			/// TODO -- Internationalization -- /// Begin{
			m_tmpMission.second.first = "Defeat " + std::to_string(m_tmpTargets.second) + " type " + std::to_string(m_tmpTargets.first) + " enemies.";

			/// TODO --Description--///
			m_tmpMission.second.second = "0/" + std::to_string(m_tmpTargets.first) + " enemies defeated.";
			/// TODO -- Internationalization -- /// }End

			generateReward(m_tmpTargets.second);

			return m_tmpMission;
		}
        else if(m_tmpMission.first == 3){

			m_tmpTargetSystem = std::atoi(CGameManager::getInstance()->getSystem().c_str());
			std::string str(CGameManager::getInstance()->getPlanet());
			m_tmpPlanet = std::atoi(str.substr(str.length()-2,1).c_str());

			/// TODO -- Internationalization -- /// Begin{
			m_tmpMission.second.first = "Visit an inhabited planet in another system.";
			m_tmpMission.second.second = "Get away from " + std::string(Common::Data::Game::GAME_GALAXY_SYSTEM_NAME[m_tmpTargetSystem]);
			/// TODO -- Internationalization -- /// }End

			generateReward(3);

			return m_tmpMission;

		}else if(m_tmpMission.first == 4){
			m_tmpTargetSystem = std::atoi(CGameManager::getInstance()->getSystem().c_str());
			std::string str(CGameManager::getInstance()->getPlanet());
			m_tmpPlanet = std::atoi(str.substr(str.length()-2,1).c_str());

			std::uniform_int_distribution<int> desiredItemDist(0,Common::Data::Game::TOTAL_ITEMS - 1);
			
			m_tmpDesiredItem = Common::Data::Game::GAME_ITEM_LIST[desiredItemDist(generator)][0];

			/// TODO -- Internationalization -- /// Begin{
			m_tmpMission.second.first = "Bring me a " + m_tmpDesiredItem;
			m_tmpMission.second.second = "Bring a " + m_tmpDesiredItem + " to planet" + Common::Data::Game::GAME_PLANETS_NAMES[m_tmpTargetSystem][m_tmpPlanet] + 
				" in " + std::string(Common::Data::Game::GAME_GALAXY_SYSTEM_NAME[m_tmpTargetSystem]);

			/// TODO -- Internationalization -- /// }End

			generateReward(9);

			return m_tmpMission;

		}else if(m_tmpMission.first == 5){
			m_tmpTargetSystem = std::atoi(CGameManager::getInstance()->getSystem().c_str());
			std::string str(CGameManager::getInstance()->getPlanet());
			m_tmpPlanet = std::atoi(str.substr(str.length()-2,1).c_str());

			std::uniform_int_distribution<int> quant(10,50);
			m_tmpTotalTargets = quant(generator) * 10;

			/// TODO -- Internationalization -- /// Begin{
			m_tmpMission.second.first = "Bring me " + std::to_string(m_tmpTotalTargets) + " fuel units.";
			m_tmpMission.second.second = "Bring " + std::to_string(m_tmpTotalTargets) + " fuel units to planet" + 
				Common::Data::Game::GAME_PLANETS_NAMES[m_tmpTargetSystem][m_tmpPlanet] + 
				" in " + std::string(Common::Data::Game::GAME_GALAXY_SYSTEM_NAME[m_tmpTargetSystem]);
			/// TODO -- Internationalization -- /// }End

			generateReward(m_tmpTotalTargets/100);
			
			return m_tmpMission;
		}
	}

	void CMissionManager::acceptMission()
	{

		CGameManager::getInstance()->setMission(m_tmpMission.first, m_tmpMission.second.first, m_tmpMission.second.second);

		m_missionType = m_tmpMission.first;
		m_resourceReward = m_tmpResourceReward;
		m_itemReward = m_tmpItemReward;

		if(m_tmpMission.first == 1){

			m_targetSystem = m_tmpTargetSystem;
			m_targetPlanet = -1;
			m_targetsDefeated = 0;
			m_desiredItem = "";
			m_targets.first = 0;
			m_targets.second = 0;

		}else if(m_tmpMission.first == 2){

			m_targets = m_tmpTargets;
			m_targetsDefeated = 0;
			m_targetSystem = 0;
			m_targetPlanet = -1;
			m_desiredItem = "";

		}else if(m_tmpMission.first == 3){
			m_targetSystem = m_tmpTargetSystem;
			m_targetPlanet = m_tmpPlanet;
			m_targetsDefeated = 0;
			m_desiredItem = "";
			m_targets.first = 0;
			m_targets.second = 0;
		}else if(m_tmpMission.first == 4){
			m_targetSystem = m_tmpTargetSystem;
			m_targetPlanet = m_tmpPlanet;
			m_desiredItem = m_tmpDesiredItem;
			m_targetsDefeated = 0;
			m_targets.first = 0;
			m_targets.second = 0;

			if(CGameManager::getInstance()->removeFromCargo(m_desiredItem)){
				getReward(false);
				abandonMission();
			}
		}else if(m_tmpMission.first == 5){
			m_targetSystem = m_tmpTargetSystem;
			m_targetPlanet = m_tmpPlanet;
			m_targetsDefeated = m_tmpTotalTargets;
			m_desiredItem = "";
			m_targets.first = 0;
			m_targets.second = 0;

			if(CGameManager::getInstance()->getPlayerResourceByName(Common::Data::Game::GAME_FUEL) >= m_targetsDefeated){
				CGameManager::getInstance()->decreaseResourceByName(Common::Data::Game::GAME_FUEL, m_targetsDefeated);
				getReward(false);
				abandonMission();
			}
		}

		m_tmpMission.first = 0;
		m_tmpMission.second.first = "";
		m_tmpMission.second.second = "";
		m_tmpTargets.first = 0;
		m_tmpTargets.second = 0;
		m_tmpTargetSystem = -1;
		m_tmpResourceReward.first = "";
		m_tmpResourceReward.second = 0;
		m_tmpItemReward.first = "";
		m_tmpItemReward.second.first = "";
		m_tmpItemReward.second.second = "";

	}

	void CMissionManager::rejectMission()
	{
		m_tmpMission.first = 0;
		m_tmpMission.second.first = "";
		m_tmpMission.second.second = "";
		m_tmpTargets.first = 0;
		m_tmpTargets.second = 0;
		m_tmpTargetSystem = -1;
		m_tmpResourceReward.first = "";
		m_tmpResourceReward.second = 0;
		m_tmpItemReward.first = "";
		m_tmpItemReward.second.first = "";
		m_tmpItemReward.second.second = "";
	}

	void CMissionManager::abandonMission()
	{
		CGameManager::getInstance()->setMission(0, "", "");

		m_missionType = 0;
		m_targets.first = 0;
		m_targets.second = 0;
		m_targetsDefeated = 0;
		m_targetSystem = 0;
		m_targetPlanet = -1;
		m_desiredItem = "";
		m_tmpMission.first = 0;
		m_tmpMission.second.first = "";
		m_tmpMission.second.second = "";
		m_tmpTargets.first = 0;
		m_tmpTargets.second = 0;
		m_tmpTargetSystem = -1;
		m_resourceReward.first = "";
		m_resourceReward.second = 0;
		m_tmpResourceReward.first = "";
		m_tmpResourceReward.second = 0;
		m_itemReward.first = "";
		m_itemReward.second.first = "";
		m_itemReward.second.second = "";
		m_tmpItemReward.first = "";
		m_tmpItemReward.second.first = "";
		m_tmpItemReward.second.second = "";
	}

	void CMissionManager::checkSystem()
	{
		if(m_missionType == 1 && m_targetPlanet == -1)
		{
			if(CGameManager::getInstance()->getSystem() == std::to_string(m_targetSystem)){
				std::time_t seed(std::chrono::system_clock::now().time_since_epoch().count());

				std::default_random_engine generator(seed);
				std::uniform_int_distribution<int> distribution(0,m_planetsPos.size()-1);

				m_targetPlanet = distribution(generator);

				{
					using namespace Common::Configuration;
					setDefaultFile(CONFIGURE_FILE);
					setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());
				}

				/// TODO --Planet names--///
				/// TODO -- Internationalization -- /// Begin{
				CGameManager::getInstance()->setMission(
					1,
					"Go to " + std::string(Common::Data::Game::GAME_GALAXY_SYSTEM_NAME[m_targetSystem]) + 
					". Land on Planet" + std::string(Common::Data::Game::GAME_PLANETS_NAMES[m_targetSystem][m_planetsPos[m_targetPlanet]-1]) + ".",
					"Go to " + std::string(Common::Data::Game::GAME_GALAXY_SYSTEM_NAME[m_targetSystem]) + 
					".\nLand on Planet" + std::string(Common::Data::Game::GAME_PLANETS_NAMES[m_targetSystem][m_planetsPos[m_targetPlanet]-1]) + ".");
				/// TODO -- Internationalization -- /// }End
			}
		}
	}

#pragma warning( pop )

	void CMissionManager::checkPlanet()
	{
		if(m_missionType == 1 && m_targetPlanet != -1 && CGameManager::getInstance()->getSystem() == std::to_string(m_targetSystem))
		{
			std::string str(CGameManager::getInstance()->getPlanet());
			str = str.substr(str.length()-2,1);
			std::string tmp(std::to_string(m_planetsPos[m_targetPlanet]-1));
			if( str == tmp ){
				getReward();
				abandonMission();
			}
		}else if(m_missionType == 3 && CGameManager::getInstance()->isInhabitedPlanet()){
			//Uncomment to allow other inhabited planets in the same system
			/*std::string str(CGameManager::getInstance()->getPlanet());
			str = str.substr(str.length()-1,1);*/
			if(m_targetSystem == std::atoi(CGameManager::getInstance()->getSystem().c_str())/* && m_targetPlanet == std::atoi(str.c_str())*/ )
				return;

			getReward();
			abandonMission();
		}else if(m_missionType == 4 && m_targetSystem == std::atoi(CGameManager::getInstance()->getSystem().c_str())){
			std::string str(CGameManager::getInstance()->getPlanet());
			str = str.substr(str.length()-2,1);
			if(m_targetPlanet == std::atoi(str.c_str()) && CGameManager::getInstance()->removeFromCargo(m_desiredItem)){
				getReward();
				abandonMission();
			}
		}else if(m_missionType == 5 && m_targetSystem == std::atoi(CGameManager::getInstance()->getSystem().c_str())){
			std::string str(CGameManager::getInstance()->getPlanet());
			str = str.substr(str.length()-2,1);
			if(m_targetPlanet == std::atoi(str.c_str()) && CGameManager::getInstance()->getPlayerResourceByName(Common::Data::Game::GAME_FUEL) >= m_targetsDefeated){
				CGameManager::getInstance()->decreaseResourceByName(Common::Data::Game::GAME_FUEL,m_targetsDefeated);
				getReward();
				abandonMission();
			}
		}
	}

	void CMissionManager::enemyDefeated(unsigned int type)
	{
		if(m_missionType == 2 && type == m_targets.first){
			++m_targetsDefeated;
			/// TODO -- Internationalization -- /// Begin{
			CGameManager::getInstance()->setMission(CGameManager::getInstance()->getMission().first,CGameManager::getInstance()->getMission().second,
				std::to_string(m_targetsDefeated) + "/" + std::to_string(m_targets.second) + " enemies defeated.");
			/// TODO -- Internationalization -- /// }End
			if(m_targetsDefeated >= m_targets.second){
				getReward();
				abandonMission();
			}
		}
	}

	void CMissionManager::getReward(bool window)
	{
		if(window)
			CMissionNotification::getInstance()->showNote();

		if(m_resourceReward.first != ""){

			CGameManager::getInstance()->addResourceByName(m_resourceReward.first, m_resourceReward.second);

			m_resourceReward.first = "";
			m_resourceReward.second = 0;
		}

		if(m_itemReward.first != ""){

			CGameManager::getInstance()->addToCargo(m_itemReward.first, m_itemReward.second.first, m_itemReward.second.second);

			m_itemReward.first = "";
			m_itemReward.second.first = "";
			m_itemReward.second.second = "";
		}
	}

	typedef std::pair<std::string, int> TReward;
	TReward CMissionManager::getStringReward()
	{
		if(m_resourceReward.first != ""){

			return m_resourceReward;
		}

		if(m_itemReward.first != ""){

			TReward ret;

			ret.first = m_itemReward.second.first;
			ret.second = -1;

			return ret;
		}
	}

	void CMissionManager::generateReward(unsigned int difficulty)
	{
		std::time_t seed(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> rewardDist(1,100);

		if(difficulty > 9) difficulty = 9;
		if(difficulty < 1) difficulty = 1;

		if(rewardDist(generator)<= 100 - difficulty * 5){

				m_tmpResourceReward.first = Common::Data::Game::GAME_ORE;
				std::uniform_int_distribution<int> moneyDist(1 + difficulty * 2, 10 + difficulty * 5);
				m_tmpResourceReward.second = moneyDist(generator) * 10;

			}else{

				std::uniform_int_distribution<int> itemDist(1,5);
				int idx(itemDist(generator));
				if(idx == 1){
					std::uniform_int_distribution<int> weaponDist(1,1 + ((float)difficulty/9.0)*(Common::Data::Game::TOTAL_PRIMARY_WEAPONS - 2) );
					int item(weaponDist(generator));
					m_tmpItemReward.first = Common::Data::Game::GAME_PRIMARY_WEAPON;
					m_tmpItemReward.second.first = Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[item][0];
					m_tmpItemReward.second.second = Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[item][1];
				}else if(idx == 2){
					std::uniform_int_distribution<int> weaponDist(1,1 + ((float)difficulty/9.0)*(Common::Data::Game::TOTAL_SECONDARY_WEAPONS - 2) );
					int item(weaponDist(generator));
					m_tmpItemReward.first = Common::Data::Game::GAME_SECONDARY_WEAPON;
					m_tmpItemReward.second.first = Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[item][0];
					m_tmpItemReward.second.second = Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[item][1];
				}else if(idx == 3){
					std::uniform_int_distribution<int> engineDist(1,1 + ((float)difficulty/9.0)*(Common::Data::Game::TOTAL_ENGINES - 2) );
					int item(engineDist(generator));
					m_tmpItemReward.first = Common::Data::Game::GAME_ENGINE;
					m_tmpItemReward.second.first = Common::Data::Game::GAME_ENGINES_LIST[item][0];
					m_tmpItemReward.second.second = Common::Data::Game::GAME_ENGINES_LIST[item][1];
				}else if(idx == 4){
					std::uniform_int_distribution<int> specialDeviceDist(0,((float)difficulty/9.0)*(Common::Data::Game::TOTAL_SPECIAL - 1) );
					int item(specialDeviceDist(generator));
					m_tmpItemReward.first = Common::Data::Game::GAME_ITEM_LIST[item][0];
					m_tmpItemReward.second.first = Common::Data::Game::GAME_ITEM_LIST[item][1];
					m_tmpItemReward.second.second = Common::Data::Game::GAME_ITEM_LIST[item][2];
				}else if(idx == 5){
					std::uniform_int_distribution<int> gameItemDist(0,((float)difficulty/9.0)*(Common::Data::Game::TOTAL_ITEMS - 1) );
					int item(gameItemDist(generator));
					m_tmpItemReward.first = Common::Data::Game::GAME_ITEM;
					m_tmpItemReward.second.first = Common::Data::Game::GAME_ITEM_LIST[item][0];
					m_tmpItemReward.second.second = Common::Data::Game::GAME_ITEM_LIST[item][1];
				}
				

			}
	}
}