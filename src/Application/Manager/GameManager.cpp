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

#include "GameManager.h"

#include <log.h>
#include <cassert>

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/widgets/ItemListbox.h>
#include <CEGUI/widgets/Listbox.h>
#include <CEGUI/widgets/ListboxItem.h>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <CEGUI/widgets/PushButton.h>
#include <CEGUI/AnimationManager.h>
#include <CEGUI/Animation.h>
#include <CEGUI/AnimationInstance.h>

#include "../../Common/Sound/Sound.h"
#include "../../Common/Data/Game_Constants.h"
#include "../States/State GUI/PlanetGUI.h"
#include "../States/State GUI/EventGUI.h"
#include "../States/State GUI/GeneralNotification.h"


namespace Application
{
	const char* const LOG_CGAMEMGR = "Application::CGameManager";
	CGameManager* CGameManager::m_instance = nullptr;

	const int BASE_LIFE = 100;
	const int BASE_ENERGY = 100;
	const int BASE_SHIELD = 100;

	const int NUM_TARGETS = 4;

	const int SHIELD_INCREMENT = 5;
	const int ENERGY_INCREMENT = 5;

	CGameManager::CGameManager():m_system(""), m_planet(""),m_totalLife(BASE_LIFE),m_life(BASE_LIFE),m_shield(BASE_SHIELD),
		m_objectives(0),m_totalObjectives(NUM_TARGETS),m_tmpShield(BASE_SHIELD),m_energy(BASE_ENERGY),m_tmpEnergy(BASE_ENERGY),m_menuWindow(nullptr),
		m_nameRepeatCounter(0),m_targetSystem(false), m_targetPlanet(false), m_inhabitedPlanet(false),m_notificationGUI(nullptr)
	{
		m_instance = this;
		m_activeMission.first = 0;
		m_activeMission.second = "";
		m_notificationGUI = CGeneralNotification::getInstance();
		registerInitialResources();
		initHUD();
		//Sounds & music
		Common::Sound::CSound::getSingletonPtr()->addSound("ButtonSound1.wav","selectItem");
		Common::Sound::CSound::getSingletonPtr()->addSound("ButtonHover1.wav","equipButton");

	}
	
	CGameManager::~CGameManager()
	{
		m_stateMap.clear();
		m_equipmentMap.clear();
		m_crewMembersMap.clear();
		Common::Sound::CSound::getSingletonPtr()->releaseSound("selectItem");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("equipButton");
		m_instance = 0;
	}

	bool CGameManager::init()
	{
		assert(!m_instance && "Second initialization of Application::CGameManager is not allowed!");

		log_trace(LOG_CGAMEMGR,"Init\n");

		CGeneralNotification::init();

		new CGameManager();

		return true;
	}

	bool CGameManager::initHUD()
	{
		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("HUDMenu.layout");

		if (!m_menuWindow) {
            log_error(LOG_CGAMEMGR, "Error loading layout");
			return false;
        }

		setHUDStringData();

		for(auto it=m_equipmentMap.begin(); it!=m_equipmentMap.end(); ++it){

			if(it->first == Common::Data::Game::GAME_PRIMARY_WEAPON){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(1)->appendText(
					"  " + it->second.first);
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(1)->setUserString(
					Common::Data::Game::GAME_HUD_DESCRIPTION, it->second.second);
			}else if(it->first == Common::Data::Game::GAME_SECONDARY_WEAPON){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(2)->appendText(
					"  " + it->second.first);
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(2)->setUserString(
					Common::Data::Game::GAME_HUD_DESCRIPTION, it->second.second);
			}else if(it->first == Common::Data::Game::GAME_ENGINE){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(3)->appendText(
					"  " + it->second.first);
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(3)->setUserString(
					Common::Data::Game::GAME_HUD_DESCRIPTION, it->second.second);
			}else if(it->first == Common::Data::Game::GAME_SENSORS){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(4)->appendText(
					"  " + it->second.first);
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(4)->setUserString(
					Common::Data::Game::GAME_HUD_DESCRIPTION, it->second.second);
			}else{
				CEGUI::Window* item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->createChild("TaharezLook/ListboxItem",it->second.first);
				item->setFont("Jura-10");
				item->setText(it->first);
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION,it->second.second);
			}

		}
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->endInitialisation();

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->appendText(" " + std::to_string(m_life) + "/" +std::to_string(m_totalLife));
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->appendText(" " + std::to_string(m_shield));
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->appendText(" " + std::to_string(m_energy));
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->endInitialisation();

		for(auto it = m_crewMembersMap.begin(); it != m_crewMembersMap.end(); ++it){
			if(it->first == Common::Data::Game::GAME_MILITARY){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(1)->
					appendText(" " + std::to_string(it->second));
			}else if(it->first == Common::Data::Game::GAME_ENGINEERS){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(2)->
					appendText(" " + std::to_string(it->second));
			}else if(it->first == Common::Data::Game::GAME_SCIENTIFICS){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(3)->
					appendText(" " + std::to_string(it->second));
			}
		}
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->endInitialisation();

		for(auto it = m_stateMap.begin(); it != m_stateMap.end(); ++it){

			if(it->first == Common::Data::Game::GAME_FUEL){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(1)->
					appendText(" " + std::to_string(it->second));
			}else if(it->first == Common::Data::Game::GAME_ORE){
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(2)->
					appendText(" " + std::to_string(it->second));
			}
		}
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->endInitialisation();

		for(auto it = m_cargoMap.begin(); it != m_cargoMap.end(); ++it){
			CEGUI::Window* item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->createChild("TaharezLook/ListboxItem",it->second.first);
				item->setFont("Jura-10");
				item->setText(">>" + it->second.first);
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, it->second.second);
		}

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->endInitialisation();

		if(m_activeMission.first != 0){
			CEGUI::Window* item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->createChild("TaharezLook/ListboxItem",m_activeMission.second);
				item->setFont("Jura-10");
				item->setText("->" + m_activeMission.second);
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, m_activeMission.second);
		}else{
			CEGUI::Window* item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->createChild("TaharezLook/ListboxItem",m_activeMission.second);
				item->setFont("Jura-10");
				item->setText(m_activeMission.second);
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, m_activeMission.second);
		}

		CEGUI::Window* item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->createChild("TaharezLook/ListboxItem", Common::Data::Game::GAME_MISSION_NAME);
		item->setFont("Jura-10");
		item->setText("=== " + std::string(Common::Data::Game::GAME_MISSION_TEXT) + std::to_string(m_objectives) + "/" + std::to_string(m_totalObjectives) + " ===");
		item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_MISSION_TEXT);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->endInitialisation();

		setupEvents();
		setupAnimations();

		/// This is just for testing ///

		addCrewMemberByName(Common::Data::Game::GAME_MILITARY);
		addCrewMemberByName(Common::Data::Game::GAME_ENGINEERS,2);
		addCrewMemberByName(Common::Data::Game::GAME_SCIENTIFICS);

		return true;
	}

	void CGameManager::setHUDStringData()
	{
		//Equipment
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(0)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION,Common::Data::Game::GAME_HUD_EQUIPMENT);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->getItemFromIndex(5)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION,Common::Data::Game::GAME_HUD_OTHERS);

		//State
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(0)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION,Common::Data::Game::GAME_HUD_SHIP_STATE);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION,Common::Data::Game::GAME_HULL);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION,Common::Data::Game::GAME_SHIELD);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION,Common::Data::Game::GAME_ENERGY);

		//Crew
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(0)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_HUD_CREW);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(1)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_MILITARY);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(2)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_ENGINEERS);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(3)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_SCIENTIFICS);

		//Resources
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(0)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_HUD_RESOURCES);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(1)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_FUEL);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(2)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_ORE);

		//Cargo
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->getItemFromIndex(0)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_HUD_CARGO);

		//Info
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->getItemFromIndex(0)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_HUD_INFO);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->getItemFromIndex(1)->setUserString(
			Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_HUD_NOTES);

	}

	void CGameManager::setupEvents()
	{
		m_menuWindow->getChild("LeftWindow/EquipmentBoard")
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::SubscriberSlot(&CGameManager::onSelectionChanged , this));
		m_menuWindow->getChild("LeftWindow/StateBoard")
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::SubscriberSlot(&CGameManager::onSelectionChanged , this));
		m_menuWindow->getChild("LeftWindow/CrewBoard")
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::SubscriberSlot(&CGameManager::onSelectionChanged , this));
		m_menuWindow->getChild("RightWindow/ResourcesBoard")
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::SubscriberSlot(&CGameManager::onSelectionChanged , this));
		m_menuWindow->getChild("RightWindow/CargoBoard")
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::SubscriberSlot(&CGameManager::onSelectionChanged , this));
		m_menuWindow->getChild("RightWindow/InformationBoard")
			->subscribeEvent(CEGUI::Listbox::EventSelectionChanged,CEGUI::SubscriberSlot(&CGameManager::onSelectionChanged , this));
		m_menuWindow->getChild("RightWindow/EquipButton")
			->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::SubscriberSlot(&CGameManager::onEquipButtonClicked , this));
	}

	void CGameManager::setupAnimations()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* fadeOutAnim = animMgr.getAnimation("BlackWindowFadeOut");
		m_blendOutAnimInstance = animMgr.instantiateAnimation(fadeOutAnim);
		m_blendOutAnimInstance->setTargetWindow(m_menuWindow);

		CEGUI::Animation* fadeInAnim = animMgr.getAnimation("BlackWindowFadeIn");
		m_blendInAnimInstance = animMgr.instantiateAnimation(fadeInAnim);
		m_blendInAnimInstance->setTargetWindow(m_menuWindow);
	}

	void CGameManager::activateHUD(CEGUI::Window* window)
	{
		window->addChild(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();
		m_notificationGUI->activate(m_menuWindow);
		m_notificationGUI->showTutorialMessage();
		m_blendInAnimInstance->start();
	}

	void CGameManager::deactivateHUD()
	{
		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);

		m_notificationGUI->deactivate(m_menuWindow);

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("LeftWindow/InfoBoard"))->resetList();
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("RightWindow/DescriptionBoard"))->resetList();

	}

	void CGameManager::fadeOutHUD()
	{
		m_blendOutAnimInstance->start();
	}

	void CGameManager::release()
	{
		assert(m_instance && "Application::CGameManager hasn't been initialized!");

		log_trace(LOG_CGAMEMGR,"Release\n");

		CGeneralNotification::release();

		if(m_instance)
		{
			delete m_instance;
		}
	}

	int CGameManager::getPlayerResourceByName(const std::string& str)
	{		
		if(m_stateMap.count(str) > 0){
			return m_stateMap[str];
		}

		return -1;
	}

	int CGameManager::getCrewMembersByName(const std::string& str)
	{
		if(m_crewMembersMap.count(str) > 0){
			return m_crewMembersMap[str];
		}

		return -1;
	}

	bool CGameManager::addResourceByName(const std::string& str, unsigned int num)
	{
		if(m_stateMap.count(str) > 0){

			m_stateMap[str] += num;

			for(int i=1; i<static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemCount(); ++i){
				std::string tmp(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(i)->getText().c_str());
				tmp = tmp.substr(0,tmp.find(':')+1);
				if(tmp == str){
					static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(i)->setText(tmp + " " + std::to_string(m_stateMap[str]));
				}
			}

			return true;
		}

		return false;
	}

	bool CGameManager::decreaseResourceByName(const std::string& str, unsigned int num)
	{
		if(m_stateMap.count(str) > 0 && (m_stateMap[str] - num) >= 0){

			if(num > m_stateMap[str])
				m_stateMap[str] = 0;
			else
				m_stateMap[str] -= num;

			if(str == Common::Data::Game::GAME_ENGINEERS){
				m_shield = BASE_SHIELD + m_crewMembersMap[str]*SHIELD_INCREMENT;
				m_tmpShield = m_shield;

				std::string tmpStr(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->getText().c_str());
				tmpStr = tmpStr.substr(0,tmpStr.find(":")+1);
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->setText(tmpStr + " " 
					+ std::to_string(m_shield));
			}

			if(str == Common::Data::Game::GAME_SCIENTIFICS){
				m_energy = BASE_ENERGY + m_crewMembersMap[str]*ENERGY_INCREMENT;
				m_tmpEnergy = m_energy;

				std::string tmpStr(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->getText().c_str());
				tmpStr = tmpStr.substr(0,tmpStr.find(":")+1);
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->setText(tmpStr + " " 
					+ std::to_string(m_energy));
			}

			for(int i=1; i<static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemCount(); ++i){
				std::string tmp(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(i)->getText().c_str());
				tmp = tmp.substr(0,tmp.find(':')+1);
				if(tmp == str){
					static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/ResourcesBoard"))->getItemFromIndex(i)->setText(tmp + " " + std::to_string(m_stateMap[str]));
				}
			}

			return true;
		}

		return false;
	}

	bool CGameManager::addCrewMemberByName(const std::string& str, unsigned int num)
	{
		if(m_crewMembersMap.count(str) > 0){

			m_crewMembersMap[str] += num;

			if(str == Common::Data::Game::GAME_ENGINEERS){
				m_shield = BASE_SHIELD + m_crewMembersMap[str]*SHIELD_INCREMENT;
				m_tmpShield = m_shield;

				std::string tmpStr(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->getText().c_str());
				tmpStr = tmpStr.substr(0,tmpStr.find(":")+1);
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->setText(tmpStr + " " 
					+ std::to_string(m_shield));
			}

			if(str == Common::Data::Game::GAME_SCIENTIFICS){
				m_energy = BASE_ENERGY + m_crewMembersMap[str]*ENERGY_INCREMENT;
				m_tmpEnergy = m_energy;

				std::string tmpStr(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->getText().c_str());
				tmpStr = tmpStr.substr(0,tmpStr.find(":")+1);
				static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->setText(tmpStr + " " 
					+ std::to_string(m_energy));
			}

			for(int i=1; i<static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemCount(); ++i){
				std::string tmp(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(i)->getText().c_str());
				tmp = tmp.substr(0,tmp.find(':')+1);
				if(tmp == str){
					static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(i)->setText(tmp + " " + std::to_string(m_crewMembersMap[str]));
				}
			}

			return true;
		}

		return false;
	}

	bool CGameManager::subtractCrewMemberByName(const std::string& str, unsigned int num)
	{
		if(m_crewMembersMap.count(str) > 0 && (m_crewMembersMap[str] - num) >= 0){

			m_crewMembersMap[str] -= num;

			for(int i=1; i<static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemCount(); ++i){
				std::string tmp(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(i)->getText().c_str());
				tmp = tmp.substr(0,tmp.find(':')+1);
				if(tmp == str){
					static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/CrewBoard"))->getItemFromIndex(i)->setText(tmp + " " + std::to_string(m_crewMembersMap[str]));
				}
			}

			return true;
		}

		return false;
	}

	typedef std::multimap<std::string,std::pair<std::string,std::string>> TCargo;
	void CGameManager::addToCargo(const std::string& key, const std::string& name, const std::string& desc)
	{
		m_cargoMap.insert(std::pair<std::string,std::pair<std::string,std::string>>(
			key,std::pair<std::string,std::string>(name, desc)));
		CEGUI::Window* item;
		try{
			item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->createChild("TaharezLook/ListboxItem",name);
		}catch(CEGUI::Exception &e){
			++m_nameRepeatCounter;
			item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->
				createChild("TaharezLook/ListboxItem",name+std::to_string(m_nameRepeatCounter));
		}
		item->setFont("Jura-10");
		item->setText(">>" + name);
		item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, desc);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->endInitialisation();
	}

	bool CGameManager::removeFromCargo(const std::string& name)
	{
		for(auto it=m_cargoMap.begin(); it != m_cargoMap.end(); ++it){
			if(it->second.first == name){
				m_cargoMap.erase(it);

				for(int i = 1; i<static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->getItemCount();++i){
					std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->getItemFromIndex(i)->getText().c_str());
					if(str == ">>"+name){
						static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->removeItem(
							static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->getItemFromIndex(i)
						);
						return true;
					}
				}

				break;
			}
		}

		return false;
	}

	void CGameManager::setMission(unsigned int id, const std::string& str, const std::string& desc)
	{
		m_activeMission = std::pair<unsigned int, std::string>(id, str);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->getItemFromIndex(2)->setText(str);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->getItemFromIndex(2)->
			setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION,desc);

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->endInitialisation();
	}

	void CGameManager::increaseLife(unsigned int num)
	{
		m_life += num;

		if(m_life > m_totalLife)
			m_life = m_totalLife;

		std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->getText().c_str());
		str = str.substr(0,str.find(":")+1);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->setText(str + " " 
			+ std::to_string(m_life) + "/" + std::to_string(m_totalLife));
	}

	void CGameManager::decreaseLife(unsigned int num)
	{
		if(num > m_life)
		{
			m_life = 0;
			CPlanetGUI::getInstance()->shipDestroyed();

		}else{
			m_life -= num;
		}

		std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->getText().c_str());
		str = str.substr(0,str.find(":")+1);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->setText(str + " " 
			+ std::to_string(m_life) + "/" + std::to_string(m_totalLife));
	}

	void CGameManager::increaseTotalLife(unsigned int num)
	{
		m_totalLife += num;

		std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->getText().c_str());
		str = str.substr(0,str.find(":")+1);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->setText(str + " " 
			+ std::to_string(m_life) + "/" + std::to_string(m_totalLife));
	}

	void CGameManager::decreaseTotalLife(unsigned int num)
	{
		if(num > m_totalLife)
			m_totalLife = 0;
		else
			m_totalLife -= num;

		std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->getText().c_str());
		str = str.substr(0,str.find(":")+1);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(1)->setText(str + " " 
			+ std::to_string(m_life) + "/" + std::to_string(m_totalLife));
	}

	void CGameManager::increaseShield(unsigned int num)
	{
		m_shield += num;

		std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->getText().c_str());
		str = str.substr(0,str.find(":")+1);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->setText(str + " " 
			+ std::to_string(m_shield));
	}

	void CGameManager::decreaseShield(unsigned int num)
	{
		if(num > m_shield)
			m_shield = 0;
		else
			m_shield -= num;

		std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->getText().c_str());
		str = str.substr(0,str.find(":")+1);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(2)->setText(str + " " 
			+ std::to_string(m_shield));
	}

	void CGameManager::increaseEnergy(unsigned int num)
	{
		m_energy += num;

		std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->getText().c_str());
		str = str.substr(0,str.find(":")+1);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->setText(str + " " 
			+ std::to_string(m_energy));
	}
	
	void CGameManager::decreaseEnergy(unsigned int num)
	{
		if(num > m_energy)
			m_energy = 0;
		else
			m_energy -= num;

		std::string str(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->getText().c_str());
		str = str.substr(0,str.find(":")+1);
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/StateBoard"))->getItemFromIndex(3)->setText(str + " " 
			+ std::to_string(m_energy));
	}

	typedef std::map<std::string,std::pair<int, int>> TEngineData;
	void CGameManager::addEngineData(const std::string& name, int maxDist, int cons)
	{
		m_engineDataMap[name] = std::pair<int, int>(maxDist, cons);
	}

	void CGameManager::changeEquippedEngine(const std::string& name)
	{
		m_eqEngineDat = m_engineDataMap[name];
	}

	void CGameManager::objectiveAquired()
	{
		m_objectives++;

		if(m_objectives >= m_totalObjectives)
			CEventGUI::getInstance()->finishGame();

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->getItemFromIndex(3)->setText(
			"=== " + std::string(Common::Data::Game::GAME_MISSION_TEXT) + std::to_string(m_objectives) + "/" + std::to_string(m_totalObjectives) + " ==="
			);

	}

	void CGameManager::registerInitialResources()
	{
		//This is just an example of how player resources could be initialised
		m_stateMap[Common::Data::Game::GAME_FUEL] = 500;
		m_stateMap[Common::Data::Game::GAME_ORE] = 200;
		//m_stateMap[Common::Data::Game::GAME_TIME] = 200;

		m_equipmentMap[Common::Data::Game::GAME_PRIMARY_WEAPON] = std::pair<std::string,std::string>(
			Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[0][0], Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[0][1]);
		m_equipmentMap[Common::Data::Game::GAME_SECONDARY_WEAPON] = std::pair<std::string,std::string>(
			Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[0][0], Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[0][1]);
		m_equipmentMap[Common::Data::Game::GAME_ENGINE] = std::pair<std::string,std::string>(
			Common::Data::Game::GAME_ENGINES_LIST[0][0], Common::Data::Game::GAME_ENGINES_LIST[0][1]);

		addEngineData(Common::Data::Game::ENGINE_DATA_MAP[0].first,Common::Data::Game::ENGINE_DATA_MAP[0].second.first,
			Common::Data::Game::ENGINE_DATA_MAP[0].second.second);

		m_equipmentMap[Common::Data::Game::GAME_SENSORS] = std::pair<std::string,std::string>(
			Common::Data::Game::GAME_SENSORS_LIST[0][0], Common::Data::Game::GAME_SENSORS_LIST[0][1]);
		m_equipmentMap[Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[0][0]] = std::pair<std::string,std::string>(
			Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[0][1], Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[0][2]);

		m_crewMembersMap[Common::Data::Game::GAME_MILITARY] = 0;
		m_crewMembersMap[Common::Data::Game::GAME_ENGINEERS] = 0;
		m_crewMembersMap[Common::Data::Game::GAME_SCIENTIFICS] = 0;

		m_cargoMap.insert(
			std::pair<std::string,std::pair<std::string,std::string>>(
			Common::Data::Game::GAME_PRIMARY_WEAPON, std::pair<std::string,std::string>(
			Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[1][0],Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[1][1])));

		m_cargoMap.insert(
			std::pair<std::string,std::pair<std::string,std::string>>(
			Common::Data::Game::GAME_SECONDARY_WEAPON, std::pair<std::string,std::string>(
			Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[1][0], Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[1][1])));

		m_cargoMap.insert(
			std::pair<std::string,std::pair<std::string,std::string>>(
			Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[1][0], std::pair<std::string,std::string>(
			Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[1][1], Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[1][2])));

		m_cargoMap.insert(
			std::pair<std::string,std::pair<std::string,std::string>>(
			Common::Data::Game::GAME_ENGINE, std::pair<std::string,std::string>(
			Common::Data::Game::GAME_ENGINES_LIST[1][0], Common::Data::Game::GAME_ENGINES_LIST[1][1])));

		addEngineData(Common::Data::Game::ENGINE_DATA_MAP[1].first, Common::Data::Game::ENGINE_DATA_MAP[1].second.first
			, Common::Data::Game::ENGINE_DATA_MAP[1].second.second);

		// Just for testing
		m_activeMission = std::pair<int,std::string>(0," ");

		changeEquippedEngine(Common::Data::Game::ENGINE_DATA_MAP[0].first);
	}

	void CGameManager::resetGame()
	{
		m_stateMap.clear();
		m_equipmentMap.clear();
		m_crewMembersMap.clear();
		m_cargoMap.clear();
		m_engineDataMap.clear();
		m_systemsVisited.clear();
		m_menuWindow->destroy();
		m_totalLife = m_life = BASE_LIFE;
		m_shield = m_tmpShield = BASE_SHIELD;
		m_energy = m_tmpEnergy = BASE_ENERGY;
		m_objectives = 0;
		m_targetPlanet = false;
		m_targetSystem = false;
		m_inhabitedPlanet = false;
		registerInitialResources();
		initHUD();
		m_system = "";
		m_planet = "";
		m_nameRepeatCounter = 0;
		//m_notificationGUI->resetTutorial();//Uncoment this to allow the tutorial to be shown every new game
	}

	bool CGameManager::onSelectionChanged(const CEGUI::EventArgs &e)
	{
		const CEGUI::WindowEventArgs& mouseArgs = static_cast<const CEGUI::WindowEventArgs&>(e);

		CEGUI::ItemListbox* boardWindow = static_cast<CEGUI::ItemListbox*>(mouseArgs.window);
		CEGUI::String parentDir = boardWindow->getParent()->getName();

		for(int i = 2; i < 6; ++i){
			if(boardWindow != static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild(parentDir)->getChildAtIdx(i))){
				if(i!=5){
					if(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild(parentDir)->getChildAtIdx(i))->getFirstSelectedItem() != 0)
						static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild(parentDir)->getChildAtIdx(i))->getFirstSelectedItem()->setSelected(false);
				}
				if(i==5 && boardWindow->getFirstSelectedItem()){
					static_cast<CEGUI::Listbox*>(m_menuWindow->getChild(parentDir)->getChildAtIdx(i))->resetList();
					if(parentDir == Common::Data::Game::GAME_HUD_LEFT_WINDOW){
						static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("LeftWindow/InfoBoard"))->addItem(
							new CEGUI::ListboxTextItem(boardWindow->getFirstSelectedItem()->getUserString(Common::Data::Game::GAME_HUD_DESCRIPTION)));
					}else if(parentDir == Common::Data::Game::GAME_HUD_RIGHT_WINDOW){
						m_menuWindow->getChild("RightWindow/EquipButton")->disable();
						static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("RightWindow/DescriptionBoard"))->addItem(
							new CEGUI::ListboxTextItem(boardWindow->getFirstSelectedItem()->getUserString(Common::Data::Game::GAME_HUD_DESCRIPTION)));
						for(auto it = m_cargoMap.begin(); it != m_cargoMap.end(); ++it){
							if(boardWindow->getFirstSelectedItem()->getText() == ">>" + it->second.first && it->first != Common::Data::Game::GAME_ITEM)
								m_menuWindow->getChild("RightWindow/EquipButton")->enable();
						}
					}
				}
			}
		}

		Common::Sound::CSound::getSingletonPtr()->playSound("selectItem");

		return true;
	}

	bool CGameManager::onEquipButtonClicked(const CEGUI::EventArgs &e)
	{
		CEGUI::Window* item(static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->getFirstSelectedItem());

		for(auto it = m_cargoMap.begin(); it != m_cargoMap.end(); ++it){
			if(item->getText() == ">>" + it->second.first){
				if(it->first == Common::Data::Game::GAME_PRIMARY_WEAPON || it->first == Common::Data::Game::GAME_SECONDARY_WEAPON ||
					it->first == Common::Data::Game::GAME_ENGINE || it->first == Common::Data::Game::GAME_SENSORS){
					std::pair<std::string,std::string> tmp(m_equipmentMap[it->first]);
					m_equipmentMap[it->first] = it->second;
					it->second = tmp;
					item->setText(">>" + tmp.first);
					item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, tmp.second);

					int idx;
					if(it->first == Common::Data::Game::GAME_PRIMARY_WEAPON) idx = 1; else
					if(it->first == Common::Data::Game::GAME_SECONDARY_WEAPON) idx = 2; else
					if(it->first == Common::Data::Game::GAME_ENGINE) idx = 3; else
					if(it->first == Common::Data::Game::GAME_SENSORS) idx = 4;
				
					std::string tmpText (static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->
						getItemFromIndex(idx)->getText().c_str());
					tmpText = tmpText.substr(0,tmpText.find(":")+1);
					tmpText += " " + m_equipmentMap[it->first].first;
					if(idx == 3)changeEquippedEngine(m_equipmentMap[it->first].first);
					static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))
						->getItemFromIndex(idx)->setText(tmpText);
					static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))
						->getItemFromIndex(idx)->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION,m_equipmentMap[it->first].second);
					static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("RightWindow/DescriptionBoard"))->resetList();
					static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("RightWindow/DescriptionBoard"))->addItem(
						new CEGUI::ListboxTextItem(tmp.second));
					break;
				}else{
					CEGUI::Window* item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))
						->createChild("TaharezLook/ListboxItem",it->second.first);
					item->setFont("Jura-10");
					item->setText(it->first);
					item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION,it->second.second);
					static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("RightWindow/DescriptionBoard"))->resetList();
					m_equipmentMap[it->first] = it->second;
					m_cargoMap.erase(it);
					static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/CargoBoard"))->getFirstSelectedItem()->destroy();
					m_menuWindow->getChild("RightWindow/EquipButton")->disable();

					break;
				}
			}
		}

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("LeftWindow/EquipmentBoard"))->endInitialisation();

		Common::Sound::CSound::getSingletonPtr()->playSound("equipButton");

		return true;
	}

	void CGameManager::showTargetMessage()
	{

		CEGUI::Window* item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->createChild("TaharezLook/ListboxItem","target_message");
				item->setFont("Jura-10");
				/// TODO -- Internationalization -- /// Begin{
				item->setText("!!>>Signal detected in the system");
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION,"An active signal has been\ndetected in this system.");
				/// TODO -- Internationalization -- /// }End

		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->endInitialisation();
	}

	void CGameManager::hideTargetMessage()
	{
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->removeItem(
			static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->getItemFromIndex(4));
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("RightWindow/InformationBoard"))->endInitialisation();
	}

}