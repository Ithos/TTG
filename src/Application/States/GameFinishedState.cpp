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

#include "GameFinishedState.h"

#include "../../Common/Sound/Sound.h"

#include <log.h>

#include <CEGUI/CEGUI.h>

namespace Application
{
	const char* const LOG_GAMEFINISHEDSTATE = "Application::CGameFinishedState";

	CGameFinishedState::~CGameFinishedState()
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel"))->removeAllEvents();
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel"))->removeAllEvents();
	}

	bool CGameFinishedState::init()
	{
		log_trace(LOG_GAMEFINISHEDSTATE,"Init\n");

		CApplicationState::init();

		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("GameEnd.layout");

		setupWindows();

		setupAnimatons();

		//Sounds & music

		Common::Sound::CSound::getSingletonPtr()->addSound("Button2.wav","buttonSoundFinished");
		Common::Sound::CSound::getSingletonPtr()->addSound("SoundIntro2.wav","buttonHoverFinished");
		Common::Sound::CSound::getSingletonPtr()->addSound("Intro1.wav","introFinishedSound");

		return true;
	}

	void CGameFinishedState::release() 
	{
		log_trace(LOG_GAMEFINISHEDSTATE,"Release\n");

		Common::Sound::CSound::getSingletonPtr()->releaseSound("buttonSoundFinished");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("buttonHoverFinished");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("introFinishedSound");

		CApplicationState::release();

	}

	void CGameFinishedState::activate() 
	{
		log_trace(LOG_GAMEFINISHEDSTATE,"Activate\n");

		CApplicationState::activate();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		onActivate();
		Common::Sound::CSound::getSingletonPtr()->playDelayedSound("introFinishedSound",400);
	}

	void CGameFinishedState::deactivate() 
	{	
		log_trace(LOG_GAMEFINISHEDSTATE,"Deactivate\n");

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	}

	void CGameFinishedState::setupWindows()
	{
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&CGameFinishedState::exitReleased, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel")->
			subscribeEvent(CEGUI::Window::EventMouseEntersArea, 
			CEGUI::SubscriberSlot(&CGameFinishedState::onExitLabelEntered, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel")->
			subscribeEvent(CEGUI::Window::EventMouseLeavesArea, 
			CEGUI::SubscriberSlot(&CGameFinishedState::onExitLabelExited, this));

		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&CGameFinishedState::menuReleased, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel")->
			subscribeEvent(CEGUI::Window::EventMouseEntersArea, 
				CEGUI::SubscriberSlot(&CGameFinishedState::onMenuLabelEntered, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel")->
			subscribeEvent(CEGUI::Window::EventMouseLeavesArea, 
				CEGUI::SubscriberSlot(&CGameFinishedState::onMenuLabelExited, this));
	}

	void CGameFinishedState::setupAnimatons()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* topBarAnim = animMgr.getAnimation("TopBarMoveInAnimationQuick");
		m_topBarAnimInst = animMgr.instantiateAnimation(topBarAnim);
		CEGUI::Window* topBarWindow = m_menuWindow->getChild("TopBar");
		m_topBarAnimInst->setTargetWindow(topBarWindow);

		CEGUI::Animation* botBarAnim = animMgr.getAnimation("BotBarMoveInAnimationQuick");
		m_botBarAnimInst = animMgr.instantiateAnimation(botBarAnim);
		CEGUI::Window* botBarWindow = m_menuWindow->getChild("BotBar");
		m_botBarAnimInst->setTargetWindow(botBarWindow);

		CEGUI::Animation* insideAnim = animMgr.getAnimation("InsideBlendInQuick");
		m_insideAnimInst = animMgr.instantiateAnimation(insideAnim);
		CEGUI::Window* insideWindows = m_menuWindow->getChild("InnerButtonsContainer");
		m_insideAnimInst->setTargetWindow(insideWindows);

		CEGUI::Animation* windowAnim = animMgr.getAnimation("OpenWindow");

		m_windowAnimInst1 = animMgr.instantiateAnimation(windowAnim);
		CEGUI::Window* buttonWindow1 = m_menuWindow->getChild("InnerButtonsContainer/MenuContainer");
		m_windowAnimInst1->setTargetWindow(insideWindows);

		m_windowAnimInst2 = animMgr.instantiateAnimation(windowAnim);
		CEGUI::Window* buttonWindow2 = m_menuWindow->getChild("InnerButtonsContainer/ExitContainer");
		m_windowAnimInst2->setTargetWindow(insideWindows);
	}

	void CGameFinishedState::onActivate()
	{
		m_topBarAnimInst->start();
		m_botBarAnimInst->start();
		m_insideAnimInst->start();
		m_windowAnimInst1->start();
		m_windowAnimInst2->start();
	}

	bool CGameFinishedState::menuReleased(const CEGUI::EventArgs& e)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonSoundFinished");
		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){
			m_app->resetToMenuState("galaxy");
		}
		return true;
	}

	bool CGameFinishedState::exitReleased(const CEGUI::EventArgs& e)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonSoundFinished");
		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){
			m_app->exitRequest();
		}
		return true;
	}

	bool CGameFinishedState::onExitLabelEntered(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel"))->
			setProperty("NormalTextColour","FFFFFFFF");
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonHoverFinished");
		return true;
	}

	bool CGameFinishedState::onExitLabelExited(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel"))->
			setProperty("NormalTextColour","FF00A9FF");
		return true;
	}

	bool CGameFinishedState::onMenuLabelEntered(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel"))->
			setProperty("NormalTextColour","FFFFFFFF");
			Common::Sound::CSound::getSingletonPtr()->playSound("buttonHoverFinished");
		return true;
	}

	bool CGameFinishedState::onMenuLabelExited(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel"))->
			setProperty("NormalTextColour","FF00A9FF");
		return true;
	}
}