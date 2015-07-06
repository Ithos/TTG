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

#ifndef __GUI_CONTROLLERS_PLAYERCONTROLLER_H
#define __GUI_CONTROLLERS_PLAYERCONTROLLER_H

#include <string>
#include <map>

#include "../InputListeners/InputListener.h"

namespace GUI
{
	namespace Controller
	{
		namespace Command
		{
			class ICommand;
		}
	}
}

namespace Logic
{
	class CEntity;
}

namespace GUI
{
	namespace Controller
	{
		class CPlayerController : public InputListener::CkeyBoardListener,
								  public InputListener::CMouseListener
		{
		public:
			CPlayerController();
			~CPlayerController();

			bool setConfiguration(const std::string &filename);

			void setControllerAvatar(Logic::CEntity* avatar){m_avatar = avatar;}

			void activate();
			void deactivate();

			bool keyPressed(Common::Input::TKey key);
			bool keyReleased(Common::Input::TKey key);

			bool mouseMoved(const InputListener::CMouseState &mouseState);
			bool mousePressed(const InputListener::CMouseState &mouseState);
			bool mouseReleased(const InputListener::CMouseState &mouseState);

		protected:
		private:

			typedef std::map<Common::Input::Key::TKeyID,GUI::Controller::Command::ICommand*> 
				TKeyCommandMap;

			TKeyCommandMap m_keyCommands;

			typedef std::map<Common::Input::Button::TMouseButton, GUI::Controller::Command::ICommand*> 
				TMouseCommandMap;

			TMouseCommandMap m_mouseCommands;

			Logic::CEntity *m_avatar;

		};
	}
}

#endif