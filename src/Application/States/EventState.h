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
#ifndef __APPLICATION_EVENTSTATE_H
#define __APPLICATION_EVENTSTATE_H

#include "ApplicationState.h"

namespace Application 
{
	class CBaseApplication;
	class CMissionManager;
	class CGameManager;
	class CEventGUI;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
	class AnimationInstance;
}

namespace Ogre
{
	class Root;
	class RenderWindow;
}

namespace Application
{

	class CEventState : public CApplicationState 
	{
	public:
		CEventState(CBaseApplication *app, Common::Data::GameType type,
							Ogre::Root* root,
							Ogre::RenderWindow* render) : CApplicationState(app, type,root,render), m_topBarAnimInst(nullptr), m_botBarAnimInst(nullptr),
							m_insideAnimInst(nullptr), m_windowAnimInst(nullptr)
				{}
		virtual ~CEventState();

		virtual bool init();
		virtual void release();
		virtual void activate();
		virtual void deactivate();

		virtual void tick(unsigned int msecs);

		virtual bool keyPressed(Common::Input::TKey key);
		virtual bool keyReleased(Common::Input::TKey key);

		virtual bool mouseMoved(const GUI::InputListener::CMouseState &mouseState);
		virtual bool mousePressed(const GUI::InputListener::CMouseState &mouseState);
		virtual bool mouseReleased(const GUI::InputListener::CMouseState &mouseState);

		bool backReleased(const CEGUI::EventArgs& e);

		void setGameFinished();

	private:

		CMissionManager* m_missionManager;
		CGameManager* m_gameManager;
		CEventGUI* m_guiInstance;

		CEGUI::Window* m_menuWindow;

		CEGUI::AnimationInstance* m_topBarAnimInst;
		CEGUI::AnimationInstance* m_botBarAnimInst;
		CEGUI::AnimationInstance* m_insideAnimInst;
		CEGUI::AnimationInstance* m_windowAnimInst;

		void setupWindows();
		void setupAnimatons();
		void onActivate();

	};

}

#endif