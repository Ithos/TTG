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

#ifndef __APPLICATION_SYSTEMSTATE_H
#define __APPLICATION_SYSTEMSTATE_H

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
	class System;
	class BasicImage;
}

namespace Ogre
{
	class Root;
	class RenderWindow;
	class RaySceneQuery;
	class SceneNode;
}

namespace Application
{

	class CSystemState : public CApplicationState 
	{
		public:
		CSystemState(CBaseApplication *app, Common::Data::GameType type,
							Ogre::Root* root, CEGUI::System* sys,
							Ogre::RenderWindow* render) : CApplicationState(app, type,root,render), m_RayScnQuery(nullptr), m_CurrentPlanet(nullptr),
							m_PointedPlanet(nullptr), m_menuWindow(nullptr), m_blackScreenFadeOut(nullptr), m_blackScreenFadeIn(nullptr), m_ceguiSystem(sys),
							m_image(nullptr), m_sytemGUIScreenFadeOut(nullptr), m_sytemGUIScreenFadeIn(nullptr), m_planetImageScreenFadeOut(nullptr), 
							m_planetImageScreenFadeIn(nullptr)
				{}
		virtual ~CSystemState() {}

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
	protected:
		CEGUI::System* m_ceguiSystem;
		CEGUI::BasicImage* m_image;

		CEGUI::Window* m_menuWindow;

		CEGUI::AnimationInstance* m_blackScreenFadeOut;
		CEGUI::AnimationInstance* m_blackScreenFadeIn;
		CEGUI::AnimationInstance* m_sytemGUIScreenFadeOut;
		CEGUI::AnimationInstance* m_sytemGUIScreenFadeIn;
		CEGUI::AnimationInstance* m_planetImageScreenFadeOut;
		CEGUI::AnimationInstance* m_planetImageScreenFadeIn;

		void setupAnimations();
		void setupEvents();

		bool onBackClicked(const CEGUI::EventArgs& e);
		bool onAccessClicked(const CEGUI::EventArgs& e);
		bool accessPlanet(const CEGUI::EventArgs& e);
		bool goBack(const CEGUI::EventArgs& e);

		Ogre::RaySceneQuery* m_RayScnQuery;
		Ogre::SceneNode *m_CurrentPlanet,*m_PointedPlanet;
	};

}


#endif