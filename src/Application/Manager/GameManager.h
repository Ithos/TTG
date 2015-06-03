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

#ifndef __APPLICATION_GAMEMANAGER_H
#define __APPLICATION_GAMEMANAGER_H
	
#include <string>
#include <map>
#include <set>

namespace Application
{
	class CGeneralNotification;
}

namespace CEGUI
{
	class Window;
	class EventArgs;
	class AnimationInstance;
}

namespace Application
{
	class CGameManager
	{
	public:
		static CGameManager* getInstance(){return m_instance;};

		static bool init();

		static void release();

		void setSystem(const std::string& sys){m_system = sys;};

		void setPlanet(const std::string& planet){m_planet = planet;};

		const std::string& getSystem() { return m_system; }
		const std::string& getPlanet() { return m_planet; }

		int getPlayerResourceByName(const std::string& str);

		bool addResourceByName(const std::string& str, unsigned int num);

		bool decreaseResourceByName(const std::string& str, unsigned int num);

		typedef std::map<std::string,std::pair<std::string,std::string>> TActiveEquipment;
		const TActiveEquipment& getPlayerActiveEquipment(){return m_equipmentMap;};

		int getCrewMembersByName(const std::string& str);

		bool addCrewMemberByName(const std::string& str, unsigned int num = 1);

		bool subtractCrewMemberByName(const std::string& str, unsigned int num = 1);

		typedef std::multimap<std::string,std::pair<std::string,std::string>> TCargo;
		const TCargo& getPlayerCargo(){return m_cargoMap;};

		void addToCargo(const std::string& key, const std::string& name, const std::string& desc);

		bool removeFromCargo(const std::string& name);

		typedef std::pair<unsigned int,std::string> TMission;
		TMission getMission(){ return m_activeMission;};

		void setMission(unsigned int id, const std::string& str, const std::string& desc);

		const int& getLife() { return m_life; }
        int* getRefLife()    { return &m_life; }
		void increaseLife(unsigned int num);
		void decreaseLife(unsigned int num);
		unsigned int getTotalLife(){return m_totalLife;}; // ??
		void increaseTotalLife(unsigned int num);
		void decreaseTotalLife(unsigned int num);

		unsigned int getShield() { return m_shield; }
        unsigned* getShieldREF() { return &m_tmpShield; }
		void resetSceneShield() {m_tmpShield = m_shield;}
		void increaseShield(unsigned int num);
		void decreaseShield(unsigned int num);
		unsigned int getShieldState(){return m_tmpShield;}
		void decreaseShieldState(unsigned int num) { m_tmpShield = (num > m_tmpShield) ? 0 : m_tmpShield - num; }
		void increaseShieldState(unsigned int num) { m_tmpShield += num;  if ( m_tmpShield > m_shield)  m_tmpShield = m_shield; }
		unsigned int getShieldRegen() {return m_shieldRegen;}
		void increaseShieldRegen(unsigned int num) {(m_shieldRegen<=30-num)?m_energyRegen+=num:m_energyRegen;}
		void decreaseShieldRegen(unsigned int num) {(m_shieldRegen>=6+num)?m_energyRegen-=num:m_energyRegen;}

		unsigned int getEnergy(){return m_energy;}
		void resetSceneEnergy() {m_tmpEnergy = m_energy;}
		void increaseEnergy(unsigned int num);
		void decreaseEnergy(unsigned int num);
		unsigned int getEnergyState(){return m_tmpEnergy;}
		void decreaseEnergyState(unsigned int num){num>m_tmpEnergy ? m_tmpEnergy : m_tmpEnergy -= num;}
		void increaseEnergyState(unsigned int num){m_tmpEnergy += num; m_tmpEnergy>m_energy ? m_tmpEnergy=m_energy : m_tmpEnergy;}
		unsigned int getEnergyRegen() {return m_energyRegen;}
		void increaseEnergyRegen(unsigned int num) {(m_energyRegen<=30-num)?m_energyRegen+=num:m_energyRegen;}
		void decreaseEnergyRegen(unsigned int num) {(m_energyRegen>=6+num)?m_energyRegen-=num:m_energyRegen;}

		void addEngineData(const std::string& name, int maxDist, int cons);

		typedef std::pair<int,int> TEquippedEngineData;
		int getEqEngineDist(){return m_eqEngineDat.first;};
		int getEqEngineCons(){return m_eqEngineDat.second;};
		const std::string& getEqEngine() {return m_eqEngine;};

		void objectiveAquired();
		unsigned int getObjectivesAquired() {return m_objectives;};
		unsigned int getTotalObjectives() {return m_totalObjectives;};
		bool isTargetSystem() {return m_targetSystem;}
		bool isTargetPlanet() {return m_targetPlanet;}
		void setIsTargetSystem(bool set) {m_targetSystem = set;}
		void setIsTargetPlanet(bool set) {m_targetPlanet = set;}
		bool isInhabitedPlanet() {return m_inhabitedPlanet;}
		void setInhabitedPlanet(bool set) {m_inhabitedPlanet=set;}

		void setSystemVisited() {m_systemsVisited.insert(std::atoi(m_system.c_str()));}
		bool isSystemVisited()	{return m_systemsVisited.find(std::atoi(m_system.c_str())) != m_systemsVisited.end();}

		void showTargetMessage();
		void hideTargetMessage();

		void resetGame();

		void activateHUD(CEGUI::Window* window);
		void deactivateHUD();
		void fadeOutHUD();

		/// TODO -- Serialization/deserialization --- ///

	private:

		static CGameManager* m_instance;

		std::string m_system;
		std::string m_planet;

		CGeneralNotification* m_notificationGUI;

		typedef std::map<std::string,unsigned int> TPlayerStateMap;
		TPlayerStateMap m_stateMap;

		
		TActiveEquipment m_equipmentMap;

		typedef std::map<std::string,unsigned int> TCrewMembers;
		TCrewMembers m_crewMembersMap;

		TCargo m_cargoMap;

		//The int is an identifier for the mission, but its not used yet
		TMission m_activeMission;

		typedef std::map<std::string,std::pair<int, int>> TEngineData;
		TEngineData m_engineDataMap;
		
		TEquippedEngineData m_eqEngineDat;
		std::string m_eqEngine;

		void changeEquippedEngine(const std::string& name);

		int m_life;
        unsigned m_totalLife; // ??
        unsigned m_shield;
        unsigned m_tmpShield;
		unsigned int m_shieldRegen;
        unsigned m_energy;
        unsigned m_tmpEnergy;
		unsigned int m_energyRegen;
        unsigned m_objectives;
        unsigned m_totalObjectives;
        unsigned m_nameRepeatCounter;

		bool m_targetSystem, m_targetPlanet, m_inhabitedPlanet;

		typedef std::set<int> TSystemsVisited;
		TSystemsVisited m_systemsVisited;

		CGameManager();
		~CGameManager();

		void registerInitialResources();

		// HUD Menu
		CEGUI::Window* m_menuWindow;

		CEGUI::AnimationInstance* m_blendInAnimInstance;
		CEGUI::AnimationInstance* m_blendOutAnimInstance;

		bool initHUD();
		void setHUDStringData();

		void setupEvents();
		void setupAnimations();
		bool onSelectionChanged(const CEGUI::EventArgs &e);
		bool onEquipButtonClicked(const CEGUI::EventArgs &e);
	};
}


#endif