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


#ifndef __APPLICATION_BASEAPPLICATION_H
#define __APPLICATION_BASEAPPLICATION_H

#include <string>
#include <map>
#include <stack>

#include <GUI/InputListeners/InputListener.h>

namespace Ogre
{
	class Root;
	class RenderWindow;
}

namespace Application
{
    class CApplicationState;
    class CClock;
}

namespace Application 
{
	class CBaseApplication : public GUI::InputListener::CkeyBoardListener,
							 public GUI::InputListener::CMouseListener
	{
	public:
		CBaseApplication();
		virtual ~CBaseApplication();

		static CBaseApplication * getInstance() {return m_instance;}

		virtual bool init();
		virtual bool secondInit(){return true;};//This is for the initialization in two steps 
		virtual void release();
		virtual void releaseAllStates();

		virtual void run();

		void exitRequest()   { m_exit = true; }
		bool exitRequested() { return m_exit; }

		bool addState(const std::string &name, CApplicationState *state, bool asleep = false);
		bool setState(const std::string &name);

        bool popState();
        bool resetToMenuState(const std::string& initialGameState);

		CApplicationState *getCurrentState() { return m_currentState; }

		unsigned int getApptime();

		//key mouse
		virtual bool keyPressed(Common::Input::TKey key);
		virtual bool keyReleased(Common::Input::TKey key);

		virtual bool mouseMoved(const GUI::InputListener::CMouseState &mouseState);
		virtual bool mousePressed(const GUI::InputListener::CMouseState &mouseState);
		virtual bool mouseReleased(const GUI::InputListener::CMouseState &mouseState);

        CClock* getClock() { return m_clock; }
		Ogre::Root* getOgreRoot() {return m_root;}

    protected:
		void changeState();

		virtual void tick(unsigned int msecs);
        //clock
        CClock* m_clock;   

		//Ogre
		Ogre::Root*           m_root;
		Ogre::RenderWindow*   m_renderWindow;

	private:
		static CBaseApplication* m_instance;

		bool m_exit;
		bool m_initialized;

		CApplicationState*  m_currentState;
		CApplicationState*  m_nextState;

		std::map<std::string, CApplicationState*> m_states;
        std::stack<CApplicationState*>            m_stackState;


	};
}

#endif
