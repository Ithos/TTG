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

#ifndef __GUI_INPUTMANAGER_H
#define __GUI_INPUTMANAGER_H

#include <list>

#include "../Common/Input/Input.h"

#include "InputListeners/InputListener.h"
#include <OgreFrameListener.h>

namespace OIS
{
	class InputManager;
}

namespace GUI
{

	class CInputManager : public OIS::KeyListener, public OIS::MouseListener, public Ogre::FrameListener
	{
	public:
		static CInputManager* getInstance(){return m_instance;}

		static bool init(OIS::InputManager* input,
							OIS::Keyboard* key,
							OIS::Mouse* mouse);
		static void release();

		void tick();

		void addPriorityKeyListener(InputListener::CkeyBoardListener * keyListener) { m_priorityKeyBoardListener = keyListener; }
		void addPriorityMouseListener(InputListener::CMouseListener *mouseListener) { m_priorityMouseListener = mouseListener; }

		void removePriorityKeyListener() { m_priorityKeyBoardListener = nullptr; }
		void removePriorityMouseListener() { m_priorityMouseListener = nullptr; }

		void addKeyListener(InputListener::CkeyBoardListener *keyListener);
		void addMouseListener(InputListener::CMouseListener *mouseListener);

		void removeKeyListener(InputListener::CkeyBoardListener* keyListener);
		void removeMouseListener(InputListener::CMouseListener* mouseListener);

		void removeAllListeners();

		void removeAllKeyListeners();
		void removeAllMouseListeners();

		bool isKeyPressed(Common::Input::TKey key)
		{
			return m_keyboard->isKeyDown((OIS::KeyCode)key.id);
		}

		bool isModifierPressed(Common::Input::Modifier::TModifier modifier)
		{
			return m_keyboard->isModifierDown((OIS::Keyboard::Modifier)modifier);
		}
	protected:
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	private:
		static CInputManager* m_instance;

		CInputManager();
		virtual ~CInputManager();

		bool open(OIS::InputManager* input,
					OIS::Keyboard* key,
					OIS::Mouse* mouse);
		void close();

		bool keyPressed(const OIS::KeyEvent &e);
		bool keyReleased(const OIS::KeyEvent &e);

		Common::Input::TKey ois2ttg(const OIS::KeyEvent &e);

		bool mouseMoved(const OIS::MouseEvent &e);
		bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID button);
		bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID button);

		OIS::Mouse* m_mouse;
		OIS::Keyboard* m_keyboard;

		InputListener::CMouseState m_mouseState;

		OIS::InputManager* m_inputSystem;

		std::list<InputListener::CkeyBoardListener*> m_keyListeners;
		std::list<InputListener::CMouseListener*> m_mouseListeners;

		InputListener::CkeyBoardListener* m_priorityKeyBoardListener;
		InputListener::CMouseListener* m_priorityMouseListener;

	};
}

#endif