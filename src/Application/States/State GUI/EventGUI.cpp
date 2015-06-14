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

#include "EventGUI.h"

#include "../EventState.h"

#include <log.h>
#include <random>
#include <chrono>
#include <ctime>

#include "../../Manager/MissionManager.h"
#include "../../Manager/GameManager.h"
#include "../../../Common/Data/Game_Constants.h"
#include "../../../Common/Sound/Sound.h"
#include "MissionNotification.h"

#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/AnimationManager.h>
#include <CEGUI/AnimationInstance.h>
#include <CEGUI/widgets/Listbox.h>
#include <CEGUI\widgets\ListboxTextItem.h>
#include <CEGUI/widgets/ItemListbox.h>
#include <CEGUI/widgets/ListboxItem.h>
#include <CEGUI\widgets\PushButton.h>

namespace Application
{
	const char* const LOG_CEVENTGUI = "Application::CEventGUI";
	CEventGUI* CEventGUI::m_instance = nullptr;

	const char* const BACKGROUND_IMAGES[] = {"DesertPlanet/full_image","LavaPlanet/full_image","RedPlanet/full_image","EarthLikePlanet/full_image",
												"OceanPlanet/full_image","GasPlanet/full_image","IcePlanet/full_image","PurplePlanet/full_image",
												"DesertPlanetInhabited/full_image","LavaPlanetInhabited/full_image","RedPlanetInhabited/full_image","EarthLikePlanetInhabited/full_image",
												"OceanPlanetInhabited/full_image","GasPlanetInhabited/full_image","IcePlanetInhabited/full_image","PurplePlanetInhabited/full_image"
											};

	CEventGUI::CEventGUI(CEventState* state) : m_eventState(state), m_menuWindow(nullptr), m_mgrInstance(nullptr), m_missionManager(nullptr),
		m_topBarAnimInst(nullptr), m_botBarAnimInst(nullptr), m_insideAnimInst(nullptr), m_windowAnimInst(nullptr), m_inhabitedMenuWindow(nullptr),
		m_unoccupiedMenuWindow(nullptr), m_fadeAnimInst(nullptr), m_eventPageInAnimInst(nullptr), m_docksPageInAnimInst(nullptr), m_cityPageInAnimInst(nullptr),
		m_topBarAnimInstInhabited(nullptr), m_botBarAnimInstInhabited(nullptr), m_insideAnimInstInhabited(nullptr),
		m_fadeAnimInstInhabited(nullptr), m_windowAnimInstInhabited(nullptr), eventFired(false)
	{
		m_instance = this;

		m_mgrInstance = CGameManager::getInstance();
		m_missionManager = CMissionManager::getInstance();

		m_inhabitedMenuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("InhabitedPlanetEvent.layout");
		m_unoccupiedMenuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("UnoccupiedPlanetEvent.layout");

		setupEvents();
		setupAnimations();

		//Sounds & music
		Common::Sound::CSound::getSingletonPtr()->addSound("Button2.wav","backButton");
		Common::Sound::CSound::getSingletonPtr()->addSound("Intro1.wav","introEventSound");
	}

	CEventGUI::~CEventGUI()
	{
		Common::Sound::CSound::getSingletonPtr()->releaseSound("backButton");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("introEventSound");
		m_instance = nullptr;
	}

	bool CEventGUI::init(CEventState* state)
	{
		assert(!m_instance && "Second initialization of Application::CPlanetGUI is not allowed!");

		log_trace(LOG_CEVENTGUI,"Init\n");

		new CEventGUI(state);

		return true;
	}

	void CEventGUI::release()
	{
		log_trace(LOG_CEVENTGUI,"Release\n");

		if(m_instance)
		{
			delete m_instance;
		}
	}

	void CEventGUI::activate()
	{
		log_trace(LOG_CEVENTGUI,"Activate\n");

		if(m_mgrInstance->isInhabitedPlanet())
			m_menuWindow = m_inhabitedMenuWindow;
		else
			m_menuWindow = m_unoccupiedMenuWindow;

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();

		if(!eventFired)
			setupHUD();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

		CMissionNotification::getInstance()->activate(m_menuWindow);

		//Missions
		bool check(m_mgrInstance->isTargetPlanet());
		if(m_mgrInstance->isTargetPlanet() && !m_mgrInstance->isSystemVisited()){
			m_mgrInstance->setSystemVisited();
			CMissionNotification::getInstance()->showNote(true);
			m_mgrInstance->objectiveAquired();
		}

		m_missionManager->checkPlanet();

		onActivateAnims();

		//Sound
		Common::Sound::CSound::getSingletonPtr()->playDelayedSound("introEventSound",400);

		eventFired = true;
	}

	void CEventGUI::deactivate()
	{
		log_trace(LOG_CEVENTGUI,"Deactivate\n");

		CMissionNotification::getInstance()->deactivate();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
	}

	void CEventGUI::setupAnimations()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* topBarAnim = animMgr.getAnimation("TopBarMoveInAnimationQuick");

		m_topBarAnimInst = animMgr.instantiateAnimation(topBarAnim);
		CEGUI::Window* topBarWindow = m_unoccupiedMenuWindow->getChild("TopBar");
		m_topBarAnimInst->setTargetWindow(topBarWindow);

		m_topBarAnimInstInhabited = animMgr.instantiateAnimation(topBarAnim);
		topBarWindow = m_inhabitedMenuWindow->getChild("TopBar");
		m_topBarAnimInstInhabited->setTargetWindow(topBarWindow);


		CEGUI::Animation* botBarAnim = animMgr.getAnimation("BotBarMoveInAnimationQuick");

		m_botBarAnimInst = animMgr.instantiateAnimation(botBarAnim);
		CEGUI::Window* botBarWindow = m_unoccupiedMenuWindow->getChild("BotBar");
		m_botBarAnimInst->setTargetWindow(botBarWindow);

		m_botBarAnimInstInhabited = animMgr.instantiateAnimation(botBarAnim);
		botBarWindow = m_inhabitedMenuWindow->getChild("BotBar");
		m_botBarAnimInstInhabited->setTargetWindow(botBarWindow);


		CEGUI::Animation* insideAnim = animMgr.getAnimation("InsideBlendInQuick");

		m_insideAnimInst = animMgr.instantiateAnimation(insideAnim);
		CEGUI::Window* insideWindows = m_unoccupiedMenuWindow->getChild("InnerButtonsContainer");
		m_insideAnimInst->setTargetWindow(insideWindows);

		m_insideAnimInstInhabited = animMgr.instantiateAnimation(insideAnim);
		insideWindows = m_inhabitedMenuWindow->getChild("InnerButtonsContainer");
		m_insideAnimInstInhabited->setTargetWindow(insideWindows);

		m_eventPageInAnimInst = animMgr.instantiateAnimation(insideAnim);
		insideWindows = m_unoccupiedMenuWindow->getChild("EventPage");
		m_eventPageInAnimInst->setTargetWindow(insideWindows);

		m_docksPageInAnimInst = animMgr.instantiateAnimation(insideAnim);
		insideWindows = m_inhabitedMenuWindow->getChild("DocksPage");
		m_docksPageInAnimInst->setTargetWindow(insideWindows);

		m_cityPageInAnimInst = animMgr.instantiateAnimation(insideAnim);
		insideWindows = m_inhabitedMenuWindow->getChild("CityPage");
		m_cityPageInAnimInst->setTargetWindow(insideWindows);


		CEGUI::Animation* fadeAnim = animMgr.getAnimation("InsideFadeOutQuick");

		m_fadeAnimInst = animMgr.instantiateAnimation(fadeAnim);
		insideWindows = m_unoccupiedMenuWindow->getChild("InnerButtonsContainer");
		m_fadeAnimInst->setTargetWindow(insideWindows);

		m_fadeAnimInstInhabited = animMgr.instantiateAnimation(fadeAnim);
		insideWindows = m_inhabitedMenuWindow->getChild("InnerButtonsContainer");
		m_fadeAnimInstInhabited->setTargetWindow(insideWindows);

		m_eventPageFadeAnimInst = animMgr.instantiateAnimation(fadeAnim);
		insideWindows = m_unoccupiedMenuWindow->getChild("EventPage");
		m_eventPageFadeAnimInst->setTargetWindow(insideWindows);

		m_docksPageFadeAnimInst = animMgr.instantiateAnimation(fadeAnim);
		insideWindows = m_inhabitedMenuWindow->getChild("DocksPage");
		m_docksPageFadeAnimInst->setTargetWindow(insideWindows);

		m_cityPageFadeAnimInst = animMgr.instantiateAnimation(fadeAnim);
		insideWindows = m_inhabitedMenuWindow->getChild("CityPage");
		m_cityPageFadeAnimInst->setTargetWindow(insideWindows);


		CEGUI::Animation* windowAnim = animMgr.getAnimation("OpenWindow");

		m_windowAnimInst = animMgr.instantiateAnimation(windowAnim);
		CEGUI::Window* buttonWindow1 = m_unoccupiedMenuWindow->getChild("InnerButtonsContainer/LogWindow");
		m_windowAnimInst->setTargetWindow(buttonWindow1);

		m_windowAnimInstInhabited = animMgr.instantiateAnimation(windowAnim);
		buttonWindow1 = m_inhabitedMenuWindow->getChild("InnerButtonsContainer/LogWindow");
		m_windowAnimInstInhabited->setTargetWindow(buttonWindow1);
	}

	void CEventGUI::setupHUD()
	{
		int i(m_mgrInstance->isInhabitedPlanet()?1:0), 
			j(std::atoi(m_mgrInstance->getPlanet().substr(m_mgrInstance->getPlanet().length()-3,1).c_str()));
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("LogWindow")->
			getChildElement("MessageBoard"))->addItem(new CEGUI::ListboxTextItem(CEGUI::String(
				Common::Data::Game::GAME_PLANET_EVENT_INTRO[i*8+j]
			)));

		m_menuWindow->setProperty("Image",BACKGROUND_IMAGES[i*8+j]);
	}

	void CEventGUI::setupEvents()
	{
		//Inhabited
		m_inhabitedMenuWindow->getChildElement("InnerButtonsContainer")->getChildElement("LogWindow")->getChildElement("DocksButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::exploreDocks, this));

		m_inhabitedMenuWindow->getChildElement("InnerButtonsContainer")->getChildElement("LogWindow")->getChildElement("CityButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::searchCity, this));

		m_inhabitedMenuWindow->getChildElement("CityPage")->getChildElement("LogWindow")->getChildElement("RejectButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::onRejectMission, this));

		m_inhabitedMenuWindow->getChildElement("CityPage")->getChildElement("LogWindow")->getChildElement("AcceptButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::onAcceptMission, this));

		m_inhabitedMenuWindow->getChildElement("DocksPage")->getChildElement("ShopWindow")->getChildElement("ItemsBoard")->
		subscribeEvent(CEGUI::Listbox::EventSelectionChanged, 
		CEGUI::SubscriberSlot(&CEventGUI::onItemSelected, this));

		m_inhabitedMenuWindow->getChildElement("DocksPage")->getChildElement("ShopWindow")->getChildElement("BackButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::onBackShop, this));

		m_inhabitedMenuWindow->getChildElement("DocksPage")->getChildElement("ShopWindow")->getChildElement("PurchaseButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::purchase, this));

		//Unoccupied
		m_unoccupiedMenuWindow->getChildElement("InnerButtonsContainer")->getChildElement("LogWindow")->getChildElement("ExploreButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::explorePlanet, this));

		m_unoccupiedMenuWindow->getChildElement("InnerButtonsContainer")->getChildElement("LogWindow")->getChildElement("ResourcesButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::gatherResources, this));

		m_unoccupiedMenuWindow->getChildElement("EventPage")->getChildElement("LogWindow")->getChildElement("BackButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CEventGUI::onBackEvent, this));

	}

	void CEventGUI::onActivateAnims()
	{
		if(m_mgrInstance->isInhabitedPlanet()){

			m_topBarAnimInstInhabited->start();
			m_botBarAnimInstInhabited->start();
			if(!eventFired){
				m_insideAnimInstInhabited->start();
				m_windowAnimInstInhabited->start();
			}

		}else{

			m_topBarAnimInst->start();
			m_botBarAnimInst->start();
			if(!eventFired){
				m_insideAnimInst->start();
				m_windowAnimInst->start();
			}

		}
	}

	bool CEventGUI::explorePlanet(const CEGUI::EventArgs& e)
	{
		setupEvent();
		m_fadeAnimInst->start();
		m_eventPageInAnimInst->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");
		return true;
	}

	bool CEventGUI::gatherResources(const CEGUI::EventArgs& e)
	{
		setupResources();
		m_fadeAnimInst->start();
		m_eventPageInAnimInst->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");
		return true;
	}

	bool CEventGUI::exploreDocks(const CEGUI::EventArgs& e)
	{
		/// TODO --Generate objects--///
		setupShop();
		m_fadeAnimInstInhabited->start();
		m_docksPageInAnimInst->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");
		return true;
	}

	bool CEventGUI::searchCity(const CEGUI::EventArgs& e)
	{
		setupCity();
		m_fadeAnimInstInhabited->start();
		m_cityPageInAnimInst->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");
		return true;
	}

	bool CEventGUI::purchase(const CEGUI::EventArgs& e)
	{
		CEGUI::ItemListbox* box = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChild("DocksPage/ShopWindow/ItemsBoard"));
		CEGUI::Listbox* costList = static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("DocksPage/ShopWindow/CostBoard"));
		CEGUI::Window* item(box->getFirstSelectedItem());
		if(!item)return true;
		int idx(box->getItemIndex(box->getFirstSelectedItem()));
		if(idx == 0)return true;

		std::string str(costList->getListboxItemFromIndex(idx)->getText().c_str());
		int cost(std::atoi(costList->getListboxItemFromIndex(idx)->getText().c_str()));

		if(idx != 1)
		{
			for(auto it = m_shopMap.begin(); it != m_shopMap.end(); ++it){
				if(item->getText() == ">>" + it->second.first){
					if(cost <= m_mgrInstance->getPlayerResourceByName(Common::Data::Game::GAME_ORE)){
						if(!m_mgrInstance->decreaseResourceByName(Common::Data::Game::GAME_ORE, cost)) return true;

						if(it->first != Common::Data::Game::GAME_MILITARY && it->first != Common::Data::Game::GAME_ENGINEERS && 
							it->first != Common::Data::Game::GAME_SCIENTIFICS){
							m_mgrInstance->addToCargo(it->first,it->second.first,it->second.second);
							if(it->first == Common::Data::Game::GAME_ENGINE)
								m_mgrInstance->addEngineData(it->second.first,
								m_engineDataMap[it->second.first].first, m_engineDataMap[it->second.first].second);
						}else{
							m_mgrInstance->addCrewMemberByName(it->first);
						}

						m_shopMap.erase(it);
						item->destroy();
						costList->removeItem(costList->getListboxItemFromIndex(idx));

						static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ResourcesBoard"))->resetList();
						static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ResourcesBoard"))->addItem(
						new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_ORE) 
						+ std::to_string(m_mgrInstance->getPlayerResourceByName(Common::Data::Game::GAME_ORE)) ));

						break;
					}else{
						static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("DocksPage/ShopWindow/DescriptionBoard"))->resetList();
						/// TODO -- Internationalization -- /// Begin{
						static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("DocksPage/ShopWindow/DescriptionBoard"))->addItem(
						new CEGUI::ListboxTextItem("\" Seems that you don't have enough money for that\"")
						);
						/// TODO -- Internationalization -- /// }End
					}
				}
			}
		}else{
			if(cost <= m_mgrInstance->getPlayerResourceByName(Common::Data::Game::GAME_ORE)){
				if(!m_mgrInstance->decreaseResourceByName(Common::Data::Game::GAME_ORE, cost)) return true;
				m_mgrInstance->increaseLife(50);

				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/HullBoard"))->resetList();
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/HullBoard"))->addItem(
				new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_HULL) + std::to_string(m_mgrInstance->getLife()) + "/"
				+ std::to_string(m_mgrInstance->getTotalLife())));

				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ResourcesBoard"))->resetList();
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ResourcesBoard"))->addItem(
				new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_ORE) 
				+ std::to_string(m_mgrInstance->getPlayerResourceByName(Common::Data::Game::GAME_ORE)) ));

			}else{
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("DocksPage/ShopWindow/DescriptionBoard"))->resetList();
				/// TODO -- Internationalization -- /// Begin{
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("DocksPage/ShopWindow/DescriptionBoard"))->addItem(
				new CEGUI::ListboxTextItem("\" Seems that you don't have enough money for that\"")
				);
				/// TODO -- Internationalization -- /// }End
			}
		}

		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");

		return true;
	}

	bool CEventGUI::onItemSelected(const CEGUI::EventArgs& e)
	{
		const CEGUI::WindowEventArgs& mouseArgs = static_cast<const CEGUI::WindowEventArgs&>(e);

		CEGUI::ItemListbox* boardWindow = static_cast<CEGUI::ItemListbox*>(mouseArgs.window);

		if(boardWindow->getFirstSelectedItem()){
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("DocksPage/ShopWindow/DescriptionBoard"))->resetList();
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("DocksPage/ShopWindow/DescriptionBoard"))->addItem(
				new CEGUI::ListboxTextItem(boardWindow->getFirstSelectedItem()->getUserString(Common::Data::Game::GAME_HUD_DESCRIPTION))
				);
		}

		return true;
	}

	bool CEventGUI::onRejectMission(const CEGUI::EventArgs& e)
	{
		m_missionManager->rejectMission();
		m_cityPageFadeAnimInst->start();
		m_eventState->backReleased(CEGUI::EventArgs());
		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");
		return true;
	}

	bool CEventGUI::onAcceptMission(const CEGUI::EventArgs& e)
	{
		m_missionManager->acceptMission();
		m_cityPageFadeAnimInst->start();
		m_eventState->backReleased(CEGUI::EventArgs());
		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");
		return true;
	}

	bool CEventGUI::onBackEvent(const CEGUI::EventArgs& e)
	{
		m_eventPageFadeAnimInst->start();
		m_eventState->backReleased(CEGUI::EventArgs());
		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");
		return true;
	}

	bool CEventGUI::onBackShop(const CEGUI::EventArgs& e)
	{
		m_docksPageFadeAnimInst->start();
		m_eventState->backReleased(CEGUI::EventArgs());
		Common::Sound::CSound::getSingletonPtr()->playSound("backButton");
		return true;
	}

	void CEventGUI::resetGUI()
	{
		static_cast<CEGUI::Listbox*>(m_inhabitedMenuWindow->
			getChildElement("InnerButtonsContainer")->getChildElement("LogWindow")->getChildElement("MessageBoard"))->resetList();
		static_cast<CEGUI::Listbox*>(m_inhabitedMenuWindow->
			getChildElement("DocksPage")->getChildElement("ShopWindow")->getChildElement("ResourcesBoard"))->resetList();
		static_cast<CEGUI::Listbox*>(m_inhabitedMenuWindow->
			getChildElement("DocksPage")->getChildElement("ShopWindow")->getChildElement("HullBoard"))->resetList();
		static_cast<CEGUI::ItemListbox*>(m_inhabitedMenuWindow->
			getChildElement("DocksPage")->getChildElement("ShopWindow")->getChildElement("ItemsBoard"))->resetList();
		static_cast<CEGUI::Listbox*>(m_inhabitedMenuWindow->
			getChildElement("DocksPage")->getChildElement("ShopWindow")->getChildElement("CostBoard"))->resetList();
		static_cast<CEGUI::Listbox*>(m_inhabitedMenuWindow->
			getChildElement("DocksPage")->getChildElement("ShopWindow")->getChildElement("DescriptionBoard"))->resetList();
		static_cast<CEGUI::Listbox*>(m_inhabitedMenuWindow->
			getChildElement("CityPage")->getChildElement("LogWindow")->getChildElement("MessageBoard"))->resetList();
		static_cast<CEGUI::Listbox*>(m_unoccupiedMenuWindow->
			getChildElement("InnerButtonsContainer")->getChildElement("LogWindow")->getChildElement("MessageBoard"))->resetList();
		static_cast<CEGUI::Listbox*>(m_unoccupiedMenuWindow->
			getChildElement("EventPage")->getChildElement("LogWindow")->getChildElement("MessageBoard"))->resetList();

		static_cast<CEGUI::Listbox*>(m_inhabitedMenuWindow->getChildElement("CityPage"))->setVisible(false);
		static_cast<CEGUI::Listbox*>(m_inhabitedMenuWindow->getChildElement("DocksPage"))->setVisible(false);
		static_cast<CEGUI::Listbox*>(m_unoccupiedMenuWindow->getChildElement("EventPage"))->setVisible(false);
	}

	void CEventGUI::setupShop()
	{
		std::time_t seed(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> priceVariance(0,20);
		std::uniform_int_distribution<int>::param_type price(0,20);
		std::uniform_int_distribution<int> numItemDist(0,5);
		std::uniform_int_distribution<int> typeItemDist(0,4);
		std::uniform_int_distribution<int> primaryWeaponDist(1,Common::Data::Game::TOTAL_PRIMARY_WEAPONS - 1);
		std::uniform_int_distribution<int> secondaryWeaponDist(1,Common::Data::Game::TOTAL_SECONDARY_WEAPONS - 1);
		std::uniform_int_distribution<int> engineDist(1,Common::Data::Game::TOTAL_ENGINES - 1);
		std::uniform_int_distribution<int> specialItemDist(0,Common::Data::Game::TOTAL_SPECIAL - 1);
		std::uniform_int_distribution<int> crewDist(0,2);
		std::uniform_int_distribution<int> coinToss(0,1);

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ResourcesBoard"))->addItem(
			new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_ORE) 
			+ std::to_string(m_mgrInstance->getPlayerResourceByName(Common::Data::Game::GAME_ORE)) ));
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/HullBoard"))->addItem(
			new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_HULL) + std::to_string(m_mgrInstance->getLife()) + "/"
			+ std::to_string(m_mgrInstance->getTotalLife())));
		CEGUI::Window* item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ItemsBoard"))->
			createChild("TaharezLook/ListboxItem","Title");
		
		item->setText(Common::Data::Game::GAME_EVENT_SHOP);
		/// TODO -- Internationalization -- /// Begin{
		item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, "\"Need something?\"");
		/// TODO -- Internationalization -- /// }End
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/CostBoard"))->addItem(
			new CEGUI::ListboxTextItem(Common::Data::Game::GAME_EVENT_COST) );

		item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ItemsBoard"))->
			createChild("TaharezLook/ListboxItem","Repair");
		item->setText(Common::Data::Game::GAME_SHOP_REPAIR);
		/// TODO -- Internationalization -- /// Begin{
		item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, "Repair the Hull for a reasonable price.");
		/// TODO -- Internationalization -- /// }End

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/CostBoard"))->addItem(
			new CEGUI::ListboxTextItem(std::to_string(Common::Data::Game::REPAIR_HULL_COST + (10 * priceVariance(generator)) )) );

		/// TODO -- Read random items from file -- //

		int itemNum(numItemDist(generator));

		for(int i = 0; i < itemNum; ++i){

			int itemType(typeItemDist(generator));

			item = static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ItemsBoard"))->
				createChild("TaharezLook/ListboxItem","Item"+std::to_string(i));

			if(itemType == 0){

				int primaryWeapon(primaryWeaponDist(generator));

				if(primaryWeapon > ((Common::Data::Game::TOTAL_PRIMARY_WEAPONS - 1) * 2/3) && m_mgrInstance->getObjectivesAquired() < 2)
					if(coinToss(generator))primaryWeapon -= Common::Data::Game::TOTAL_PRIMARY_WEAPONS/2;

				item->setText(">>"+std::string(Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[primaryWeapon][0]));
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[primaryWeapon][1]);
				price._Max = Common::Data::Game::PRIMARY_WEAPON_COST[primaryWeapon].second.second;
				priceVariance.param(price);
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/CostBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::to_string(Common::Data::Game::PRIMARY_WEAPON_COST[primaryWeapon].second.first + (10 * priceVariance(generator)) )) );

				m_shopMap.insert(std::pair<std::string, std::pair<std::string,std::string>>(Common::Data::Game::GAME_PRIMARY_WEAPON,
					std::pair<std::string,std::string>(Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[primaryWeapon][0],Common::Data::Game::GAME_PRIMARY_WEAPONS_LIST[primaryWeapon][1])));

			}else if(itemType == 1){

				int secondaryWeapon(secondaryWeaponDist(generator));

				if(secondaryWeapon > ((Common::Data::Game::TOTAL_SECONDARY_WEAPONS - 1) * 2/3) && m_mgrInstance->getObjectivesAquired() < 2)
					if(coinToss(generator))secondaryWeapon -= Common::Data::Game::TOTAL_SECONDARY_WEAPONS/2;

				item->setText(">>"+std::string(Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[secondaryWeapon][0]));
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[secondaryWeapon][1]);
				price._Max = Common::Data::Game::SECONDARY_WEAPON_COST[secondaryWeapon].second.second;
				priceVariance.param(price);
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/CostBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::to_string(Common::Data::Game::SECONDARY_WEAPON_COST[secondaryWeapon].second.first + (10 * priceVariance(generator)) )) );

				m_shopMap.insert(std::pair<std::string, std::pair<std::string,std::string>>(Common::Data::Game::GAME_SECONDARY_WEAPON,
					std::pair<std::string,std::string>(Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[secondaryWeapon][0],Common::Data::Game::GAME_SECONDARY_WEAPONS_LIST[secondaryWeapon][1])));

			}else if(itemType == 2){

				int engine(engineDist(generator));

				if(engine > ((Common::Data::Game::TOTAL_ENGINES - 1) * 2/3) && m_mgrInstance->getObjectivesAquired() < 2)
					if(coinToss(generator))engine -= Common::Data::Game::TOTAL_ENGINES/2;

				item->setText(">>"+std::string(Common::Data::Game::GAME_ENGINES_LIST[engine][0]));
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_ENGINES_LIST[engine][1]);
				price._Max = Common::Data::Game::ENGINE_COST[engine].second.second;
				priceVariance.param(price);
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/CostBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::to_string(Common::Data::Game::ENGINE_COST[engine].second.first + (10 * priceVariance(generator)) )) );

				m_shopMap.insert(std::pair<std::string, std::pair<std::string,std::string>>(Common::Data::Game::GAME_ENGINE,
					std::pair<std::string,std::string>(Common::Data::Game::GAME_ENGINES_LIST[engine][0],Common::Data::Game::GAME_ENGINES_LIST[engine][1])));

			}else if(itemType == 3){

				int specialItem(specialItemDist(generator));

				item->setText(">>"+std::string(Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[specialItem][1]));
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[specialItem][2]);
				price._Max = Common::Data::Game::SPECIAL_EQUIPMENT_COST[specialItem].second.second;
				priceVariance.param(price);
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/CostBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::to_string(Common::Data::Game::SPECIAL_EQUIPMENT_COST[specialItem].second.first + (10 * priceVariance(generator)) )) );

				m_shopMap.insert(std::pair<std::string, std::pair<std::string,std::string>>(Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[specialItem][0],
					std::pair<std::string,std::string>(Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[specialItem][1],Common::Data::Game::GAME_SPECIAL_EQUIPMENT_LIST[specialItem][2])));

			}else if(itemType == 4){

				int crewMember(crewDist(generator));

				item->setText(">>"+std::string(Common::Data::Game::GAME_CREW_SHOP_LIST[crewMember][0]));
				item->setUserString(Common::Data::Game::GAME_HUD_DESCRIPTION, Common::Data::Game::GAME_CREW_SHOP_LIST[crewMember][1]);
				price._Max = Common::Data::Game::CREW_SHOP_COST[crewMember].second.second;
				priceVariance.param(price);
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/CostBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::to_string(Common::Data::Game::CREW_SHOP_COST[crewMember].second.first + (10 * priceVariance(generator)) )) );

				m_shopMap.insert(std::pair<std::string, std::pair<std::string,std::string>>(Common::Data::Game::GAME_CREW_TYPES[crewMember],
					std::pair<std::string,std::string>(Common::Data::Game::GAME_CREW_SHOP_LIST[crewMember][0],Common::Data::Game::GAME_CREW_SHOP_LIST[crewMember][1])));

			}

		}

		
		static_cast<CEGUI::ItemListbox*>(m_menuWindow->getChildElement("DocksPage/ShopWindow/ItemsBoard"))->endInitialisation();
	}
	
	void CEventGUI::setupCity()
	{
		///TODO -- get real texts --///

		Application::CMissionManager::TMissionDescriptor mission = m_missionManager->generateMission();

		Application::CMissionManager::TItemDescriptor item = m_missionManager->getTmpItemReward();

		Application::CMissionManager::TResourceDescriptor resource = m_missionManager->getTmpResourceReward();
		/// TODO -- Internationalization -- /// Begin{
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("CityPage/LogWindow/MessageBoard"))->addItem(new CEGUI::ListboxTextItem(
			CEGUI::String("--- Mission ---")));
		/// TODO -- Internationalization -- /// }End
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("CityPage/LogWindow/MessageBoard"))->addItem(new CEGUI::ListboxTextItem(
			std::string(Common::Data::Game::GAME_MISSION_AQUIRED_TEXT[mission.first - 1]) + "\n" + mission.second.second));
		/// TODO -- Internationalization -- /// Begin{1
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("CityPage/LogWindow/MessageBoard"))->addItem(new CEGUI::ListboxTextItem(
			CEGUI::String("--- Reward ---")));
		/// TODO -- Internationalization -- /// }End
		if(item.first != "")
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("CityPage/LogWindow/MessageBoard"))->addItem(new CEGUI::ListboxTextItem(
			">>"+item.second.first));

		if(resource.first != "")
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("CityPage/LogWindow/MessageBoard"))->addItem(new CEGUI::ListboxTextItem(
			resource.first + std::to_string(resource.second)));
	}
	
	void CEventGUI::setupEvent()
	{
		///TODO --More events?--///
		int planetType(std::atoi(m_mgrInstance->getPlanet().substr(m_mgrInstance->getPlanet().length()-3,1).c_str()));
		std::time_t seed(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> eventSelector(1,100);

		int diceRoll1(eventSelector(generator));

		if(diceRoll1 < 31){
			std::uniform_int_distribution<int> eventText(0,Common::Data::Game::MAX_EVENT_NOTHING);
			
			/// TODO -- Internationalization -- /// Begin{
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
			new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_NOTHING_TEXT[planetType][eventText(generator)]) + std::string("\nNothig happens.") ));
			/// TODO -- Internationalization -- /// }End
		}else{
			int diceRoll2(eventSelector(generator));
			if(diceRoll2<51){
				std::uniform_int_distribution<int> resourceVariance(1,50);
				std::uniform_int_distribution<int> eventText(0,Common::Data::Game::MAX_EVENT_VALUABLE);
				int moneyFound(resourceVariance(generator) * 10);
				m_mgrInstance->addResourceByName(Common::Data::Game::GAME_ORE, moneyFound);
				/// TODO -- Internationalization -- /// Begin{
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_VALUABLE_TEXT[planetType][eventText(generator)]) +"\n" + std::to_string(moneyFound) + " scrap gained.") );
				/// TODO -- Internationalization -- /// }End

			}else if(diceRoll2<70){
				std::uniform_int_distribution<int> lifeGain(10,40);
				int gain(lifeGain(generator));
				int lifeLeft(m_mgrInstance->getTotalLife() - m_mgrInstance->getLife());
				m_mgrInstance->increaseLife(gain);
				if(gain > lifeLeft) gain = lifeLeft;
				/// TODO -- Internationalization -- /// Begin{
				if(gain != 0)
				{
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::string("The remainings of a space port still have the necessary machinery to repair the hull\nof your ship.\n" + std::to_string(gain) + "% of the hull repaired.") ));
				}else{
					static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::string("The remainings of a space port still have the necessary machinery to repair the hull\nof your ship, but the hull doesn't need any repairings.") ));
				}
				/// TODO -- Internationalization -- /// }End
			}else if(diceRoll2<76){

				std::uniform_int_distribution<int> crewType(0,2);
				int crewMember(crewType(generator));

				std::string newMember;
				if(crewMember == 0){
					newMember = Common::Data::Game::GAME_MILITARY;
				}else if(crewMember == 1){
					newMember = Common::Data::Game::GAME_ENGINEERS;
				}else if(crewMember == 2){
					newMember = Common::Data::Game::GAME_SCIENTIFICS;
				}

				m_mgrInstance->addCrewMemberByName(newMember);
				/// TODO -- Internationalization -- /// Begin{
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_CREW_TEXT[crewMember]) + std::string("\n+1 " + newMember.substr(2,newMember.length()-3) + ".") ));
				/// TODO -- Internationalization -- /// }End
			}else if(diceRoll2<90){
				int maxDamage(m_mgrInstance->getLife() - 10);
				if(maxDamage < 5) maxDamage = 0;
				if(maxDamage > 30) maxDamage = 30;
				if(maxDamage != 0)
				{
					std::uniform_int_distribution<int> lifeLost(5,m_mgrInstance->getLife() - 10 > 30 ? 30 :m_mgrInstance->getLife() - 10 );
					std::uniform_int_distribution<int> crashText(0,Common::Data::Game::MAX_EVENT_CRASH);
					int damage(lifeLost(generator));
					m_mgrInstance->decreaseLife(damage);
					/// TODO -- Internationalization -- /// Begin{
					static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
						new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_CRASH_TEXT[planetType][crashText(generator)]) + std::string("\n" + std::to_string(damage) + "% of the hull damaged.") ));
				}else{
					std::uniform_int_distribution<int> eventText(0,Common::Data::Game::MAX_EVENT_NOTHING);
					static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_NOTHING_TEXT[planetType][eventText(generator)]) + std::string("\nNothig happens.") ));
				}
					/// TODO -- Internationalization -- /// }End
			}else if(diceRoll2<96){
	
				m_mgrInstance->addToCargo(Common::Data::Game::GAME_ITEM,Common::Data::Game::GAME_ITEM_LIST[planetType + 1][0], Common::Data::Game::GAME_ITEM_LIST[planetType + 1][1]);
				/// TODO -- Internationalization -- /// Begin{
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_ITEM_TEXT[planetType]) + std::string("\n" + std::string(Common::Data::Game::GAME_ITEM_LIST[1][0])+ " added to cargo.") ));
				/// TODO -- Internationalization -- /// }End
			}else{
				std::uniform_int_distribution<int> crewType(0,2);
				int crewMember(crewType(generator));

				std::string newMember;
				if(crewMember == 0){
					newMember = Common::Data::Game::GAME_MILITARY;
				}else if(crewMember == 1){
					newMember = Common::Data::Game::GAME_ENGINEERS;
				}else if(crewMember == 2){
					newMember = Common::Data::Game::GAME_SCIENTIFICS;
				}
				/// TODO -- Internationalization -- /// Begin{
				if(m_mgrInstance->getCrewMembersByName(newMember) > 0){
					m_mgrInstance->subtractCrewMemberByName(newMember);

					static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
						new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_DEATH_TEXT[planetType]) + std::string("\n-1 " + newMember.substr(2,newMember.length()-3) + ".") ));
				}else{
					std::uniform_int_distribution<int> eventText(0,Common::Data::Game::MAX_EVENT_NOTHING);
					static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
					new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_NOTHING_TEXT[planetType][eventText(generator)]) + std::string("\nNothig happens.") ));
				}
				/// TODO -- Internationalization -- /// }End
			}
		}
	}
	
	void CEventGUI::setupResources()
	{
		///TODO --Real texts--///
		int planetType(std::atoi(m_mgrInstance->getPlanet().substr(m_mgrInstance->getPlanet().length()-3,1).c_str()));
		std::time_t seed(std::chrono::system_clock::now().time_since_epoch().count());
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> resourcesSelector(1,100);

		if(resourcesSelector(generator) < 71){
			std::uniform_int_distribution<int> fuelDist(5,30);
			std::uniform_int_distribution<int> eventText(0,Common::Data::Game::MAX_EVENT_FUEL);
			int fuelExtracted = fuelDist(generator) * 10;
			m_mgrInstance->addResourceByName(Common::Data::Game::GAME_FUEL, fuelExtracted);
			/// TODO -- Internationalization -- /// Begin{
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
				new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_FUEL_TEXT[planetType][eventText(generator)]) +std::string("\n" + std::to_string(fuelExtracted) + " fuel units obtained." )));
			/// TODO -- Internationalization -- /// }End

		}else{
			std::uniform_int_distribution<int> moneyDist(1,15);
			std::uniform_int_distribution<int> eventText(0,Common::Data::Game::MAX_EVENT_VALUABLE);
			int oreExtracted = moneyDist(generator) * 10 + 20;
			m_mgrInstance->addResourceByName(Common::Data::Game::GAME_ORE, oreExtracted);
			/// TODO -- Internationalization -- /// Begin{
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChildElement("EventPage/LogWindow/MessageBoard"))->addItem(
				new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_EVENT_VALUABLE_TEXT[planetType][eventText(generator)]) + std::string("\n" + std::to_string(oreExtracted) + " scrap obtained." )));
			/// TODO -- Internationalization -- /// }End
		}
	}

	void CEventGUI::finishGame()
	{
		m_eventState->setGameFinished();
	}
}