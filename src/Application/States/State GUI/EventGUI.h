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

#ifndef __APPLICATION_EVENTGUI_H
#define __APPLICATION_EVENTGUI_H

#include <map>

namespace Application 
{
	class CGameManager;
	class CMissionManager;
	class CEventState;
}

namespace CEGUI
{
	class Window;
	class EventArgs;
	class AnimationInstance;
}

namespace Application
{
	class CEventGUI
	{
	public:
		static CEventGUI* getInstance() {return m_instance;}

		static bool init(CEventState* state);
		static void release();

		void activate();
		void deactivate();

		void resetGUI();
		void finishGame();

		void reactivateEventGUI(){eventFired = false;}

	private:

		CEventGUI(CEventState* state);
		~CEventGUI();

		bool eventFired;

		static CEventGUI* m_instance;
		CEventState* m_eventState;

		CEGUI::Window* m_menuWindow;
		CEGUI::Window* m_inhabitedMenuWindow;
		CEGUI::Window* m_unoccupiedMenuWindow;

		CGameManager* m_mgrInstance;
		CMissionManager* m_missionManager;

		CEGUI::AnimationInstance* m_topBarAnimInst;
		CEGUI::AnimationInstance* m_botBarAnimInst;
		CEGUI::AnimationInstance* m_insideAnimInst;
		CEGUI::AnimationInstance* m_fadeAnimInst;
		CEGUI::AnimationInstance* m_windowAnimInst;

		CEGUI::AnimationInstance* m_topBarAnimInstInhabited;
		CEGUI::AnimationInstance* m_botBarAnimInstInhabited;
		CEGUI::AnimationInstance* m_insideAnimInstInhabited;
		CEGUI::AnimationInstance* m_fadeAnimInstInhabited;
		CEGUI::AnimationInstance* m_windowAnimInstInhabited;

		CEGUI::AnimationInstance* m_eventPageInAnimInst;
		CEGUI::AnimationInstance* m_eventPageFadeAnimInst;

		CEGUI::AnimationInstance* m_docksPageInAnimInst;
		CEGUI::AnimationInstance* m_docksPageFadeAnimInst;

		CEGUI::AnimationInstance* m_cityPageInAnimInst;
		CEGUI::AnimationInstance* m_cityPageFadeAnimInst;

		typedef std::multimap<std::string,std::pair<std::string,std::string>> TShopStock;
		TShopStock m_shopMap;
		typedef std::map<std::string,std::pair<int, int>> TEngineData;
		TEngineData m_engineDataMap;

		void setupAnimations();
		void setupHUD();
		void setupEvents();
		void onActivateAnims();

		void setupShop();
		void setupCity();
		void setupEvent();
		void setupResources();

		bool explorePlanet(const CEGUI::EventArgs& e);
		bool gatherResources(const CEGUI::EventArgs& e);
		bool exploreDocks(const CEGUI::EventArgs& e);
		bool searchCity(const CEGUI::EventArgs& e);
		bool purchase(const CEGUI::EventArgs& e);
		bool onItemSelected(const CEGUI::EventArgs& e);
		bool onRejectMission(const CEGUI::EventArgs& e);
		bool onAcceptMission(const CEGUI::EventArgs& e);
		bool onBackEvent(const CEGUI::EventArgs& e);
		bool onBackShop(const CEGUI::EventArgs& e);
	};
}


#endif