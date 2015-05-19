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

#ifndef __APPLICATION_APPLICATOINSTATE_H
#define __APPLICATION_APPLICATOINSTATE_H

#include "../BaseApplication.h"
#include "../../Common/Data/TTG_Types.h"
#include "../../GUI/InputListeners/InputListener.h"

namespace Ogre
{
	class Root;
	class RenderWindow;
}

namespace Application
{
	class CApplicationState : public GUI::InputListener::CkeyBoardListener,
					          public GUI::InputListener::CMouseListener
	{
	public:
		CApplicationState(CBaseApplication *app, Common::Data::GameType type,
							Ogre::Root*           root,
							Ogre::RenderWindow*   render) : m_app(app), m_type(type),
							m_root(root), m_renderWindow(render){}
		virtual ~CApplicationState() {}

		virtual bool init() { return true; }
		virtual void release()    {}
		virtual void activate()   {}
		virtual void deactivate() {}
		virtual Common::Data::GameType getType() {return m_type;}
		virtual void setType(Common::Data::GameType type) {m_type = type;}

		virtual void tick(unsigned int msecs) {}

		//keys
		virtual bool keyPressed(Common::Input::TKey key)  { return false; }
		virtual bool keyReleased(Common::Input::TKey key) { return false; }
        // mouse
		virtual bool mouseMoved(const GUI::InputListener::CMouseState &mouseState)    { return false; }
		virtual bool mousePressed(const GUI::InputListener::CMouseState &mouseState)  { return false; }
		virtual bool mouseReleased(const GUI::InputListener::CMouseState &mouseState) { return false; }

	protected:

		CBaseApplication* m_app;

		/*
		This variable stores the type of state, currently we have three types
		MENU_STATE ->this type is for states that have an Ogre Scene which has to be created 
			   when the state is entered from other game typed state, and destroyed when 
			   the scene is changed for another game state.
		GAME_STATE-> this type is for states that only have a GUI, when we enter one of this states
				from a game state the secene doesn´t have to be erased.
		TMP_STATE-> this type si for temporary states, that are either terminated by themselves or 
			  by another state. Currently it is only used for the exit state, but it may be useful for loading screens.
		*/
		Common::Data::GameType m_type;

		//Ogre
		Ogre::Root*           m_root;
		Ogre::RenderWindow*   m_renderWindow;
	};
}

#endif