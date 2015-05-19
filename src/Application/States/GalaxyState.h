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

#ifndef __APPLICATION_GALAXYSTATE_H
#define __APPLICATION_GALAXYSTATE_H

#include "ApplicationState.h"

namespace Application 
{
	class CBaseApplication;
}

namespace Logic
{
	class CScene;
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
	class RaySceneQuery;
	class SceneNode;
	class Vector3;
	class Vector2;
	class Camera;
}


namespace Application
{

	class CGalaxyState : public CApplicationState 
	{
	public:
		CGalaxyState(CBaseApplication *app, Common::Data::GameType type,
							Ogre::Root* root,
							Ogre::RenderWindow* render) : CApplicationState(app, type,root,render), m_RayScnQuery(nullptr), m_CurrentSystem(nullptr),
							m_PointedSystem(nullptr), m_playerSystem(nullptr), m_menuWindow(nullptr), m_buttonPopupLines(nullptr), m_systemIconAnimation(nullptr),
							m_overButton(false), m_blackScreenFadeOut(nullptr), m_blackScreenFadeIn(nullptr), m_tagPopupLines(nullptr), m_rad(0.0)
				{}
		virtual ~CGalaxyState() {}

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

		CEGUI::Window* m_menuWindow;

		CEGUI::AnimationInstance* m_buttonPopupLines;
		CEGUI::AnimationInstance* m_tagPopupLines;
		CEGUI::AnimationInstance* m_systemIconAnimation;
		CEGUI::AnimationInstance* m_blackScreenFadeOut;
		CEGUI::AnimationInstance* m_blackScreenFadeIn;

		Ogre::RaySceneQuery* m_RayScnQuery;
		Ogre::SceneNode *m_CurrentSystem,*m_PointedSystem, *m_playerSystem;

		bool m_overButton;

		void setupWindows();
		void setupAnimations();

		bool onLabelEntered(const CEGUI::EventArgs& e);
		bool onLabelExited(const CEGUI::EventArgs& e);
		bool onLabelClicked(const CEGUI::EventArgs& e);
		bool accessSystem(const CEGUI::EventArgs& e);

		Ogre::Vector2 GetScreenspaceCoords(const Ogre::Vector3& iPoint, const Ogre::Camera& iCamera);
		void drawCircle(Logic::CScene* scene,Ogre::SceneNode *centerNode, float radius, const char* str);

		float m_rad;
	};
}

#endif