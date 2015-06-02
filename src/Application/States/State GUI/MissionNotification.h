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

#ifndef __APPLICATION_MISSIONNOTIFICATION_H
#define __APPLICATION_MISSIONNOTIFICATION_H

namespace Application 
{
	class CMissionManager;
	class CGameManager;
}

namespace CEGUI
{
	class Window;
	class EventArgs;
	class AnimationInstance;
}

namespace Application
{
	class CMissionNotification
	{
	public:
		static CMissionNotification* getInstance() {return m_instance;}

		static bool init();
		static void release();

		void activate(CEGUI::Window* window);
		void deactivate();

		void showNote(bool mainMission=false);

		void gameEnded(){m_gameEnded = true;}
		void resetGame(){m_gameEnded = false;}
	private:
		CMissionNotification();
		~CMissionNotification();

		bool m_gameEnded;

		static CMissionNotification* m_instance;
		CEGUI::Window* m_menuWindow;

		CMissionManager* m_missionManager;
		CGameManager* m_gameManager;

		CEGUI::AnimationInstance* m_blendIn;
		CEGUI::AnimationInstance* m_fadeOut;

		void setupAnimations();
		void setupEvents();

		bool onBackPushed(const CEGUI::EventArgs& e);

	};

}

#endif