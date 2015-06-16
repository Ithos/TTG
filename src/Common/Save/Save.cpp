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

#include "Save.h"

#include <Application/Manager/GameManager.h>
#include <Application/Manager/MissionManager.h>
#include "../Map/Map.h"

#include <fstream>
#include <chrono>

namespace Common { namespace Save 
{
	using namespace std;

	const char* const FILE_NAME = "./save/save.dat";
	const char* const SEPARATOR = "##";
	const char* const _NULL = "null_value";
	
	std::string loadLines(std::ifstream& in)
	{
		string line ("");
		string ret ("");

		if(in){
			do{
				ret += line;
				getline(in,line);
			}while(in && line != SEPARATOR);
		}
		return (ret);
	}

	bool loadGame()
	{
		return (true);
		ifstream in (FILE_NAME);

		if(!in.is_open()){ return false; }

		string line ("");

		getline(in,Application::CGameManager::getInstance()->m_system);
		getline(in,Application::CGameManager::getInstance()->m_planet);

		getline(in,line);
		Application::CGameManager::getInstance()->m_stateMap.clear();
		unsigned int num = to<unsigned int>(line);
		for(unsigned int i = 0; i < num; i++){
			string f (loadLines(in));
			getline(in,line);
			Application::CGameManager::getInstance()->m_stateMap.insert(pair<string,unsigned int>(f,to<unsigned int>(line)));
			getline(in,line);
		}

		getline(in,line);
		Application::CGameManager::getInstance()->m_equipmentMap.clear();
		num = to<unsigned int>(line);
		for(unsigned int i = 0; i < num; i++){
			string f (loadLines(in));
			string sf(loadLines(in));
			string ss(loadLines(in));
			pair<string,string> second(sf,ss);
			pair<string,pair<string,string>> p(f,second);
			Application::CGameManager::getInstance()->m_equipmentMap.insert(p);
		}

		getline(in,line);
		Application::CGameManager::getInstance()->m_crewMembersMap.clear();
		num = to<unsigned int>(line);
		for(unsigned int i = 0; i < num; i++){
			string f (loadLines(in));
			getline(in,line);
			Application::CGameManager::getInstance()->m_crewMembersMap.insert(pair<string,unsigned int>(f,to<unsigned int>(line)));
			getline(in,line);
		}

		getline(in,line);
		Application::CGameManager::getInstance()->m_cargoMap.clear();
		num = to<unsigned int>(line);
		for(unsigned int i = 0; i < num; i++){
			string f (loadLines(in));
			string sf(loadLines(in));
			string ss(loadLines(in));
			pair<string,string> second(sf,ss);
			pair<string,pair<string,string>> p(f,second);
			Application::CGameManager::getInstance()->m_cargoMap.insert(p);
		}

		getline(in,line);
		string s(loadLines(in));
		Application::CGameManager::getInstance()->m_activeMission.first = to<unsigned int>(line);
		string aux(_NULL);
		if(!s.compare(aux))
			Application::CGameManager::getInstance()->m_activeMission.second = "";
		else
			Application::CGameManager::getInstance()->m_activeMission.second = s;

		getline(in,line);
		Application::CGameManager::getInstance()->m_engineDataMap.clear();
		num = to<unsigned int>(line);
		for(unsigned int i = 0; i < num; i++){
			string f (loadLines(in));
			int sf = 0;
			int ss = 0;
			getline(in,line);
			sf = to<int>(line);
			getline(in,line);
			getline(in,line);
			ss = to<int>(line);
			getline(in,line);
			pair<int,int> second(sf,ss);
			pair<string,pair<int,int>> p(f,second);
			Application::CGameManager::getInstance()->m_engineDataMap.insert(p);
		}

		getline(in,line);
		Application::CGameManager::getInstance()->m_eqEngineDat.first = to<int>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_eqEngineDat.second = to<int>(line);

		getline(in,line);
		Application::CGameManager::getInstance()->m_life = to<int>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_totalLife = to<unsigned>(line);
		getline(in,line);
        Application::CGameManager::getInstance()->m_maxShield = to<unsigned>(line);
		getline(in,line);
        Application::CGameManager::getInstance()->m_shieldRegen = to<unsigned>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_maxEnergy = to<unsigned>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_energyRegen = to<unsigned int>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_objectives = to<unsigned>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_totalObjectives = to<unsigned>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_nameRepeatCounter = to<unsigned>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_sensorLevel = to<unsigned int>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_fuelConsumeProportion = to<float>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_mineralProportion = to<float>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_distanceProportion = to<float>(line);

		getline(in,line);
		Application::CGameManager::getInstance()->m_targetSystem = to<bool>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_targetPlanet = to<bool>(line);
		getline(in,line);
		Application::CGameManager::getInstance()->m_inhabitedPlanet = to<bool>(line);

		getline(in,line);
		Application::CGameManager::getInstance()->m_systemsVisited.clear();
		num = to<unsigned int>(line);
		for(unsigned int i = 0; i < num; i++){
			getline(in,line);
			Application::CGameManager::getInstance()->m_systemsVisited.insert(to<int>(line));
		}

		getline(in,line);
		(Map::CMap::getInstance()->m_mapGenerator.m_SeedMap.clear());		
		num = to<unsigned int>(line);
		for(unsigned int i = 0; i < num; i++){
				
			string f(loadLines(in));
			getline(in,line);
			pair<string,time_t> p (f,to<unsigned int>(line));
			Map::CMap::getInstance()->m_mapGenerator.m_SeedMap.insert(p);
			getline(in,line);
		}

		return (true);
	}

	bool saveGame()
	{
		ofstream out (FILE_NAME);

		out << Application::CGameManager::getInstance()->m_system << endl;
		out << Application::CGameManager::getInstance()->m_planet << endl;

		out << Application::CGameManager::getInstance()->m_stateMap.size() << endl;
		for(map<string,unsigned int>::const_iterator it = Application::CGameManager::getInstance()->m_stateMap.begin() ;
			it != Application::CGameManager::getInstance()->m_stateMap.end(); ++it){
				out << it->first <<  endl;
				out << SEPARATOR << endl;
				out << it->second << endl;
				out << SEPARATOR << endl;
		}

		out << Application::CGameManager::getInstance()->m_equipmentMap.size() << endl;
		for(map<string,pair<string,string>>::const_iterator it = Application::CGameManager::getInstance()->m_equipmentMap.begin();
			it != Application::CGameManager::getInstance()->m_equipmentMap.end(); ++it){
				out << it->first << endl;
				out << SEPARATOR << endl;
				out << it->second.first << endl;
				out << SEPARATOR << endl;
				out << it->second.second << endl;
				out << SEPARATOR << endl;
		}

		out << Application::CGameManager::getInstance()->m_crewMembersMap.size() << endl;
		for(map<string,unsigned int>::const_iterator it = Application::CGameManager::getInstance()->m_crewMembersMap.begin() ;
			it != Application::CGameManager::getInstance()->m_crewMembersMap.end(); ++it){
				out << it->first <<  endl;
				out << SEPARATOR << endl;
				out << it->second << endl;
				out << SEPARATOR << endl;
		}

		out << Application::CGameManager::getInstance()->m_cargoMap.size() << endl;
		for(multimap<string,pair<string,string>>::const_iterator it = Application::CGameManager::getInstance()->m_cargoMap.begin();
			it != Application::CGameManager::getInstance()->m_cargoMap.end(); ++it){
				out << it->first << endl;
				out << SEPARATOR << endl;
				out << it->second.first << endl;
				out << SEPARATOR << endl;
				out << it->second.second << endl;
				out << SEPARATOR << endl;
		}

		out << Application::CGameManager::getInstance()->m_activeMission.first << endl;
		if(Application::CGameManager::getInstance()->m_activeMission.second != "")
			out << Application::CGameManager::getInstance()->m_activeMission.second << endl;
		else
			out << _NULL << endl;
		out << SEPARATOR << endl;

		out << Application::CGameManager::getInstance()->m_engineDataMap.size() << endl;
		for(map<string,pair<int,int>>::const_iterator it = Application::CGameManager::getInstance()->m_engineDataMap.begin();
			it != Application::CGameManager::getInstance()->m_engineDataMap.end(); ++it){
				out << it->first << endl;
				out << SEPARATOR << endl;
				out << it->second.first << endl;
				out << SEPARATOR << endl;
				out << it->second.second << endl;
				out << SEPARATOR << endl;
		}

		out << Application::CGameManager::getInstance()->m_eqEngineDat.first << endl;
		out << Application::CGameManager::getInstance()->m_eqEngineDat.second << endl;
		out << Application::CGameManager::getInstance()->m_life << endl;
		out << Application::CGameManager::getInstance()->m_totalLife << endl;
		out << Application::CGameManager::getInstance()->m_maxShield << endl;
        out << Application::CGameManager::getInstance()->m_shieldRegen << endl;
		out << Application::CGameManager::getInstance()->m_maxEnergy << endl;
		out << Application::CGameManager::getInstance()->m_energyRegen << endl;
		out << Application::CGameManager::getInstance()->m_objectives << endl;
		out << Application::CGameManager::getInstance()->m_totalObjectives << endl;
		out << Application::CGameManager::getInstance()->m_nameRepeatCounter << endl;
		out << Application::CGameManager::getInstance()->m_sensorLevel << endl;
		out << Application::CGameManager::getInstance()->m_fuelConsumeProportion << endl;
		out << Application::CGameManager::getInstance()->m_mineralProportion << endl;
		out << Application::CGameManager::getInstance()->m_distanceProportion << endl;

		out << Application::CGameManager::getInstance()->m_targetSystem << endl;
		out << Application::CGameManager::getInstance()->m_targetPlanet << endl;
		out << Application::CGameManager::getInstance()->m_inhabitedPlanet << endl;

		out << Application::CGameManager::getInstance()->m_systemsVisited.size() << endl;
		for(set<int>::const_iterator it = Application::CGameManager::getInstance()->m_systemsVisited.begin();
			it != Application::CGameManager::getInstance()->m_systemsVisited.end(); ++it){
				out << *it << endl;
		}
		Map::CMapGenerator mapg = (Map::CMap::getInstance()->m_mapGenerator);
		out << mapg.m_SeedMap.size() << endl;
		for(map<string,time_t>::const_iterator it = mapg.m_SeedMap.begin();
			it != mapg.m_SeedMap.end(); ++it){
				out << it->first << endl;
				out << SEPARATOR << endl;
				out << it->second << endl;
				out << SEPARATOR << endl;
		}

		out.close();

		return (true);
	}
}}