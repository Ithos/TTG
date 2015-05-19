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

#ifndef __GUI_GUI_H
#define __GUI_GUI_H

#ifdef _DEBUG
#define NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
#endif

#include "InputListeners/InputListener.h"
#include "../Common/Input/Input.h"

namespace GUI
{
	namespace Controller
	{
		class CPlayerController;
	}
}

namespace CEGUI
{
	class System;
}

namespace GUI
{
	class CGUI : public InputListener::CkeyBoardListener, public InputListener::CMouseListener
	{
	public:
		static CGUI* getInstance(){return m_instance;}

		static bool init(CEGUI::System* system);
		static void release();

		Controller::CPlayerController* getPlayerController(){return m_playerController;}

		bool keyPressed(Common::Input::TKey key);
		bool keyReleased(Common::Input::TKey key);

		bool mouseMoved(const InputListener::CMouseState &mouseState);
		bool mousePressed(const InputListener::CMouseState &mouseState);
		bool mouseReleased(const InputListener::CMouseState &mouseState);

	protected:
	private:
		static CGUI* m_instance;
		
		CGUI();
		~CGUI();

		bool open(CEGUI::System* system);
		void close();

		Controller::CPlayerController* m_playerController;

		CEGUI::System* m_system;
	};
}

#endif