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


#ifndef __APPLICATION_GENERALNOTIFICATION_H
#define __APPLICATION_GENERALNOTIFICATION_H


namespace CEGUI
{
	class Window;
	class EventArgs;
	class AnimationInstance;
}

namespace Application
{
	class CGeneralNotification
	{
	public:
		static CGeneralNotification* getInstance(){return m_instance;}

		static bool init();
		static void release();

		void activate(CEGUI::Window* window);
		void deactivate(CEGUI::Window* window);

		void showTutorialMessage();
		void repeatTutorial();

		void showSignalNotification();

		void resetTutorial(){m_showTut=true;}

	private:
		CGeneralNotification();
		~CGeneralNotification();

		static CGeneralNotification* m_instance;

		CEGUI::Window* m_menuWindow;

		CEGUI::AnimationInstance* m_blendIn;
		CEGUI::AnimationInstance* m_fadeOut;

		bool m_showTut;

		void setupAnimations();
		void setupEvents();
		void setupWindow();
		void setupSecondWindow();

		void setupSignalNotification();

		bool onBackPushed(const CEGUI::EventArgs& e);
		bool onContinuePushed(const CEGUI::EventArgs& e);
	};
}
#endif