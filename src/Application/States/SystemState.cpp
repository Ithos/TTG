#include "SystemState.h"

#include "../../Logic/Logic.h"
#include "../../Logic/EntityFactory.h"
#include "../../Logic/Scene/Scene.h"

#include "../3DApplication.h"
#include "../Manager/GameManager.h"
#include "../Manager/MissionManager.h"

#include "../../GUI/GUI.h"

#include <Common/Sound/Sound.h>
#include <Common/Data/Spawn_Constants.h>
#include <Common/Data/Game_Constants.h>

#include <log.h>

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreRay.h>
#include <OGRE\OgreSceneQuery.h>
#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>
#include <OGRE\OgreAny.h>
#include <OGRE\OgreTexture.h>
#include <OGRE\OgreTextureManager.h>

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/AnimationManager.h>
#include <CEGUI/AnimationInstance.h>
#include <CEGUI\Texture.h>
#include <CEGUI\ImageManager.h>
#include <CEGUI\BasicImage.h>
#include <CEGUI\widgets\Listbox.h>
#include <CEGUI\widgets\ListboxTextItem.h>
#include <CEGUI\RendererModules\Ogre\Renderer.h>

#include "../../Common/Sound/Sound.h"

const Ogre::Vector3 offset(2.5,2.5,2.5);

namespace Application
{
	const char* const LOG_CSYSTEMSTATE = "Application::CSystemState";

	bool CSystemState::init() 
	{
		log_trace(LOG_CSYSTEMSTATE,"Init\n");

		CApplicationState::init();

		if (!Logic::CEntityFactory::getInstance()->loadBluePrints("systemBlueprints.xml"))	
			return false;

		std::string str(CGameManager::getInstance()->getSystem());

		if(CGameManager::getInstance()->isTargetSystem()){
			if (!Logic::CLogic::getInstance()->loadLevel(str.length() > 1 ? "0001_10" + str : "0001_100" + str ,m_root,m_renderWindow,Common::Data::SYSTEM))
				return false;
		}else{
			if (!Logic::CLogic::getInstance()->loadLevel(str.length() > 1 ? "0001_00" + str : "0001_000" + str ,m_root,m_renderWindow,Common::Data::SYSTEM))
				return false;
		}
		//Music & sounds
		Common::Sound::CSound::getSingletonPtr()->addSound("Button4.wav","planetSlected");
		Common::Sound::CSound::getSingletonPtr()->addSound("ButtonSound1.wav","planetButtonHover");
		Common::Sound::CSound::getSingletonPtr()->addSound("ButtonHover1.wav","systemHUDButton");

		//CEGUI
		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("System.layout");

		setupEvents();
		setupAnimations();


		return true;
	}//init

	void CSystemState::release() 
	{
		log_trace(LOG_CSYSTEMSTATE,"Release\n");

		Common::Sound::CSound::getSingletonPtr()->releaseSound("planetSlected");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("planetButtonHover");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("systemHUDButton");

		Logic::CLogic::getInstance()->unloadLevel();

		Logic::CEntityFactory::getInstance()->unloadBluePrints();

		CApplicationState::release();
	}

	void CSystemState::activate() 
	{
		log_trace(LOG_CSYSTEMSTATE,"Activate\n");

		CApplicationState::activate();

		Logic::CLogic::getInstance()->activateScene();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

		m_RayScnQuery = Logic::CLogic::getInstance()->getScene()->getSceneManager()->createRayQuery(Ogre::Ray());

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();

		Ogre::TexturePtr tex = m_root->getTextureManager()->getByName("texture_camerarender_texture");
		CEGUI::Texture &guiTex = static_cast<CEGUI::OgreRenderer *>(m_ceguiSystem->getRenderer())->createTexture("systemTexture", tex);
		const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), guiTex.getOriginalDataSize());
		m_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", "RTTImage"));
		m_image->setTexture(&guiTex);
		m_image->setArea(rect);
		m_image->setAutoScaled(CEGUI::ASM_Both);

		m_menuWindow->getChild("PlanetWindow/PlanetImage")->setProperty("Image","RTTImage");

		CGameManager::getInstance()->activateHUD(m_menuWindow);
		CGameManager::getInstance()->setIsTargetPlanet(false);

		m_blackScreenFadeOut->start();
		m_sytemGUIScreenFadeIn->start();
		m_planetImageScreenFadeIn->start();

		CMissionManager::getInstance()->checkSystem();

		if(CGameManager::getInstance()->isTargetSystem())
			CGameManager::getInstance()->showTargetMessage();

	}

	void CSystemState::deactivate() 
	{
		log_trace(LOG_CSYSTEMSTATE,"Deactivate\n");

		if(CGameManager::getInstance()->isTargetSystem())
			CGameManager::getInstance()->hideTargetMessage();

		CEGUI::ImageManager::getSingleton().destroy("RTTImage");
		m_ceguiSystem->getRenderer()->destroyTexture("systemTexture");

		CGameManager::getInstance()->deactivateHUD();

		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);

		if(m_PointedPlanet){
			static_cast<Ogre::SceneNode*>(m_PointedPlanet->getChild(m_PointedPlanet->getName()+"OutlineGlow"))->setVisible(false);
			m_PointedPlanet = nullptr;
		}

		if(m_CurrentPlanet){
			static_cast<Ogre::SceneNode*>(m_CurrentPlanet->getChild(m_CurrentPlanet->getName()+"OutlineGreenGlow"))->setVisible(false);
			m_CurrentPlanet = nullptr;
		}

		Logic::CLogic::getInstance()->getScene()->getSceneManager()->destroyQuery(m_RayScnQuery);

		m_RayScnQuery = nullptr;

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();

		Logic::CLogic::getInstance()->deactivateScene();

		CApplicationState::deactivate();
	}

	void CSystemState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		Logic::CLogic::getInstance()->tick(msecs);

		m_menuWindow->getChild("PlanetWindow/PlanetImage")->invalidate(true);

	}

	bool CSystemState::keyPressed(Common::Input::TKey key)
	{
		return false;

	}

	bool CSystemState::keyReleased(Common::Input::TKey key)
	{
		switch(key.id)
		{
		case Common::Input::Key::ESCAPE:
			m_app->setState("pause");
			break;
		case Common::Input::Key::G:
			//m_app->setState("planet");
			//onAccessClicked(CEGUI::EventArgs());
			break;
		case Common::Input::Key::R:
			//m_app->popState();
			//onBackClicked(CEGUI::EventArgs());
			break;
		case Common::Input::Key::E:
			//m_app->setState("event");
			break;
		default:
			return false;
		}
		return true;

	}
	
	bool CSystemState::mouseMoved(const GUI::InputListener::CMouseState &mouseState)
	{
		bool playSound(m_PointedPlanet ?false:true);
		if(m_PointedPlanet){
			static_cast<Ogre::SceneNode*>(m_PointedPlanet->getChild(m_PointedPlanet->getName()+"OutlineGlow"))->setVisible(false);
			m_PointedPlanet = nullptr;
		}else{
			m_PointedPlanet = nullptr;
		}

		CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
		Ogre::Ray mouseRay = Logic::CLogic::getInstance()->getScene()->getSceneCamera()->getCameraToViewportRay(mousePos.d_x/float(mouseState.width),mousePos.d_y/float(mouseState.height));
		m_RayScnQuery->setRay(mouseRay);
		m_RayScnQuery->setSortByDistance(true);

		Ogre::RaySceneQueryResult &queryResult = m_RayScnQuery->execute();

		for(auto it = queryResult.begin(); it != queryResult.end(); ++it){
			if(it->movable && it->movable->getName() != "" 
				&& it->movable->getName() != Logic::CLogic::getInstance()->getScene()->getSceneCamera()->getName())
			{
				m_PointedPlanet = it->movable->getParentSceneNode();
				if(m_PointedPlanet!=m_CurrentPlanet && m_PointedPlanet->getAttachedObject(0)->getQueryFlags() == Common::Data::PLANET_MASK)
				{
					static_cast<Ogre::SceneNode*>(m_PointedPlanet->getChild(m_PointedPlanet->getName()+"OutlineGlow"))->setVisible(true);
					if(playSound)
						Common::Sound::CSound::getSingletonPtr()->playSound("planetButtonHover");
					break;
				}
				else
				{
					m_PointedPlanet = nullptr;
				}
			}
		}

		return false;

	}
		
	bool CSystemState::mousePressed(const GUI::InputListener::CMouseState &mouseState)
	{
		if(m_CurrentPlanet){
			static_cast<Ogre::SceneNode*>(m_CurrentPlanet->getChild(m_CurrentPlanet->getName()+"OutlineGreenGlow"))->setVisible(false);
			m_CurrentPlanet = nullptr;
		}

		if(mouseState.button == Common::Input::Button::LEFT){
			
			CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
			Ogre::Ray mouseRay = Logic::CLogic::getInstance()->getScene()->getSceneCamera()->getCameraToViewportRay(mousePos.d_x/float(mouseState.width),mousePos.d_y/float(mouseState.height));
			m_RayScnQuery->setRay(mouseRay);
			m_RayScnQuery->setSortByDistance(true);

			Ogre::RaySceneQueryResult &queryResult = m_RayScnQuery->execute();

			for(auto it = queryResult.begin(); it != queryResult.end(); ++it){
				if(it->movable && it->movable->getName() != "" 
					&& it->movable->getName() != Logic::CLogic::getInstance()->getScene()->getSceneCamera()->getName())
				{
					m_CurrentPlanet = it->movable->getParentSceneNode();
					if(m_CurrentPlanet->getAttachedObject(0)->getQueryFlags() == Common::Data::PLANET_MASK){
						//Get information from the planet
						std::string str = Ogre::any_cast<Ogre::String>(m_CurrentPlanet->getUserAny());
						if(str.substr(0,1) == Common::Data::Spawn::COMMON_TARGET)
							CGameManager::getInstance()->setIsTargetPlanet(true);
						else
							CGameManager::getInstance()->setIsTargetPlanet(false);

						if(str.substr(1,1) == Common::Data::Spawn::COMMON_INHABITED)
							CGameManager::getInstance()->setInhabitedPlanet(true);
						else
							CGameManager::getInstance()->setInhabitedPlanet(false);

						CGameManager::getInstance()->setPlanet(str.substr(2,6));
						Logic::CLogic::getInstance()->getScene()->getSceneManager()->getCamera("texture_camera")->detachFromParent();
						m_CurrentPlanet->attachObject(Logic::CLogic::getInstance()->getScene()->getSceneManager()->getCamera("texture_camera"));
						Logic::CLogic::getInstance()->getScene()->getSceneManager()->getCamera("texture_camera")->setAutoTracking(true,m_CurrentPlanet);
						Logic::CLogic::getInstance()->getScene()->getSceneManager()->getCamera("texture_camera")->setPosition(
							m_CurrentPlanet->getScale().x*offset
							);
						static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetWindow/PlanetBoard"))->resetList();
						std::string tmpstr(str.substr(6,1));
						std::string additionalInfo("");
						if(CGameManager::getInstance()->getSensorLevel() > 0){
							std::string risk = str.substr(7,1);
							if( risk == "0"){
								additionalInfo += std::string(Common::Data::Game::GAME_PLANET_NO_RISK);
							}else if (risk == "1"){
								additionalInfo += std::string(Common::Data::Game::GAME_PLANET_LOW_RISK);
							}else if (risk == "2"){
								additionalInfo += std::string(Common::Data::Game::GAME_PLANET_MEDIUM_RISK);
							}else if (risk == "3"){
								additionalInfo += std::string(Common::Data::Game::GAME_PLANET_HIGH_RISK);
							}else if (risk == "4"){
								additionalInfo += std::string(Common::Data::Game::GAME_PLANET_VERY_HIGH_RISK);
							}

							additionalInfo += "  ";

						}
						if(CGameManager::getInstance()->getSensorLevel() > 1){
							if(CGameManager::getInstance()->isInhabitedPlanet())
								additionalInfo += std::string(Common::Data::Game::GAME_PLANET_INHABITED);
							else
								additionalInfo += std::string(Common::Data::Game::GAME_PLANET_UNNOCUPIED);

							additionalInfo += "  ";
						}
						if(CGameManager::getInstance()->getSensorLevel() > 2){
							if(CGameManager::getInstance()->isTargetPlanet()){
								additionalInfo += std::string(Common::Data::Game::GAME_PLANET_SIGNAL);
								additionalInfo += "  ";
							}
						}
						static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetWindow/PlanetBoard"))->addItem(new CEGUI::ListboxTextItem(
							Common::Data::Game::GAME_PLANETS_NAMES[std::atoi(CGameManager::getInstance()->getSystem().c_str())][std::atoi(str.substr(6,1).c_str())]
						+ std::string("\n") + additionalInfo + std::string("\n") + str.substr(8)
							));
						m_menuWindow->getChild("PlanetWindow/PlanetButton")->enable();
						Common::Sound::CSound::getSingletonPtr()->playSound("planetSlected");
						break;
					}else
						m_CurrentPlanet = nullptr;
						Logic::CLogic::getInstance()->getScene()->getSceneManager()->getCamera("texture_camera")->detachFromParent();
						Logic::CLogic::getInstance()->getScene()->getSceneManager()->getSceneNode("texture_cameranode_camera")->attachObject(
							Logic::CLogic::getInstance()->getScene()->getSceneManager()->getCamera("texture_camera")
							);
						Logic::CLogic::getInstance()->getScene()->getSceneManager()->getCamera("texture_camera")->setPosition(Ogre::Vector3::ZERO);
						Logic::CLogic::getInstance()->getScene()->getSceneManager()->getCamera("texture_camera")->setAutoTracking(false);
						m_menuWindow->getChild("PlanetWindow/PlanetButton")->disable();
						static_cast<CEGUI::Listbox*>(m_menuWindow->getChild("PlanetWindow/PlanetBoard"))->resetList();
				}
			}
		}

		if(m_CurrentPlanet){
			static_cast<Ogre::SceneNode*>(m_CurrentPlanet->getChild(m_CurrentPlanet->getName()+"OutlineGreenGlow"))->setVisible(true);
		}

		return false;

	}

	bool CSystemState::mouseReleased(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	void CSystemState::setupAnimations()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* blackScreenFadeOut = animMgr.getAnimation("BlackWindowFadeOut");
		m_blackScreenFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_blackScreenFadeOut->setTargetWindow(m_menuWindow->getChild("BlackScreen"));

		m_sytemGUIScreenFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_sytemGUIScreenFadeOut->setTargetWindow(m_menuWindow->getChild("PlanetWindow"));

		m_planetImageScreenFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_planetImageScreenFadeOut->setTargetWindow(m_menuWindow->getChild("PlanetWindow/PlanetImage"));

		CEGUI::Animation* partialFadeOut = animMgr.getAnimation("PartialFadeOut");
		m_sytemGUIScreenFadeOut = animMgr.instantiateAnimation(partialFadeOut);
		m_sytemGUIScreenFadeOut->setTargetWindow(m_menuWindow->getChild("PlanetWindow"));

		CEGUI::Animation* blackScreenFadeIn = animMgr.getAnimation("BlackWindowFadeIn");
		m_blackScreenFadeIn = animMgr.instantiateAnimation(blackScreenFadeIn);
		m_blackScreenFadeIn->setTargetWindow(m_menuWindow->getChild("BlackScreen"));

		m_planetImageScreenFadeIn = animMgr.instantiateAnimation(blackScreenFadeIn);
		m_planetImageScreenFadeIn->setTargetWindow(m_menuWindow->getChild("PlanetWindow/PlanetImage"));

		CEGUI::Animation* partialFadeIn = animMgr.getAnimation("PartialFadeIn");
		m_sytemGUIScreenFadeIn = animMgr.instantiateAnimation(partialFadeIn);
		m_sytemGUIScreenFadeIn->setTargetWindow(m_menuWindow->getChild("PlanetWindow"));

	}

	void CSystemState::setupEvents()
	{
		m_menuWindow->getChild("PlanetWindow/PlanetButton")->subscribeEvent( CEGUI::Window::EventMouseClick,
			CEGUI::SubscriberSlot(&CSystemState::onAccessClicked, this));

		m_menuWindow->getChild("PlanetWindow/BackButton")->subscribeEvent( CEGUI::Window::EventMouseClick,
			CEGUI::SubscriberSlot(&CSystemState::onBackClicked, this));
	}

	bool CSystemState::onAccessClicked(const CEGUI::EventArgs& e)
	{
		m_menuWindow->getChild("BlackScreen")->subscribeEvent(CEGUI::AnimationInstance::EventAnimationEnded,
			CEGUI::SubscriberSlot(&CSystemState::accessPlanet,this));

		m_blackScreenFadeIn->start();
		m_sytemGUIScreenFadeOut->start();
		m_planetImageScreenFadeOut->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("systemHUDButton");
		CGameManager::getInstance()->fadeOutHUD();

		return true;
	}

	bool CSystemState::onBackClicked(const CEGUI::EventArgs& e)
	{
		m_menuWindow->getChild("BlackScreen")->subscribeEvent(CEGUI::AnimationInstance::EventAnimationEnded,
			CEGUI::SubscriberSlot(&CSystemState::goBack,this));

		m_blackScreenFadeIn->start();
		m_sytemGUIScreenFadeOut->start();
		m_planetImageScreenFadeOut->start();
		Common::Sound::CSound::getSingletonPtr()->playSound("systemHUDButton");
		CGameManager::getInstance()->fadeOutHUD();

		return true;
	}

	bool CSystemState::accessPlanet(const CEGUI::EventArgs& e)
	{
		m_app->setState("planet");

		return true;
	}
	
	bool CSystemState::goBack(const CEGUI::EventArgs& e)
	{
		m_app->popState();

		return true;
	}

}