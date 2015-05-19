#include "LoadState.h"

#include <log.h>

#include <CEGUI/CEGUI.h>
#include <OGRE\OgreRoot.h>

namespace Application
{
	const char* const LOG_CLOADSTATE = "Application::CLoadState";

	bool CLoadState::init()
	{
		log_trace(LOG_CLOADSTATE,"Init\n");

		CApplicationState::init();

		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Load.layout");
		return true;
	}

	void CLoadState::release() 
	{
		log_trace(LOG_CLOADSTATE,"Release\n");

		CApplicationState::release();

	}

	void CLoadState::activate() 
	{
		log_trace(LOG_CLOADSTATE,"Activate\n");

		CApplicationState::activate();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();
		//Force render before second initialization takes place
		m_root->renderOneFrame(1/1000.0f);
		
		onActivate();

	}

	void CLoadState::deactivate() 
	{	
		log_trace(LOG_CLOADSTATE,"Deactivate\n");

		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
		
		
		CApplicationState::deactivate();

	}

	void CLoadState::tick(unsigned int msecs)
	{
		m_app->setState("menu");
	}

	void CLoadState::onActivate()
	{
		m_app->secondInit();
	}

}