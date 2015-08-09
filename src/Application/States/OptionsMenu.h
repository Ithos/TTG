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

#ifndef __APPLICATION_OPTIONSMENU_H
#define __APPLICATION_OPTIONSMENU_H

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

namespace Common
{
	namespace Sound
	{
		class CSound;
	}
}

namespace Application 
{

	class COptionsMenu : public CApplicationState
	{
	public:
		COptionsMenu(CBaseApplication *app, Common::Data::GameType type,
							Ogre::Root* root,
							Ogre::RenderWindow* render) : CApplicationState(app, type,root,render), m_menuWindow(nullptr), m_topBarAnimInst(nullptr),
							m_botBarAnimInst(nullptr), m_insideAnimInst(nullptr)
				{}
		virtual ~COptionsMenu();

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

	private:
		Common::Sound::CSound* m_soundInstance;
		CEGUI::Window* m_menuWindow;

		CEGUI::AnimationInstance* m_topBarAnimInst;
		CEGUI::AnimationInstance* m_botBarAnimInst;
		CEGUI::AnimationInstance* m_insideAnimInst;

		bool applyReleased(const CEGUI::EventArgs& e);
		bool cancelReleased(const CEGUI::EventArgs& e);
		bool onApplyLabelExited(const CEGUI::EventArgs& e);
		bool onApplyLabelEntered(const CEGUI::EventArgs& e);
		bool onCancelLabelEntered(const CEGUI::EventArgs& e);
		bool onCancelLabelExited(const CEGUI::EventArgs& e);

		bool onSoundFXChanged(const CEGUI::EventArgs& e);
		bool onMenuSoundsChanged(const CEGUI::EventArgs& e);
		bool onMusicChanged(const CEGUI::EventArgs& e);

		void setupWindows();
		void setupAnimatons();
		void onActivate();

	};

}
#endif