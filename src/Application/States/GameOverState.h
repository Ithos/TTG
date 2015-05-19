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
#ifndef __APPLICATION_GAMEOVERSTATE_H
#define __APPLICATION_GAMEOVERSTATE_H

#include "ApplicationState.h"

namespace Application 
{
	class CBaseApplication;
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
	class CGameOverState : public CApplicationState
	{
	public:
		CGameOverState(CBaseApplication *app, Common::Data::GameType type,
							Ogre::Root* root,
							Ogre::RenderWindow* render) : CApplicationState(app, type,root,render)
		{}
		virtual ~CGameOverState();

		virtual bool init();
		virtual void release();
		virtual void activate();
		virtual void deactivate();

	private:
		CEGUI::Window* m_menuWindow;

		CEGUI::AnimationInstance* m_topBarAnimInst;
		CEGUI::AnimationInstance* m_botBarAnimInst;
		CEGUI::AnimationInstance* m_insideAnimInst;
		CEGUI::AnimationInstance* m_windowAnimInst1;
		CEGUI::AnimationInstance* m_windowAnimInst2;

		bool menuReleased(const CEGUI::EventArgs& e);
		bool exitReleased(const CEGUI::EventArgs& e);
		bool onExitLabelEntered(const CEGUI::EventArgs& e);
		bool onExitLabelExited(const CEGUI::EventArgs& e);
		bool onMenuLabelEntered(const CEGUI::EventArgs& e);
		bool onMenuLabelExited(const CEGUI::EventArgs& e);

		void setupWindows();
		void setupAnimatons();
		void onActivate();
	};
}

#endif