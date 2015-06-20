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

#include "TTGApplication.h"

#include"../Common/Data/TTG_Types.h"

#include "Application\Manager\GameManager.h"
#include "Application\Manager\MissionManager.h"

#include "Application\States\MenuState.h"
#include "Application\States\PauseState.h"
#include "Application\States\EventState.h"
#include "Application\States\ExitState.h"
#include "Application\States\GalaxyState.h"
#include "Application\States\SystemState.h"
#include "Application\States\PlanetState.h"
#include "Application\States\LoadState.h"
#include "Application\States\GameOverState.h"
#include "Application\States\GameFinishedState.h"

#include <Common/Save/Save.h>

namespace Application
{
	CTTGApplication::CTTGApplication() : C3DApplication(){}
	CTTGApplication::~CTTGApplication(){}

	bool CTTGApplication::init()
	{
		if(!C3DApplication::init()) return false;

		if(!addState("load", new CLoadState(this,Common::Data::TMP_STATE,m_root,m_renderWindow)))
			return false;

		if(!setState("load"))
			return false;

		return true;
	}

	bool CTTGApplication::secondInit()
	{
		if(!C3DApplication::secondInit()) return false;

		if(!CGameManager::init()) return false;

		if(!CMissionManager::init()) return false;

		if(!addState("menu", new CMenuState(this,Common::Data::MENU_STATE,m_root,m_renderWindow)))
			return false;

		if(!addState("galaxy", new CGalaxyState(this,Common::Data::GAME_STATE,m_root,m_renderWindow)))
			return false;

		if(!addState("pause", new CPauseState(this,Common::Data::MENU_STATE,m_root,m_renderWindow)))
			return false;

		if(!addState("event", new CEventState(this,Common::Data::MENU_STATE,m_root,m_renderWindow)))
			return false;
		
		if(!addState("system", new CSystemState(this,Common::Data::GAME_STATE,m_root,m_GUISystem,m_renderWindow),true))
			return false;

		if(!addState("planet", new CPlanetState(this,Common::Data::GAME_STATE,m_root,m_GUISystem,m_renderWindow),true))
			return false;

		if(!addState("game_over", new CGameOverState(this,Common::Data::MENU_STATE,m_root,m_renderWindow)))
			return false;

		if(!addState("game_finished", new CGameFinishedState(this,Common::Data::MENU_STATE,m_root,m_renderWindow)))
			return false;

		//Common::Save::loadGame();

		return true;

	}

	void CTTGApplication::release()
	{
		//Common::Save::saveGame();
		releaseAllStates();
		CMissionManager::release();
		CGameManager::release();
		C3DApplication::release();
	}
}