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

#ifndef __APPLICATION_MENUSTATE_H
#define __APPLICATION_MENUSTATE_H

#include "ApplicationState.h"

namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class Window;
	class EventArgs;
	class AnimationInstance;
}

namespace Ogre
{
	class Root;
	class RenderWindow;
}

namespace Application 
{

	class CMenuState : public CApplicationState
	{
	public:
		CMenuState(CBaseApplication *app, Common::Data::GameType type,
							Ogre::Root* root,
							Ogre::RenderWindow* render) : CApplicationState(app, type,root,render), m_menuWindow(nullptr), m_popupLinesShipsAnimInst(nullptr),
							m_popupLinesLoadAnimInst(nullptr), m_popupLinesQuitAnimInst(nullptr), m_popupLinesOptionsAnimInst(nullptr), m_topBarAnimInst(nullptr),
							m_botBarAnimInst(nullptr), m_insideBlendInAnimInst(nullptr), m_insideImage3RotateInInst(nullptr), m_insideImage4RotateInInst(nullptr),
							m_insideImageRingsContainerSizeInInst(nullptr), m_buttonFadeInAnimInst1(nullptr), m_buttonFadeInAnimInst2(nullptr), m_buttonFadeInAnimInst3(nullptr),
							m_buttonFadeInAnimInst4(nullptr), insideImg1AnimInst(nullptr), startButtAnimInstance(nullptr), m_startButtonBlendInAnimInst(nullptr),
							m_centerButtonsPartialBlendOutInst(nullptr), m_centerButtonsBlendInInst(nullptr),m_delay1(900), m_delay2(2000)
				{}
		virtual ~CMenuState();

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
		int m_delay1, m_delay2;
		CEGUI::Window* m_menuWindow;

		CEGUI::AnimationInstance* m_popupLinesShipsAnimInst;
		CEGUI::AnimationInstance* m_popupLinesLoadAnimInst;
		CEGUI::AnimationInstance* m_popupLinesQuitAnimInst;
		CEGUI::AnimationInstance* m_popupLinesOptionsAnimInst;
		CEGUI::AnimationInstance* m_topBarAnimInst;
		CEGUI::AnimationInstance* m_botBarAnimInst;
		CEGUI::AnimationInstance* m_insideBlendInAnimInst;
		CEGUI::AnimationInstance* m_insideImage3RotateInInst;
		CEGUI::AnimationInstance* m_insideImage4RotateInInst;
		CEGUI::AnimationInstance* m_insideImageRingsContainerSizeInInst;
		CEGUI::AnimationInstance* m_buttonFadeInAnimInst1;
		CEGUI::AnimationInstance* m_buttonFadeInAnimInst2;
		CEGUI::AnimationInstance* m_buttonFadeInAnimInst3;
		CEGUI::AnimationInstance* m_buttonFadeInAnimInst4;

		CEGUI::AnimationInstance* insideImg1AnimInst;
		CEGUI::AnimationInstance* startButtAnimInstance;
		CEGUI::AnimationInstance* m_startButtonBlendInAnimInst;
		CEGUI::AnimationInstance* m_centerButtonsPartialBlendOutInst;
		CEGUI::AnimationInstance* m_centerButtonsBlendInInst;

		bool startReleased(const CEGUI::EventArgs& e);
		bool exitReleased(const CEGUI::EventArgs& e);

		bool handleStartPopupLinesShipsDisplay(const CEGUI::EventArgs& args);
		bool handleStartPopupLinesLoadDisplay(const CEGUI::EventArgs& args);
		bool handleStartPopupLinesQuitDisplay(const CEGUI::EventArgs& args);
		bool handleStartPopupLinesOptionsDisplay(const CEGUI::EventArgs& args);

		bool handleInnerButtonsLabelEntered(const CEGUI::EventArgs& args);
		bool handleInnerButtonsLabelLeft(const CEGUI::EventArgs& args);

		bool handleInnerButtonEntered(const CEGUI::EventArgs& args);
		bool handleInnerButtonLeft(const CEGUI::EventArgs& args);

		CEGUI::Window* getIconWindowFromLabel(CEGUI::Window* window);
		void setupWindows();
		void setupPopupLinesAnimations();
		void setupSelectionIconAnimations();

		void setupAnimatons();
		void setupInnerButtonsSubOptionsLabels();
		void stopStartPopupLinesAnimations();
		void makeAllSelectionIconsInvisible();

		void onActivateMenu();
		void resetAnimations();
		void startEntranceAnimations();
		void startButtonsAnims();

	};
}


#endif