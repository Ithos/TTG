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

#ifndef __APPLICATION_PLANETGUI_H
#define __APPLICATION_PLANETGUI_H

namespace Application 
{
	class CGameManager;
	class CPlanetState;
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
	class CPlanetGUI
	{
	public:
		static CPlanetGUI* getInstance() {return m_instance;}

		static bool init(CEGUI::System* sys, Ogre::Root* root, CPlanetState* state);
		static void release();

		void activate();
		void deactivate();

		void tick(unsigned int msecs);

		void addLock();
		void releaseLock();

		void disableAccess();

		void activateEnterButton();
		void deactivateEnterButton();

		void showWarning();
		void hideWarning();

		bool onBackClicked(const CEGUI::EventArgs& e);
		bool onLandClicked(const CEGUI::EventArgs& e);

		void shipDestroyed();
	private:
		static CPlanetGUI* m_instance;

		Ogre::Root* m_root;
		CPlanetState* m_planetState;

		CEGUI::Window* m_menuWindow;
		CEGUI::BasicImage* m_image;
		CEGUI::System* m_ceguiSystem;

		CEGUI::AnimationInstance* m_blackScreenFadeOut;
		CEGUI::AnimationInstance* m_blackScreenFadeIn;
		CEGUI::AnimationInstance* m_planetGUIScreenFadeOut;
		CEGUI::AnimationInstance* m_planetGUIScreenFadeIn;
		CEGUI::AnimationInstance* m_minimapImageScreenFadeOut;
		CEGUI::AnimationInstance* m_minimapImageScreenFadeIn;
		CEGUI::AnimationInstance* m_buttonFadeOut;
		CEGUI::AnimationInstance* m_buttonFadeIn;
		CEGUI::AnimationInstance* m_buttonLandFadeOut;
		CEGUI::AnimationInstance* m_messageFadeOut;

		CGameManager* m_mgrInstance;

		unsigned int m_backLockCount;

		CPlanetGUI(CEGUI::System* sys, Ogre::Root* root, CPlanetState* state);
		~CPlanetGUI();

		void setupAnimations();
		void setupHUD();
		void deactivateGUI();
		void setupEvents();
	};
}


#endif