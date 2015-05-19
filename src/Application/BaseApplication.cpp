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

#include "BaseApplication.h"

#include <log.h>

#include <Common/Configure/Configure.h>
#include "States/ApplicationState.h"
#include "Manager/GameManager.h"
#include "States\State GUI\EventGUI.h"
#include "Manager/MissionManager.h"
#include "CLock.h"

#include <Common/Data/TTG_Types.h>
#include <Common/Map/Map.h>

namespace Application 
{
	const char* LOG_CBASEAPP = "Application::CBaseApplication";

	CBaseApplication* CBaseApplication::m_instance = 0;

	CBaseApplication::CBaseApplication() : m_initialized(false),
										  m_currentState(0),
										  m_nextState(0),
										  m_exit(false),
										  m_root(nullptr),
										  m_renderWindow(nullptr),
										  m_clock(nullptr)
	{
		m_instance = this;
	}

	CBaseApplication::~CBaseApplication()
	{
		m_instance = 0;
	}

	bool CBaseApplication::init()
	{
		if(m_initialized) return false;

		return m_initialized = true;
	}

	void CBaseApplication::release()
	{
		releaseAllStates();
		m_initialized = false;
	}

	void CBaseApplication::releaseAllStates()
	{
		if(m_currentState){
			m_currentState->deactivate();
			m_currentState = 0;
		}
		
		for (auto it(m_states.begin()); it != m_states.end(); ++it) {
			it->second->release();
			delete it->second;
		}

		//This is necessary
		m_states.clear();
		while (!m_stackState.empty())
			m_stackState.pop();

	}

	void CBaseApplication::run()
	{
        m_clock->updateTime();

		while (!exitRequested()) {
			if (!m_currentState || (m_nextState && (m_currentState != m_nextState)))
				changeState();

            m_clock->updateTime();

            tick(m_clock->getLastFrameDuration());
		}
	}

    bool CBaseApplication::addState(const std::string &name, CApplicationState* state, bool asleep)
	{
		if (m_states.find(name) != m_states.end()) {
			log_fatal(LOG_CBASEAPP,"Duplicated State!\n");
        }
		
        m_states[name] = state;

		return asleep ? true : state->init();
	}

	bool CBaseApplication::setState(const std::string &name)
	{
		auto it(m_states.find(name));
		if (it == m_states.end()) 
			return false;

		m_nextState = it->second;
		//Temoral states are not added to the stack because they are supposed to activate the actual state and die
		if(m_nextState->getType() != Common::Data::GameType::TMP_STATE)
			m_stackState.push(m_nextState);

		return true;
	}

    bool CBaseApplication::popState()
    {
        if (m_stackState.size() < 2)
            return false;

        m_stackState.pop();
        m_nextState = m_stackState.top();
        return true;
    }

    bool CBaseApplication::resetToMenuState(const std::string& initialGameState)
    {
        // MenuState is supposed to be at the bottom of the stack
        while (m_stackState.size() != 1) {

			//Here all game state scenes are destroyed
			if(m_stackState.top()->getType()==Common::Data::GAME_STATE) 
			{
				m_stackState.top()->release();
			}
			else
				m_stackState.top()->deactivate();

            popState();
        }

		//Here we reset the procedural scene generation
		Map::CMap::getInstance()->resetMaps();

		//Here we reset the missions
		CMissionManager::getInstance()->abandonMission();

		//Here the event state gui is resetted
		CEventGUI::getInstance()->resetGUI();

		//Here we reset the state of the game
		CGameManager::getInstance()->resetGame();

		//We have to create the scene of the first game state of the application 
		m_states[initialGameState]->init();

		return true;
    }

	unsigned int CBaseApplication::getApptime()
	{
		return m_clock->getTime();
	}

	void CBaseApplication::changeState()
	{
		if (m_currentState) {
			//When a games state is changed for another game state is necesary to destroy the scene of the first and create the new scene
            if (m_currentState->getType() ==  Common::Data::GAME_STATE && 
				m_nextState->getType() == Common::Data::GAME_STATE) {
                m_currentState->deactivate();
                m_currentState->release();
                
                m_nextState->init();
            }
            else
			    m_currentState->deactivate();
		}

		m_nextState->activate();
		m_currentState = m_nextState;
	}

	void CBaseApplication::tick(unsigned int msecs)
	{
		if (m_currentState){
			m_currentState->tick(msecs);
		}
	}

	bool CBaseApplication::keyPressed(Common::Input::TKey key)
	{
		return m_currentState ? m_currentState->keyPressed(key) : false;
	}

	bool CBaseApplication::keyReleased(Common::Input::TKey key)
	{
		return m_currentState ? m_currentState->keyReleased(key) : false;
	}

	bool CBaseApplication::mouseMoved(const GUI::InputListener::CMouseState &mouseState)
	{
		return m_currentState ? m_currentState->mouseMoved(mouseState) : false;
	}

	bool CBaseApplication::mousePressed(const GUI::InputListener::CMouseState &mouseState)
	{
		return m_currentState ? m_currentState->mousePressed(mouseState) : false;
	}

	bool CBaseApplication::mouseReleased(const GUI::InputListener::CMouseState &mouseState)
	{
		return m_currentState ? m_currentState->mouseReleased(mouseState) : false;
	}
}
