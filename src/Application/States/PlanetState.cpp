#include "PlanetState.h"

#include "../../Logic/Logic.h"
#include "../../Logic/EntityFactory.h"
#include "../../Logic/Scene/Scene.h"

#include "../3DApplication.h"
#include "../Manager/GameManager.h"
#include "../../Common/Data/Game_Constants.h"

#include "../../GUI/GUI.h"

#include "State GUI\PlanetGUI.h"
#include "State GUI\EventGUI.h"

#include <log.h>


namespace Application
{
	const char* const LOG_CPLANETSTATE = "Application::CPlanetState";

	bool CPlanetState::init() 
	{
		log_trace(LOG_CPLANETSTATE,"Init\n");

		CApplicationState::init();

		if (!Logic::CEntityFactory::getInstance()->loadBluePrints("planetBlueprints.xml"))
			return false;

		m_mgrInstance = CGameManager::getInstance();

		std::string str(m_mgrInstance->getSystem().length()>1?"00"+
			m_mgrInstance->getSystem():"000"+m_mgrInstance->getSystem());
		str += "_"+m_mgrInstance->getPlanet();

		if (!Logic::CLogic::getInstance()->loadLevel("0001_"+str,m_root,
			m_renderWindow,Common::Data::BATTLE))
			return false;

		if(!CPlanetGUI::init(m_ceguiSystem,m_root, this))
			return false;

		m_guiInstance = CPlanetGUI::getInstance();

		return true;
	}//init

	void CPlanetState::release() 
	{
		log_trace(LOG_CPLANETSTATE,"Release\n");

		CPlanetGUI::release();

		Logic::CLogic::getInstance()->unloadLevel();

		Logic::CEntityFactory::getInstance()->unloadBluePrints();

		CApplicationState::release();
	}

	void CPlanetState::activate() 
	{
		log_trace(LOG_CPLANETSTATE,"Activate\n");

		CApplicationState::activate();

		Logic::CLogic::getInstance()->activateScene();

		m_guiInstance->activate();

		CEventGUI::getInstance()->reactivateEventGUI();

	}

	void CPlanetState::deactivate() 
	{
		log_trace(LOG_CPLANETSTATE,"Deactivate\n");

		m_guiInstance->deactivate();

		Logic::CLogic::getInstance()->deactivateScene();

		CApplicationState::deactivate();
	}

	void CPlanetState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		Logic::CLogic::getInstance()->tick(msecs);

		m_guiInstance->tick(msecs);
	}

	bool CPlanetState::keyPressed(Common::Input::TKey key)
	{
		return false;

	}

	bool CPlanetState::keyReleased(Common::Input::TKey key)
	{
		switch(key.id)
		{
		case Common::Input::Key::ESCAPE:
			m_app->setState("pause");
			break;
		case Common::Input::Key::G:
			/*m_app->popState();*/
			break;
		case Common::Input::Key::E:
			/*m_app->setState("event");*/
			break;
		default:
			return false;
		}
		return true;

	}
	
	bool CPlanetState::mouseMoved(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}
		
	bool CPlanetState::mousePressed(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool CPlanetState::mouseReleased(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool CPlanetState::goBack(const CEGUI::EventArgs& e)
	{
		m_app->popState();

		return true;
	}

	bool CPlanetState::setGameOver(const CEGUI::EventArgs& e)
	{
		m_app->setState("game_over");

		return true;
	}

	bool CPlanetState::setEvent()
	{
		m_app->setState("event");
		m_guiInstance->disableAccess();
		return true;
	}

}