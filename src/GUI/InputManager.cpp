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

#include "InputManager.h"
#include <OgreRoot.h>

namespace GUI
{
	CInputManager* CInputManager::m_instance = 0;

	CInputManager::CInputManager(): m_mouse(0),
									m_keyboard(0),
									m_inputSystem(0),
									m_priorityKeyBoardListener(nullptr),
									m_priorityMouseListener(nullptr)
	{
		m_instance = this;
		Ogre::Root::getSingleton().addFrameListener(this);
	}

	CInputManager::~CInputManager()
	{
		m_instance = 0;
		Ogre::Root::getSingleton().removeFrameListener(this);
	}

	bool CInputManager::init(OIS::InputManager* input,
								OIS::Keyboard* key,
								OIS::Mouse* mouse)
	{
		new CInputManager();
		if(!m_instance->open(input,key,mouse)){
			release();
			return false;
		}
		return true;
	}

	void CInputManager::release()
	{
		if(m_instance){
			m_instance->close();
			delete m_instance;
		}
	}

	void CInputManager::tick()
	{

		if(m_mouse){
			m_mouse->capture();
		}
		if(m_keyboard){
			m_keyboard->capture();
		}
	}

	bool CInputManager::open(OIS::InputManager* input,
								OIS::Keyboard* key,
								OIS::Mouse* mouse)
	{
		if(!input || !key || !mouse) return false;

		m_inputSystem = input;
		m_keyboard = key;
		m_mouse = mouse;

		m_keyboard->setEventCallback(this);
		m_mouse->setEventCallback(this);

		return true;
	}

	void CInputManager::close()
	{
		m_mouse = 0;
		m_keyboard = 0;
		m_inputSystem = 0;
		m_priorityKeyBoardListener = nullptr;
		m_priorityMouseListener = nullptr;
	}

	void CInputManager::addKeyListener(InputListener::CkeyBoardListener *keyListener)
	{
		if(m_keyboard){
			m_keyListeners.push_front(keyListener);
		}
	}

	void CInputManager::addMouseListener(InputListener::CMouseListener *mouseListener)
	{
		if(m_mouse){
			m_mouseListeners.push_front(mouseListener);
		}
	}

	void CInputManager::removeKeyListener(InputListener::CkeyBoardListener *keyListener)
	{
		m_keyListeners.remove(keyListener);
	}

	void CInputManager::removeMouseListener(InputListener::CMouseListener *mouseListener)
	{
		m_mouseListeners.remove(mouseListener);
	}

	void CInputManager::removeAllKeyListeners()
	{
		m_priorityKeyBoardListener = nullptr;
		m_keyListeners.clear();
	}

	void CInputManager::removeAllMouseListeners()
	{
		m_priorityMouseListener = nullptr;
		m_mouseListeners.clear();
	}

	void CInputManager::removeAllListeners()
	{
		m_priorityKeyBoardListener = nullptr;
		m_priorityMouseListener = nullptr;
		m_keyListeners.clear();
		m_mouseListeners.clear();
	}

	Common::Input::TKey CInputManager::ois2ttg(const OIS::KeyEvent &e)
	{
		unsigned int text = e.text;
		//The numpad keys dont have text... Adding text to the numpad keys
        switch (e.key)
        {
        case OIS::KC_DECIMAL:
                text = 46;
                break;
        case OIS::KC_DIVIDE:
                text = 47;
                break;
        case OIS::KC_NUMPAD0:
                text = 48;
                break;
        case OIS::KC_NUMPAD1:
                text = 49;
                break;
        case OIS::KC_NUMPAD2:
                text = 50;
                break;
        case OIS::KC_NUMPAD3:
                text = 51;
                break;
        case OIS::KC_NUMPAD4:
                text = 52;
                break;
        case OIS::KC_NUMPAD5:
                text = 53;
                break;
        case OIS::KC_NUMPAD6:
                text = 54;
                break;
        case OIS::KC_NUMPAD7:
                text = 55;
                break;
        case OIS::KC_NUMPAD8:
                text = 56;
                break;
        case OIS::KC_NUMPAD9:
                text = 57;
                break;
        }

		return Common::Input::TKey(text,(const Common::Input::Key::TKeyID)e.key);
	}

	bool CInputManager::frameRenderingQueued(const Ogre::FrameEvent& fe)
	{
		
		m_keyboard->capture();

		for(int i = Common::Input::Key::TKeyID::UNASSIGNED; i <= Common::Input::Key::TKeyID::MEDIASELECT; ++i){
			if(m_keyboard->isKeyDown(static_cast<OIS::KeyCode>(i))){

				if(!m_keyListeners.empty()){
					std::list<InputListener::CkeyBoardListener*>::const_iterator it 
					(m_keyListeners.begin());
					for(; it != m_keyListeners.end(); ++it){
						(*it)->pollingKeyCheckPressed(Common::Input::TKey(0xFFFFFFFF,(const Common::Input::Key::TKeyID)i));
					}
				}

			}else{

				if(!m_keyListeners.empty()){
					std::list<InputListener::CkeyBoardListener*>::const_iterator it 
					(m_keyListeners.begin());
					for(; it != m_keyListeners.end(); ++it){
						(*it)->pollingKeyCheckReleased(Common::Input::TKey(0xFFFFFFFF,(const Common::Input::Key::TKeyID)i));
					}

				}
			}
		}

		return true;
	}

	bool CInputManager::keyPressed(const OIS::KeyEvent &e)
	{
		if(m_priorityKeyBoardListener)
			if(m_priorityKeyBoardListener->keyPressed(ois2ttg(e)))
				return false;
		if(!m_keyListeners.empty()){
			std::list<InputListener::CkeyBoardListener*>::const_iterator it 
				(m_keyListeners.begin());
			for(; it != m_keyListeners.end(); ++it){
				if((*it)->keyPressed(ois2ttg(e))) break;
			}
		}
		return false;
	}

	bool CInputManager::keyReleased(const OIS::KeyEvent &e)
	{
		if(m_priorityKeyBoardListener)
			if(m_priorityKeyBoardListener->keyReleased(ois2ttg(e)))
				return false;
		if(!m_keyListeners.empty()){
			std::list<InputListener::CkeyBoardListener*>::const_iterator it 
				(m_keyListeners.begin());
			for(; it != m_keyListeners.end(); ++it){
				if((*it)->keyReleased(ois2ttg(e))) break;
			}
		}
		return false;
	}

	bool CInputManager::mouseMoved(const OIS::MouseEvent &e)
	{
		if(m_priorityMouseListener){
			m_mouseState.setExtents(e.state.width, e.state.height);
			m_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			m_mouseState.movX = e.state.X.rel;
			m_mouseState.movY = e.state.Y.rel;
			m_mouseState.scrool = e.state.Z.rel;
			m_mouseState.button = Common::Input::Button::TMouseButton::UNASSIGNED;
			if(m_priorityMouseListener->mouseMoved(m_mouseState))
				return true;
		}
		if(!m_mouseListeners.empty()){
			m_mouseState.setExtents(e.state.width, e.state.height);
			m_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			m_mouseState.movX = e.state.X.rel;
			m_mouseState.movY = e.state.Y.rel;
			m_mouseState.scrool = e.state.Z.rel;
			m_mouseState.button = Common::Input::Button::TMouseButton::UNASSIGNED;

			std::list<InputListener::CMouseListener*>::const_iterator it
				(m_mouseListeners.begin());
			for (; it != m_mouseListeners.end(); it++) 
			{
				if ((*it)->mouseMoved(m_mouseState))
				  return true;
			}
		}
		return false;
	}

	bool CInputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID button)
	{
		if(m_priorityMouseListener){
			m_mouseState.setExtents(e.state.width, e.state.height);
			m_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			m_mouseState.movX = e.state.X.rel;
			m_mouseState.movY = e.state.Y.rel;
			m_mouseState.scrool = e.state.Z.rel;
			m_mouseState.button = (Common::Input::Button::TMouseButton) button;
			if(m_priorityMouseListener->mousePressed(m_mouseState))
				return true;
		}
		if(!m_mouseListeners.empty()){
			m_mouseState.setExtents(e.state.width, e.state.height);
			m_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			m_mouseState.movX = e.state.X.rel;
			m_mouseState.movY = e.state.Y.rel;
			m_mouseState.scrool = e.state.Z.rel;
			m_mouseState.button = (Common::Input::Button::TMouseButton) button;

			std::list<InputListener::CMouseListener*>::const_iterator it
				(m_mouseListeners.begin());
			for (; it != m_mouseListeners.end(); it++) 
			{
				if ((*it)->mousePressed(m_mouseState))
				  return true;
			}
		}
		return false;
	}

	bool CInputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID button)
	{
		if(m_priorityMouseListener){
			m_mouseState.setExtents(e.state.width, e.state.height);
			m_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			m_mouseState.movX = e.state.X.rel;
			m_mouseState.movY = e.state.Y.rel;
			m_mouseState.scrool = e.state.Z.rel;
			m_mouseState.button = (Common::Input::Button::TMouseButton) button;
			if(m_priorityMouseListener->mouseReleased(m_mouseState))
				return true;
		}
		if(!m_mouseListeners.empty()){
			m_mouseState.setExtents(e.state.width, e.state.height);
			m_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			m_mouseState.movX = e.state.X.rel;
			m_mouseState.movY = e.state.Y.rel;
			m_mouseState.scrool = e.state.Z.rel;
			m_mouseState.button = (Common::Input::Button::TMouseButton) button;

			std::list<InputListener::CMouseListener*>::const_iterator it
				(m_mouseListeners.begin());
			for (; it != m_mouseListeners.end(); it++) 
			{
				if ((*it)->mouseReleased(m_mouseState))
				  return true;
			}
		}
		return false;
	}
}