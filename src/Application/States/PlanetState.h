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

#ifndef __APPLICATION_PLANETSTATE_H
#define __APPLICATION_PLANETSTATE_H

#include "ApplicationState.h"

namespace Application 
{
	class CBaseApplication;
	class CGameManager;
	class CPlanetGUI;
}

namespace CEGUI
{
	class Window;
	class EventArgs;
	class AnimationInstance;
	class BasicImage;
	class System;
}

namespace Ogre
{
	class Root;
	class RenderWindow;
}

namespace Application
{

	class CPlanetState : public CApplicationState 
	{
		public:
		CPlanetState(CBaseApplication *app, Common::Data::GameType type,
							Ogre::Root* root,
							CEGUI::System* sys,
							Ogre::RenderWindow* render) : CApplicationState(app, type,root,render), m_mgrInstance(nullptr),
							m_guiInstance(nullptr), m_ceguiSystem(sys), m_time(0.0f)
				{}
		virtual ~CPlanetState() {}

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

		bool goBack(const CEGUI::EventArgs& e);
		bool setGameOver(const CEGUI::EventArgs& e);
		bool setEvent();

	protected:
		CGameManager* m_mgrInstance;
		CEGUI::System* m_ceguiSystem;
		CPlanetGUI* m_guiInstance;
		float m_time;
	};

}

#endif