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

#include "GeneralNotification.h"

#include "../../../Common/Sound/Sound.h"

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
	const char* const LOG_CGENERALNOTE = "Application::CMissionNotification";
	CGeneralNotification* CGeneralNotification::m_instance = nullptr;

	CGeneralNotification::CGeneralNotification(): m_menuWindow(nullptr),m_blendIn(nullptr),m_fadeOut(nullptr),m_showTut(true)
	{
		m_instance = this;

		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("GeneralNotification.layout");

		setupEvents();
		setupAnimations();

		//Sounds & music
		Common::Sound::CSound::getSingletonPtr()->addSound("Button2.wav","genBackButton");
	}

	CGeneralNotification::~CGeneralNotification()
	{
		Common::Sound::CSound::getSingletonPtr()->releaseSound("genBackButton");
		m_instance = nullptr;
	}

	bool CGeneralNotification::init()
	{
		assert(!m_instance && "Second initialization of Application::CGeneralNotification is not allowed!");

		log_trace(LOG_CGENERALNOTE,"Init\n");

		new CGeneralNotification();

		return true;
	}

	void CGeneralNotification::release()
	{
		log_trace(LOG_CGENERALNOTE,"Release\n");

		if(m_instance)
		{
			delete m_instance;
		}
	}

	void CGeneralNotification::activate(CEGUI::Window* window)
	{
		log_trace(LOG_CGENERALNOTE,"Activate\n");

		window->addChild(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();
	}

	void CGeneralNotification::deactivate(CEGUI::Window* window)
	{
		log_trace(LOG_CGENERALNOTE,"Deactivate\n");

		window->removeChild(m_menuWindow);

		m_menuWindow->getChild("NoteWindow/NoteContinueButton")->enable();

		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
		m_menuWindow->getChild("NoteWindow")->setVisible(false);
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->resetList();
	}

	void CGeneralNotification::setupAnimations()
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

	void CGeneralNotification::setupEvents()
	{
		m_menuWindow->getChild("NoteWindow/NoteBackButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CGeneralNotification::onBackPushed, this));

		m_menuWindow->getChild("NoteWindow/NoteContinueButton")->
		subscribeEvent(CEGUI::PushButton::EventClicked, 
		CEGUI::SubscriberSlot(&CGeneralNotification::onContinuePushed, this));
	}

	bool CGeneralNotification::onBackPushed(const CEGUI::EventArgs& e)
	{
		m_fadeOut->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("genBackButton");
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->resetList();
		return true;
	}

	bool CGeneralNotification::onContinuePushed(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->resetList();
		m_menuWindow->getChild("NoteWindow/NoteContinueButton")->disable();
		Common::Sound::CSound::getSingletonPtr()->playSound("genBackButton");
		setupSecondWindow();

		return true;
	}

	void CGeneralNotification::setupWindow()
	{
		/// TODO -- Internationalization -- /// Begin{
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->addItem(new CEGUI::ListboxTextItem(
			CEGUI::String(">>Encrypted Message<<        >>Written by:XXXXX<<\nThe job consists in investigating 4 encrypted tachyon\nsignals which are supposed to be emmitted by active emrgency beacons.\nAlso, you should know that there is\nsome people very interested in keeping those beacons lost,\nso you should be carefull.\n>>End Message<<"))
			);
		/// TODO -- Internationalization -- /// }End
	}

	void CGeneralNotification::setupSecondWindow()
	{
		/// TODO -- Internationalization -- /// Begin{
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->setFont("Jura-13");
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->addItem(new CEGUI::ListboxTextItem(
			CEGUI::String(">> Controls\n"))
			);
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->setFont("Jura-10");
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("NoteWindow/NoteBoard"))->addItem(new CEGUI::ListboxTextItem(
			CEGUI::String("->General:\nESC-->>pause\n->Galaxy:\nDrag window frame-->>Move window\nDouble click window frame-->>Hide window\nDrag window edge-->>Resize window\n->Combat:\nWASD-->>Move\nSPACE-->>Fire Primary\nLeft Shift-->>Fire Secondary"))
			);
		/// TODO -- Internationalization -- /// }End
	}

	void CGeneralNotification::showTutorialMessage()
	{
		if(m_showTut){
			m_showTut = false;
			setupWindow();
			m_blendIn->start();
		}
	}

	void CGeneralNotification::repeatTutorial()
	{
		setupWindow();
		m_blendIn->start();
	}

}
