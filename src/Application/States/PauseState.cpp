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
#include "PauseState.h"

#include "../../Common/Sound/Sound.h"

#include <log.h>

#include <CEGUI/CEGUI.h>

namespace Application
{
	const char* const LOG_CPAUSESTATE = "Application::CPauseState";

	CPauseState::~CPauseState()
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel"))->removeAllEvents();
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ResumeContainer")->getChildElement("ResumeLabel"))->removeAllEvents();
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel"))->removeAllEvents();
	}

	bool CPauseState::init()
	{
		log_trace(LOG_CPAUSESTATE,"Init\n");

		CApplicationState::init();

		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("NewPause.layout");

		setupWindows();

		setupAnimatons();

		//Sounds & music

		Common::Sound::CSound::getSingletonPtr()->addSound("Button2.wav","buttonSound");
		Common::Sound::CSound::getSingletonPtr()->addSound("SoundIntro2.wav","buttonHover");
		Common::Sound::CSound::getSingletonPtr()->addSound("Intro1.wav","introPauseSound");


		return true;
	}

	void CPauseState::release() 
	{
		log_trace(LOG_CPAUSESTATE,"Release\n");

		Common::Sound::CSound::getSingletonPtr()->releaseSound("buttonSound");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("buttonHover");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("introPauseSound");

		CApplicationState::release();

	}

	void CPauseState::activate() 
	{
		log_trace(LOG_CPAUSESTATE,"Activate\n");

		CApplicationState::activate();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		onActivate();
		Common::Sound::CSound::getSingletonPtr()->playDelayedSound("introPauseSound",400);
	}

	void CPauseState::deactivate() 
	{	
		log_trace(LOG_CPAUSESTATE,"Deactivate\n");

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	}

	void CPauseState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	}

	bool CPauseState::keyPressed(Common::Input::TKey key)
	{
		return false;

	} 

	bool CPauseState::keyReleased(Common::Input::TKey key)
	{

		switch(key.id)
		{
		case Common::Input::Key::ESCAPE:
			m_app->popState();
			break;
		case Common::Input::Key::RETURN:
			break;
		default:
			return false;
		}
		return true;

	} 
	
	bool CPauseState::mouseMoved(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}
		
	bool CPauseState::mousePressed(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool CPauseState::mouseReleased(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}
		
	bool CPauseState::resumeReleased(const CEGUI::EventArgs& e)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonSound");
		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){
			m_app->popState();
		}
		return true;

	}

	bool CPauseState::exitReleased(const CEGUI::EventArgs& e)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonSound");
		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){
			m_app->exitRequest();
		}
		return true;

	}

	bool CPauseState::menuReleased(const CEGUI::EventArgs& e)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonSound");
		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){
			m_app->resetToMenuState("galaxy");
		}
		return true;

	}

	bool CPauseState::onExitLabelEntered(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel"))->
			setProperty("NormalTextColour","FFFFFFFF");
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonHover");
		return true;
	}

	bool CPauseState::onExitLabelExited(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel"))->
			setProperty("NormalTextColour","FF00A9FF");
		return true;
	}

	bool CPauseState::onResumeLabelEntered(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ResumeContainer")->getChildElement("ResumeLabel"))->
			setProperty("NormalTextColour","FFFFFFFF");
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonHover");
		return true;
	}

	bool CPauseState::onResumeLabelExited(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ResumeContainer")->getChildElement("ResumeLabel"))->
			setProperty("NormalTextColour","FF00A9FF");
		return true;
	}

	bool CPauseState::onMenuLabelEntered(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel"))->
			setProperty("NormalTextColour","FFFFFFFF");
			Common::Sound::CSound::getSingletonPtr()->playSound("buttonHover");
		return true;
	}

	bool CPauseState::onMenuLabelExited(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel"))->
			setProperty("NormalTextColour","FF00A9FF");
		return true;
	}

	void CPauseState::setupWindows()
	{
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&CPauseState::exitReleased, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel")->
			subscribeEvent(CEGUI::Window::EventMouseEntersArea, 
			CEGUI::SubscriberSlot(&CPauseState::onExitLabelEntered, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ExitContainer")->getChildElement("ExitLabel")->
			subscribeEvent(CEGUI::Window::EventMouseLeavesArea, 
			CEGUI::SubscriberSlot(&CPauseState::onExitLabelExited, this));

		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ResumeContainer")->getChildElement("ResumeLabel")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&CPauseState::resumeReleased, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ResumeContainer")->getChildElement("ResumeLabel")->
			subscribeEvent(CEGUI::Window::EventMouseEntersArea, 
				CEGUI::SubscriberSlot(&CPauseState::onResumeLabelEntered, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ResumeContainer")->getChildElement("ResumeLabel")->
			subscribeEvent(CEGUI::Window::EventMouseLeavesArea, 
				CEGUI::SubscriberSlot(&CPauseState::onResumeLabelExited, this));

		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&CPauseState::menuReleased, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel")->
			subscribeEvent(CEGUI::Window::EventMouseEntersArea, 
				CEGUI::SubscriberSlot(&CPauseState::onMenuLabelEntered, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("MenuContainer")->getChildElement("MenuLabel")->
			subscribeEvent(CEGUI::Window::EventMouseLeavesArea, 
				CEGUI::SubscriberSlot(&CPauseState::onMenuLabelExited, this));
	}

	void CPauseState::setupAnimatons()
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
		CEGUI::Window* buttonWindow2 = m_menuWindow->getChild("InnerButtonsContainer/ResumeContainer");
		m_windowAnimInst2->setTargetWindow(insideWindows);
		m_windowAnimInst3 = animMgr.instantiateAnimation(windowAnim);
		CEGUI::Window* buttonWindow3 = m_menuWindow->getChild("InnerButtonsContainer/ExitContainer");
		m_windowAnimInst3->setTargetWindow(insideWindows);
	}

	void CPauseState::onActivate()
	{
		m_topBarAnimInst->start();
		m_botBarAnimInst->start();
		m_insideAnimInst->start();
		m_windowAnimInst1->start();
		m_windowAnimInst2->start();
		m_windowAnimInst3->start();
	}
}