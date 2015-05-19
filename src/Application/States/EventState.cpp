#include "EventState.h"

#include "../../Common/Sound/Sound.h"
#include "../Manager/MissionManager.h"
#include "../Manager/GameManager.h"

#include "State GUI\EventGUI.h"

#include <log.h>

#include <CEGUI/CEGUI.h>

namespace Application
{

	const char* const LOG_CEVENTSTATE = "Application::CEventState";

	CEventState::~CEventState()
	{
	}

	bool CEventState::init()
	{
		log_trace(LOG_CEVENTSTATE,"Init\n");

		CApplicationState::init();

		if(!CEventGUI::init(this))
			return false;

		m_guiInstance = CEventGUI::getInstance();

		m_gameManager = CGameManager::getInstance();

		return true;
	}

	void CEventState::release() 
	{
		log_trace(LOG_CEVENTSTATE,"Release\n");

		CEventGUI::release();

		CApplicationState::release();
	}

	void CEventState::activate() 
	{
		log_trace(LOG_CEVENTSTATE,"Activate\n");
		CApplicationState::activate();

		m_guiInstance->activate();
	}

	void CEventState::deactivate() 
	{		
		log_trace(LOG_CEVENTSTATE,"Deactivate\n");

		m_guiInstance->deactivate();
		
		CApplicationState::deactivate();

	}

	void CEventState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	}

	bool CEventState::keyPressed(Common::Input::TKey key)
	{
		return false;

	}

	bool CEventState::keyReleased(Common::Input::TKey key)
	{

		switch(key.id)
		{
		case Common::Input::Key::ESCAPE:
			m_app->setState("pause");
			break;
		case Common::Input::Key::RETURN:
			m_app->popState();
			break;
		default:
			return false;
		}
		return true;

	}
	
	bool CEventState::mouseMoved(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}
		
	bool CEventState::mousePressed(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool CEventState::mouseReleased(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}
		
	bool CEventState::backReleased(const CEGUI::EventArgs& e)
	{
		m_app->popState();
		m_guiInstance->resetGUI();
		return true;

	}

	void CEventState::setGameFinished()
	{
		m_app->setState("game_finished");
	}

	void CEventState::setupWindows()
	{
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("LogWindow")->getChildElement("Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CEventState::backReleased, this));


	}

	void CEventState::setupAnimatons()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* topBarAnim = animMgr.getAnimation("TopBarMoveInAnimationQuick");
		m_topBarAnimInst = animMgr.instantiateAnimation(topBarAnim);
		CEGUI::Window* topBarWindow = m_menuWindow->getChild("TopBar");
		m_topBarAnimInst->setTargetWindow(topBarWindow);

		CEGUI::Animation* botBarAnim = animMgr.getAnimation("BotBarMoveInAnimationQuick");
		m_botBarAnimInst = animMgr.instantiateAnimation(botBarAnim);
		CEGUI::Window* botBarWindow = m_menuWindow->getChild("BotBar");
		m_botBarAnimInst->setTargetWindow(botBarWindow);

		CEGUI::Animation* insideAnim = animMgr.getAnimation("InsideBlendInQuick");
		m_insideAnimInst = animMgr.instantiateAnimation(insideAnim);
		CEGUI::Window* insideWindows = m_menuWindow->getChild("InnerButtonsContainer");
		m_insideAnimInst->setTargetWindow(insideWindows);

		CEGUI::Animation* windowAnim = animMgr.getAnimation("OpenWindow");
		m_windowAnimInst = animMgr.instantiateAnimation(windowAnim);
		CEGUI::Window* buttonWindow1 = m_menuWindow->getChild("InnerButtonsContainer/LogWindow");
	}

	void CEventState::onActivate()
	{
		m_topBarAnimInst->start();
		m_botBarAnimInst->start();
		m_insideAnimInst->start();
		m_windowAnimInst->start();
	}
}