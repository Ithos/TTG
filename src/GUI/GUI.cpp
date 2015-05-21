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

#include "GUI.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/FontManager.h>
#include <CEGUI/AnimationManager.h>

#include "Controllers/PlayerController.h"
#include "InputManager.h"

namespace GUI
{
	CGUI* CGUI::m_instance = 0;

	CGUI::CGUI(): m_playerController(0)
	{
		m_instance = this;
	}

	CGUI::~CGUI()
	{
		m_instance = 0;
	}

	bool CGUI::init(CEGUI::System* system)
	{
		new CGUI();

		if(!m_instance->open(system)){
			release();
			return false;
		}

		return true;
	}

	void CGUI::release()
	{
		if(m_instance){
			m_instance->close();
			delete m_instance;
		}
	}

	bool CGUI::open(CEGUI::System* system)
	{
		m_playerController = new Controller::CPlayerController();
		m_playerController->setConfiguration("");

		m_system = system;

		//Load common animations here
		CEGUI::AnimationManager::getSingletonPtr()->loadAnimationsFromXML("GameMenu.anims");

		CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("SampleBrowser.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("TTGBackground.scheme");

		
#ifndef NON_EXCLUSIVE_MODE_IN_WINDOW_MODE 
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("GameMenuImages/MouseCursor");
#endif	
		
		CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
		CEGUI::Font& defaultFont = CEGUI::FontManager::getSingleton().createFromFile("Jura-18.font");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setDefaultFont(&defaultFont);
		
		CInputManager::getInstance()->addKeyListener(this);
		CInputManager::getInstance()->addMouseListener(this);

		return true;
	}

	void CGUI::close()
	{
		CInputManager::getInstance()->removeKeyListener(this);
		CInputManager::getInstance()->removeMouseListener(this);
		delete m_playerController;
	}

	bool CGUI::keyPressed(Common::Input::TKey key)
	{
		m_system->getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan) key.id);
		m_system->getDefaultGUIContext().injectChar(key.text);
		return false;
	}

	bool CGUI::keyReleased(Common::Input::TKey key)
	{
		m_system->getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan) key.id);
		return false;
	}

	bool CGUI::mouseMoved(const InputListener::CMouseState &mouseState)
	{
#if defined NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
		m_system->getDefaultGUIContext().injectMousePosition((float)mouseState.posAbsX,
														     (float)mouseState.posAbsY);
#else 
		m_system->getDefaultGUIContext().injectMouseMove((float)mouseState.movX,
													     (float)mouseState.movY);
#endif	
		return false;
	}

	bool CGUI::mousePressed(const InputListener::CMouseState &mouseState)
	{
		switch (mouseState.button)
		{
		case Common::Input::Button::TMouseButton::LEFT:
			m_system->getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);break;
		case Common::Input::Button::TMouseButton::RIGHT:
			m_system->getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton);break;
		case Common::Input::Button::TMouseButton::MIDDLE:
			m_system->getDefaultGUIContext().injectMouseButtonDown(CEGUI::MiddleButton);break;
		}
		return false;
	}

	bool CGUI::mouseReleased(const InputListener::CMouseState &mouseState)
	{
		switch (mouseState.button)
		{
		case Common::Input::Button::TMouseButton::LEFT:
			m_system->getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);break;
		case Common::Input::Button::TMouseButton::RIGHT:
			m_system->getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton);break;
		case Common::Input::Button::TMouseButton::MIDDLE:
			m_system->getDefaultGUIContext().injectMouseButtonUp(CEGUI::MiddleButton);break;
		}
		return false;
	}
}