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

#include "MissionNotification.h"

#include "../../../Common/Sound/Sound.h"
#include "../../Manager/MissionManager.h"
#include "../../Manager/GameManager.h"

#include <log.h>

#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/AnimationManager.h>
#include <CEGUI/AnimationInstance.h>
#include <CEGUI\widgets\PushButton.h>
#include <CEGUI/widgets/Listbox.h>
#include <CEGUI/widgets/ListboxTextItem.h>

namespace Application
{
	const char* const LOG_CMISSIONNOTE = "Application::CMissionNotification";
	CMissionNotification* CMissionNotification::m_instance = nullptr;

	CMissionNotification::CMissionNotification(): m_menuWindow(nullptr),m_blendIn(nullptr),m_fadeOut(nullptr),m_missionManager(nullptr) 
	{
		m_instance = this;

		m_missionManager = CMissionManager::getInstance();
		m_gameManager = CGameManager::getInstance();

		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("MissionNotification.layout");

		setupEvents();
		setupAnimations();

		//Sounds & music
		Common::Sound::CSound::getSingletonPtr()->addSound("Button2.wav","noteBackButton");
	}

	CMissionNotification::~CMissionNotification()
	{
		Common::Sound::CSound::getSingletonPtr()->releaseSound("noteBackButton");
		m_instance = nullptr;
	}

	bool CMissionNotification::init()
	{
		assert(!m_instance && "Second initialization of Application::CMissionNotification is not allowed!");

		log_trace(LOG_CMISSIONNOTE,"Init\n");

		new CMissionNotification();

		return true;
	}

	void CMissionNotification::release()
	{
		log_trace(LOG_CMISSIONNOTE,"Release\n");

		if(m_instance)
		{
			delete m_instance;
		}
	}

	void CMissionNotification::activate(CEGUI::Window* window)
	{
		log_trace(LOG_CMISSIONNOTE,"Activate\n");

		window->addChild(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();
	}

	void CMissionNotification::deactivate()
	{
		log_trace(LOG_CMISSIONNOTE,"Deactivate\n");

		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->resetList();
	}

	void CMissionNotification::showNote(bool mainMission)
	{
		if(mainMission){
			/// TODO -- Internationalization -- /// Begin{
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->addItem(new CEGUI::ListboxTextItem(
				m_gameManager->getObjectivesAquired()!=m_gameManager->getTotalObjectives() ? CEGUI::String("Signal source found.") : CEGUI::String("All signals checked.")
				));
			/// TODO -- Internationalization -- /// }End
		}else{
			/// TODO -- Internationalization -- /// Begin{
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->addItem(new CEGUI::ListboxTextItem(
				CEGUI::String(">> Mission Completed\n") + (m_missionManager->getStringReward().second == -1 ? 
				CEGUI::String("") : CEGUI::String(std::to_string(m_missionManager->getStringReward().second).c_str())) + 
				(m_missionManager->getStringReward().second == -1 ? 
				CEGUI::String(m_missionManager->getStringReward().first.c_str()) : CEGUI::String(" ") + 
				CEGUI::String(m_missionManager->getStringReward().first.substr(2, m_missionManager->getStringReward().first.length()-3 ).c_str()))
				+ " obtained."));
			/// TODO -- Internationalization -- /// }End
			}
		m_blendIn->start();
	}

	void CMissionNotification::setupAnimations()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* insideAnim = animMgr.getAnimation("InsideBlendInQuick");

		m_blendIn = animMgr.instantiateAnimation(insideAnim);
		CEGUI::Window* insideWindows = m_menuWindow->getChild("NoteWindow");
		m_blendIn->setTargetWindow(insideWindows);

		CEGUI::Animation* fadeAnim = animMgr.getAnimation("InsideFadeOutQuick");

		m_fadeOut = animMgr.instantiateAnimation(fadeAnim);
		insideWindows = m_menuWindow->getChild("NoteWindow");
		m_fadeOut->setTargetWindow(insideWindows);
	}

	void CMissionNotification::setupEvents()
	{
		m_menuWindow->getChild("NoteWindow/NoteBackButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CMissionNotification::onBackPushed, this));
	}

	bool CMissionNotification::onBackPushed(const CEGUI::EventArgs& e)
	{
		m_fadeOut->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("noteBackButton");
		return true;
	}
}