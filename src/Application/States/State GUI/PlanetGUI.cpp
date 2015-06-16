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

#include "PlanetGUI.h"

#include "../PlanetState.h"

#include "../../Manager/GameManager.h"
#include "../../../Common/Data/Game_Constants.h"
#include "../../../Common/Sound/Sound.h"
#include "MissionNotification.h"

#include <log.h>

#include <OGRE\OgreTexture.h>
#include <OGRE\OgreTextureManager.h>
#include <OGRE\OgreRoot.h>

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/AnimationManager.h>
#include <CEGUI/AnimationInstance.h>
#include <CEGUI/widgets/Listbox.h>
#include <CEGUI\widgets\ListboxTextItem.h>
#include <CEGUI\widgets\ProgressBar.h>
#include <CEGUI\Texture.h>
#include <CEGUI\ImageManager.h>
#include <CEGUI\BasicImage.h>
#include <CEGUI\widgets\Listbox.h>
#include <CEGUI\widgets\ListboxTextItem.h>
#include <CEGUI\RendererModules\Ogre\Renderer.h>

namespace Application
{
	const char* const LOG_CPLANETGUI = "Application::CPlanetGUI";
	CPlanetGUI* CPlanetGUI::m_instance = nullptr;

	CPlanetGUI::CPlanetGUI(CEGUI::System* sys, Ogre::Root* root, CPlanetState* state):m_menuWindow(nullptr), m_image(nullptr), m_ceguiSystem(sys), m_blackScreenFadeOut(nullptr), m_blackScreenFadeIn(nullptr),
		m_planetGUIScreenFadeOut(nullptr), m_planetGUIScreenFadeIn(nullptr), m_minimapImageScreenFadeOut(nullptr), m_minimapImageScreenFadeIn(nullptr),
		m_buttonFadeOut(nullptr), m_buttonFadeIn(nullptr), m_mgrInstance(nullptr), m_root(root), m_planetState(state), m_backLockCount(0), m_buttonLandFadeOut(nullptr),
		m_messageFadeOut(nullptr)
	{
		m_instance = this;

		m_mgrInstance = CGameManager::getInstance();

		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Planet.layout");

		setupEvents();
		setupAnimations();

		//Sounds
		Common::Sound::CSound::getSingletonPtr()->addSound("ButtonSound1.wav","backButtonSound");

	}

	
	CPlanetGUI::~CPlanetGUI()
	{
		Common::Sound::CSound::getSingletonPtr()->releaseSound("backButtonSound");
		m_instance = nullptr;
	}

	bool CPlanetGUI::init(CEGUI::System* sys, Ogre::Root* root, CPlanetState* state)
	{
		assert(!m_instance && "Second initialization of Application::CPlanetGUI is not allowed!");

		log_trace(LOG_CPLANETGUI,"Init\n");

		new CPlanetGUI(sys, root, state);

		return true;
	}

	void CPlanetGUI::release()
	{
		/*assert(m_instance && "Application::CPlanetGUI hasn't been initialized!");*/

		log_trace(LOG_CPLANETGUI,"Release\n");

		if(m_instance)
		{
			delete m_instance;
		}
	}

	void CPlanetGUI::activate()
	{
		log_trace(LOG_CPLANETGUI,"Activate\n");

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();

		setupHUD();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

		Ogre::TexturePtr tex = m_root->getTextureManager()->getByName("minimap_camerarender_texture");
		CEGUI::Texture &guiTex = static_cast<CEGUI::OgreRenderer *>(m_ceguiSystem->getRenderer())->createTexture("minimapTexture", tex);
		const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), guiTex.getOriginalDataSize());
		m_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", "MinimapImage"));
		m_image->setTexture(&guiTex);
		m_image->setArea(rect);
		m_image->setAutoScaled(CEGUI::ASM_Both);

		m_menuWindow->getChild("PlanetHUD/Radar")->setProperty("Image","MinimapImage");

		CMissionNotification::getInstance()->activate(m_menuWindow);

		m_blackScreenFadeOut->start();
		m_buttonFadeIn->start();
		m_planetGUIScreenFadeIn->start();
		m_minimapImageScreenFadeIn->start();

	}

	void CPlanetGUI::deactivate()
	{
		log_trace(LOG_CPLANETGUI,"Deactivate\n");

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();

		CMissionNotification::getInstance()->deactivate();

		CEGUI::ImageManager::getSingleton().destroy("MinimapImage");
		m_ceguiSystem->getRenderer()->destroyTexture("minimapTexture");

		deactivateGUI();

		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
	}

	void CPlanetGUI::tick(unsigned int msecs)
	{
		static_cast<CEGUI::ProgressBar*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/Hull"))->
			setProgress((float)m_mgrInstance->getLife()/(float)m_mgrInstance->getTotalLife());

		static_cast<CEGUI::ProgressBar*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/Shield"))->
            setProgress((float)m_mgrInstance->m_curShield/(float)m_mgrInstance->m_maxShield);

		static_cast<CEGUI::ProgressBar*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/Energy"))->
            setProgress((float)m_mgrInstance->m_curEnergy/(float)m_mgrInstance->m_maxEnergy);

		// Uncomment this to show the FPS count

		/*if(static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->getItemCount() < 2)
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->addItem(
				new CEGUI::ListboxTextItem("FPS:" + std::to_string((1/(float)msecs)*1000) + "\n")
			);
		else
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->getListboxItemFromIndex(2)->setText(
				"FPS:" + std::to_string((1/(float)msecs)*1000) + "\n"
			);

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->invalidate();*/
	}

	void CPlanetGUI::setupAnimations()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* blackScreenFadeOut = animMgr.getAnimation("BlackWindowFadeOut");
		m_blackScreenFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_blackScreenFadeOut->setTargetWindow(m_menuWindow->getChild("BlackScreen"));

		m_buttonFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_buttonFadeOut->setTargetWindow(m_menuWindow->getChild("PlanetHUD/LeaveButton"));

		m_buttonLandFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_buttonLandFadeOut->setTargetWindow(m_menuWindow->getChild("PlanetHUD/PlanetButton"));

		m_minimapImageScreenFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_minimapImageScreenFadeOut->setTargetWindow(m_menuWindow->getChild("PlanetHUD/Radar"));

		m_messageFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_messageFadeOut->setTargetWindow(m_menuWindow->getChild("PlanetHUD/MessageBoard"));

		CEGUI::Animation* blackScreenFadeIn = animMgr.getAnimation("BlackWindowFadeIn");
		m_blackScreenFadeIn = animMgr.instantiateAnimation(blackScreenFadeIn);
		m_blackScreenFadeIn->setTargetWindow(m_menuWindow->getChild("BlackScreen"));

		m_buttonFadeIn = animMgr.instantiateAnimation(blackScreenFadeIn);
		m_buttonFadeIn->setTargetWindow(m_menuWindow->getChild("PlanetHUD/LeaveButton"));

		m_minimapImageScreenFadeIn = animMgr.instantiateAnimation(blackScreenFadeIn);
		m_minimapImageScreenFadeIn->setTargetWindow(m_menuWindow->getChild("PlanetHUD/Radar"));

		CEGUI::Animation* partialFadeOut = animMgr.getAnimation("PartialFadeOut");
		m_planetGUIScreenFadeOut = animMgr.instantiateAnimation(partialFadeOut);
		m_planetGUIScreenFadeOut->setTargetWindow(m_menuWindow->getChild("PlanetHUD/InfoBoard"));

		CEGUI::Animation* partialFadeIn = animMgr.getAnimation("PartialFadeIn");
		m_planetGUIScreenFadeIn = animMgr.instantiateAnimation(partialFadeIn);
		m_planetGUIScreenFadeIn->setTargetWindow(m_menuWindow->getChild("PlanetHUD/InfoBoard"));
	}

	void CPlanetGUI::setupHUD()
	{
		CGameManager::TActiveEquipment equipment = m_mgrInstance->getPlayerActiveEquipment();

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/PrimaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(Common::Data::Game::GAME_PRIMARY_WEAPON));
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/PrimaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(equipment[Common::Data::Game::GAME_PRIMARY_WEAPON].first));

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/PrimaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(Common::Data::Game::GAME_SECONDARY_WEAPON));
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/PrimaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(equipment[Common::Data::Game::GAME_SECONDARY_WEAPON].first));

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/PrimaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(">>" + std::string(Common::Data::Game::GAME_MISSION_NAME) + ":"));
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/PrimaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_MISSION_TEXT) + " " + 
			std::to_string(CGameManager::getInstance()->getObjectivesAquired()) + "/" + 
			std::to_string(CGameManager::getInstance()->getTotalObjectives())));

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(Common::Data::Game::GAME_ENGINE));
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(equipment[Common::Data::Game::GAME_ENGINE].first));

		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(std::string(Common::Data::Game::GAME_SENSORS)));
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->addItem(
			new CEGUI::ListboxTextItem(equipment[Common::Data::Game::GAME_SENSORS].first));

		if(CGameManager::getInstance()->getMission().first != 0){
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->addItem(
				new CEGUI::ListboxTextItem(Common::Data::Game::GAME_HUD_NOTES));
			static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->addItem(
				new CEGUI::ListboxTextItem(CGameManager::getInstance()->getMission().second));
		}

		for(auto it = equipment.begin(); it != equipment.end(); ++it){
			if (it->first != Common::Data::Game::GAME_PRIMARY_WEAPON && it->first != Common::Data::Game::GAME_SECONDARY_WEAPON &&
				it->first != Common::Data::Game::GAME_ENGINE && it->first != Common::Data::Game::GAME_SENSORS)
			{
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/Others"))->addItem(
					new CEGUI::ListboxTextItem(it->first));
				static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/Others"))->addItem(
					new CEGUI::ListboxTextItem(it->second.first));
			}
		}
	}

	void CPlanetGUI::deactivateGUI()
	{
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/PrimaryWindow"))->resetList();
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/SecondaryWindow"))->resetList();
		static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetHUD/InfoBoard/Others"))->resetList();
	}

	void CPlanetGUI::setupEvents()
	{
		m_menuWindow->getChild("PlanetHUD/LeaveButton")->subscribeEvent(CEGUI::Window::EventMouseClick,
			CEGUI::SubscriberSlot(&CPlanetGUI::onBackClicked, this));
		m_menuWindow->getChild("PlanetHUD/PlanetButton")->subscribeEvent(CEGUI::Window::EventMouseClick,
			CEGUI::SubscriberSlot(&CPlanetGUI::onLandClicked, this));
	}

	bool CPlanetGUI::onBackClicked(const CEGUI::EventArgs& e)
	{
		m_menuWindow->getChild("BlackScreen")->subscribeEvent(CEGUI::AnimationInstance::EventAnimationEnded,
			CEGUI::SubscriberSlot(&CPlanetState::goBack,m_planetState));

		m_blackScreenFadeIn->start();
		m_buttonFadeOut->start();
		m_planetGUIScreenFadeOut->start();
		m_minimapImageScreenFadeOut->start();
		if(m_menuWindow->getChild("PlanetHUD/PlanetButton")->isVisible())
			m_buttonLandFadeOut->start();
		if(m_menuWindow->getChild("PlanetHUD/MessageBoard")->isVisible())
			m_messageFadeOut->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("backButtonSound");

		return true;
	}

	bool CPlanetGUI::onLandClicked(const CEGUI::EventArgs& e)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("backButtonSound");
		m_planetState->setEvent();

		return true;
	}

	void CPlanetGUI::shipDestroyed()
	{
		m_menuWindow->getChild("BlackScreen")->subscribeEvent(CEGUI::AnimationInstance::EventAnimationEnded,
			CEGUI::SubscriberSlot(&CPlanetState::setGameOver,m_planetState));

		m_blackScreenFadeIn->start();
		m_buttonFadeOut->start();
		m_planetGUIScreenFadeOut->start();
		m_minimapImageScreenFadeOut->start();
		if(m_menuWindow->getChild("PlanetHUD/PlanetButton")->isVisible())
			m_buttonLandFadeOut->start();
		if(m_menuWindow->getChild("PlanetHUD/MessageBoard")->isVisible())
			m_messageFadeOut->start();
	}

	void CPlanetGUI::addLock()
	{
		++m_backLockCount;

		if(m_backLockCount > 0 && !m_menuWindow->getChild("PlanetHUD/LeaveButton")->isDisabled())
			m_menuWindow->getChild("PlanetHUD/LeaveButton")->disable();

		if(m_backLockCount > 0 && !m_menuWindow->getChild("PlanetHUD/PlanetButton")->isDisabled())
			m_menuWindow->getChild("PlanetHUD/PlanetButton")->disable();

	}

	void CPlanetGUI::disableAccess()
	{
		m_menuWindow->getChild("PlanetHUD/PlanetButton")->disable();
	}

	void CPlanetGUI::releaseLock()
	{
		if(m_backLockCount > 0)
			--m_backLockCount;
		
		if(m_backLockCount == 0 && m_menuWindow->getChild("PlanetHUD/LeaveButton")->isDisabled())
			m_menuWindow->getChild("PlanetHUD/LeaveButton")->enable();

		if(m_backLockCount == 0 && m_menuWindow->getChild("PlanetHUD/PlanetButton")->isDisabled())
			m_menuWindow->getChild("PlanetHUD/PlanetButton")->enable();
	}

	void CPlanetGUI::activateEnterButton()
	{
		m_menuWindow->getChild("PlanetHUD/PlanetButton")->setVisible(true);
	}

	void CPlanetGUI::deactivateEnterButton()
	{
		m_menuWindow->getChild("PlanetHUD/PlanetButton")->setVisible(false);
	}

	void CPlanetGUI::showWarning()
	{
		m_menuWindow->getChild("PlanetHUD/MessageBoard")->setVisible(true);
	}

	void CPlanetGUI::hideWarning()
	{
		m_menuWindow->getChild("PlanetHUD/MessageBoard")->setVisible(false);
	}
}