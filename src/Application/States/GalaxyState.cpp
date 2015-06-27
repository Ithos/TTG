#include "GalaxyState.h"

#include <log.h>

#include "../../Logic/Logic.h"
#include "../../Logic/EntityFactory.h"
#include "../../Logic/Scene/Scene.h"

#include "../3DApplication.h"
#include "../Manager/GameManager.h"

#include "../../GUI/GUI.h"

#include "../../Common/Sound/Sound.h"
#include <Common/Configure/Configure.h>
#include <Common/Data/Spawn_Constants.h>
#include <Common/Data/Game_Constants.h>

#include <OGRE\OgreSceneNode.h>
#include <OGRE\OgreRay.h>
#include <OGRE\OgreSceneQuery.h>
#include <OGRE\OgreCamera.h>
#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreEntity.h>
#include <Ogre\OgreVector3.h>
#include <Ogre\OgreVector2.h>
#include <OgreManualObject.h>

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/AnimationManager.h>
#include <CEGUI/Animation.h>
#include <CEGUI/AnimationInstance.h>

#include <Common/Language/Tags.h>

namespace Application
{
	const char* const LOG_CGALAXYSTATE = "Application::CGalaxyState";

	bool CGalaxyState::init() 
	{
		log_trace(LOG_CGALAXYSTATE,"Init\n");

		CApplicationState::init();

		//Logic 
		if (!Logic::CEntityFactory::getInstance()->loadBluePrints("galaxyBlueprints.xml")) {
            log_error(LOG_CGALAXYSTATE, "Error loading blueprints");
			return false;
        }

		if (!Logic::CLogic::getInstance()->loadLevel("0001",m_root,m_renderWindow,Common::Data::GALAXY)) {
            log_error(LOG_CGALAXYSTATE, "Error loading level");
			return false;
        }

		//Game Manager
		if(CGameManager::getInstance()->getSystem() == "")
		{
			CGameManager::getInstance()->setSystem("1");
		}

		//CEGUI
		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Galaxy.layout");

		setupWindows();
		setupAnimations();

		m_menuWindow->getChild("ButtonContainer")->setVisible(false);

		//Sounds & music
		Common::Sound::CSound::getSingletonPtr()->addSound("Button3.wav","systemButton");
		Common::Sound::CSound::getSingletonPtr()->addSound("ButtonHover1.wav","labelButtonHover");
		Common::Sound::CSound::getSingletonPtr()->addSound("ButtonSound1.wav","systemButtonHover");
		Common::Sound::CSound::getSingletonPtr()->addSound("Failed2.wav","fail");

		return true;
	}

	void CGalaxyState::release() 
	{
		log_trace(LOG_CGALAXYSTATE,"Release\n");

		Common::Sound::CSound::getSingletonPtr()->releaseSound("systemButton");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("labelButtonHover");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("systemButtonHover");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("fail");

		Logic::CLogic::getInstance()->unloadLevel();

		Logic::CEntityFactory::getInstance()->unloadBluePrints();

		CApplicationState::release();
	}

	void CGalaxyState::activate() 
	{
		log_trace(LOG_CGALAXYSTATE,"Activate\n");

		CApplicationState::activate();

		Logic::CLogic::getInstance()->activateScene();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();

		CGameManager::getInstance()->activateHUD(m_menuWindow);

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

		m_RayScnQuery = Logic::CLogic::getInstance()->getScene()->getSceneManager()->createRayQuery(Ogre::Ray());

		m_playerSystem = Logic::CLogic::getInstance()->getScene()->getConstSceneManager()->
			getSceneNode("System" + CGameManager::getInstance()->getSystem() + "_node");

		static_cast<Ogre::SceneNode*>(m_playerSystem->getChild(m_playerSystem->getName()+"OutlineGreenGlow"))->setVisible(true);

		drawCircle(Logic::CLogic::getInstance()->getScene(),m_playerSystem, static_cast<float>(CGameManager::getInstance()->getEqEngineDist()), "engineDist");

		m_blackScreenFadeOut->start();

	}

	void CGalaxyState::deactivate() 
	{

		log_trace(LOG_CGALAXYSTATE,"Deactivate\n");

		Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(m_playerSystem->getChild("engineDist_subNode"));

		Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

       while ( itObject.hasMoreElements() )
       {
          Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
          node->getCreator()->destroyMovableObject( pObject );
       }
		node->getCreator()->destroySceneNode(node);

		if(m_playerSystem){
			static_cast<Ogre::SceneNode*>(m_playerSystem->getChild(m_playerSystem->getName()+"OutlineGreenGlow"))->setVisible(false);
			m_playerSystem = nullptr;
		}

		if(m_PointedSystem){
			static_cast<Ogre::SceneNode*>(m_PointedSystem->getChild(m_PointedSystem->getName()+"OutlineGlow"))->setVisible(false);
			m_PointedSystem = nullptr;
		}

		if(m_CurrentSystem){
			static_cast<Ogre::SceneNode*>(m_CurrentSystem->getChild(m_CurrentSystem->getName()+"OutlineGlow"))->setVisible(false);
			m_CurrentSystem = nullptr;
		}

		Logic::CLogic::getInstance()->getScene()->getSceneManager()->destroyQuery(m_RayScnQuery);

		m_RayScnQuery = nullptr;

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();

		CGameManager::getInstance()->deactivateHUD();

		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);

		Logic::CLogic::getInstance()->deactivateScene();

		CApplicationState::deactivate();
	}

	void CGalaxyState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		Logic::CLogic::getInstance()->tick(msecs);

		if(m_rad != CGameManager::getInstance()->getEqEngineDist()){
			Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(m_playerSystem->getChild("engineDist_subNode"));

			Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

		   while ( itObject.hasMoreElements() )
		   {
			  Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
			  node->getCreator()->destroyMovableObject( pObject );
		   }
           
           node->getCreator()->destroySceneNode(node);
		   drawCircle(Logic::CLogic::getInstance()->getScene(), m_playerSystem, static_cast<float>(CGameManager::getInstance()->getEqEngineDist()), "engineDist");
		}

	}

	bool CGalaxyState::keyPressed(Common::Input::TKey key)
	{
		return false;

	}

	bool CGalaxyState::keyReleased(Common::Input::TKey key)
	{
		switch(key.id)
		{
		case Common::Input::Key::ESCAPE:
			m_app->setState("pause");
			break;
		case Common::Input::Key::G:
			/*m_app->setState("system");*/
			//This indicates that the planet that the player is in has to be initialized in the systemState
			/*CGameManager::getInstance()->setPlanet("");*/
			m_app->setState("game_finished");
			break;
		case Common::Input::Key::E:
			/*m_app->setState("event");*/
			m_app->setState("game_over");
			break;
		default:
			return false;
		}
		return true;

	}

	bool CGalaxyState::mouseMoved(const GUI::InputListener::CMouseState &mouseState)
	{
		bool playSound(m_PointedSystem ?false:true);
		if(m_PointedSystem && m_PointedSystem!=m_CurrentSystem){
			static_cast<Ogre::SceneNode*>(m_PointedSystem->getChild(m_PointedSystem->getName()+"OutlineGlow"))->setVisible(false);
			m_PointedSystem = nullptr;
		}else{
			m_PointedSystem = nullptr;
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
				m_PointedSystem = it->movable->getParentSceneNode();
				static_cast<Ogre::SceneNode*>(m_PointedSystem->getChild(m_PointedSystem->getName()+"OutlineGlow"))->setVisible(true);
				if(playSound)
					Common::Sound::CSound::getSingletonPtr()->playSound("systemButtonHover");
				break;
			}
		}

		return false;

	}
		
	bool CGalaxyState::mousePressed(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool CGalaxyState::mouseReleased(const GUI::InputListener::CMouseState &mouseState)
	{
		if(m_CurrentSystem){
			static_cast<Ogre::SceneNode*>(m_CurrentSystem->getChild(m_CurrentSystem->getName()+"OutlineGlow"))->setVisible(false);
			m_CurrentSystem = nullptr;
		}

		m_menuWindow->getChild("ButtonContainer")->setVisible(false);

		if(mouseState.button == Common::Input::Button::LEFT){
			
			CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
			Ogre::Ray mouseRay = Logic::CLogic::getInstance()->getScene()->getSceneCamera()->getCameraToViewportRay(mousePos.d_x/float(mouseState.width),mousePos.d_y/float(mouseState.height));
			m_RayScnQuery->setRay(mouseRay);
			m_RayScnQuery->setSortByDistance(true);

			Ogre::RaySceneQueryResult &queryResult = m_RayScnQuery->execute();

			for(auto it = queryResult.begin(); it != queryResult.end(); ++it){
				if(!m_overButton && it->movable && it->movable->getName() != "" 
					&& it->movable->getName() != Logic::CLogic::getInstance()->getScene()->getSceneCamera()->getName())
				{
					{
						using namespace Common::Configuration;
						setDefaultFile(CONFIGURE_FILE);
						setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());
					}
					m_CurrentSystem = it->movable->getParentSceneNode();
					if(m_CurrentSystem->getPosition().distance(m_playerSystem->getPosition()) <= CGameManager::getInstance()->getEqEngineDist())
					{
						std::string str = m_CurrentSystem->getName();
						str = str.substr(6);
						int pos(str.find("_"));
						str.erase(pos,str.length());
						CGameManager::getInstance()->setSystem(str);
						m_menuWindow->getChild("ButtonContainer/LabelSysName")->
							setText(std::string(Common::Data::Game::GAME_GALAXY_SYSTEM_NAME[std::atoi(str.c_str())]));
						Ogre::Vector2 vec(GetScreenspaceCoords(m_CurrentSystem->getPosition(), *Logic::CLogic::getInstance()->getScene()->getSceneCamera()));
						CEGUI::UDim height(m_menuWindow->getChild("ButtonContainer")->getHeight()),width(m_menuWindow->getChild("ButtonContainer")->getWidth());
						CEGUI::UDim dim1(vec.x,0),dim2(vec.y,0);
						m_menuWindow->getChild("ButtonContainer")->setPosition(CEGUI::UVector2(dim1 - width*0.9f, dim2 - height*0.6f));

						str = Ogre::any_cast<Ogre::String>(m_CurrentSystem->getUserAny());

						if(str == Common::Data::Spawn::COMMON_TARGET)
							CGameManager::getInstance()->setIsTargetSystem(true);
						else
							CGameManager::getInstance()->setIsTargetSystem(false);

						m_systemIconAnimation->start();

						m_tagPopupLines->setPosition(m_tagPopupLines->getDefinition()->getDuration());
						m_tagPopupLines->apply();
						m_tagPopupLines->pause();
						m_tagPopupLines->start();

						m_buttonPopupLines->setPosition(m_buttonPopupLines->getDefinition()->getDuration());
						m_buttonPopupLines->apply();
						m_buttonPopupLines->pause();
						m_buttonPopupLines->start();

						m_menuWindow->getChild("ButtonContainer")->setVisible(true);
						Common::Sound::CSound::getSingletonPtr()->playSound("systemButton");
					}else{
						m_CurrentSystem = nullptr;
						Common::Sound::CSound::getSingletonPtr()->playSound("fail");
					}
					break;
				}
			}
		}

		if(m_CurrentSystem){
			static_cast<Ogre::SceneNode*>(m_CurrentSystem->getChild(m_CurrentSystem->getName()+"OutlineGlow"))->setVisible(true);
		}

		return false;

	}

	void CGalaxyState::setupWindows()
	{
		m_menuWindow->getChild("ButtonContainer/PopupLines/LabelProceed")->
			subscribeEvent(CEGUI::Window::EventMouseEntersArea, 
			CEGUI::SubscriberSlot(&CGalaxyState::onLabelEntered, this));

		m_menuWindow->getChild("ButtonContainer/PopupLines/LabelProceed")->
			subscribeEvent(CEGUI::Window::EventMouseLeavesArea, 
			CEGUI::SubscriberSlot(&CGalaxyState::onLabelExited, this));
		m_menuWindow->getChild("ButtonContainer/PopupLines/LabelProceed")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
			CEGUI::SubscriberSlot(&CGalaxyState::onLabelClicked, this));

	}

	void CGalaxyState::setupAnimations()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* sizeGrowth = animMgr.getAnimation("SizeGrowth");
		m_buttonPopupLines = animMgr.instantiateAnimation(sizeGrowth);
		m_buttonPopupLines->setTargetWindow(m_menuWindow->getChild("ButtonContainer/PopupLines"));

		m_tagPopupLines = animMgr.instantiateAnimation(sizeGrowth);
		m_tagPopupLines->setTargetWindow(m_menuWindow->getChild("ButtonContainer/LabelSysName"));

		CEGUI::Animation* iconAnimationLoop = animMgr.getAnimation("LoopRotateRight");
		m_systemIconAnimation = animMgr.instantiateAnimation(iconAnimationLoop);
		m_systemIconAnimation->setTargetWindow(m_menuWindow->getChild("ButtonContainer/PlanetButton"));

		CEGUI::Animation* blackScreenFadeOut = animMgr.getAnimation("BlackWindowFadeOut");
		m_blackScreenFadeOut = animMgr.instantiateAnimation(blackScreenFadeOut);
		m_blackScreenFadeOut->setTargetWindow(m_menuWindow->getChild("BlackScreen"));

		CEGUI::Animation* blackScreenFadeIn = animMgr.getAnimation("InsideBlendInQuick");
		m_blackScreenFadeIn = animMgr.instantiateAnimation(blackScreenFadeIn);
		m_blackScreenFadeIn->setTargetWindow(m_menuWindow->getChild("BlackScreen"));

	}

	bool CGalaxyState::onLabelEntered(const CEGUI::EventArgs& e)
	{
		m_menuWindow->getChildElement("ButtonContainer/PopupLines/LabelProceed")->
			setProperty("NormalTextColour","FFFFFFFF");
		m_overButton = true;
		Common::Sound::CSound::getSingletonPtr()->playSound("labelButtonHover");
		return true;
	}

	bool CGalaxyState::onLabelExited(const CEGUI::EventArgs& e)
	{
		m_menuWindow->getChildElement("ButtonContainer/PopupLines/LabelProceed")->
			setProperty("NormalTextColour","FF00A9FF");
		m_overButton = false;
		return true;
	}

	bool CGalaxyState::onLabelClicked(const CEGUI::EventArgs& e)
	{
		if(CGameManager::getInstance()->getPlayerResourceByName(Common::Language::getString(GAME_FUEL)) >= CGameManager::getInstance()->getEqEngineCons() ||
			m_CurrentSystem == m_playerSystem){
			if(m_CurrentSystem != m_playerSystem)
				CGameManager::getInstance()->decreaseResourceByName(Common::Language::getString(GAME_FUEL),CGameManager::getInstance()->getEqEngineCons());

			//Here we define a sound that is only gonna be used one time
			Common::Sound::CSound::getSingletonPtr()->addSound("Button2.wav","labelButton");
			Common::Sound::CSound::getSingletonPtr()->playSound("labelButton", 0.7f, true);

			m_menuWindow->getChild("BlackScreen")->subscribeEvent(CEGUI::AnimationInstance::EventAnimationEnded,
				CEGUI::SubscriberSlot(&CGalaxyState::accessSystem,this));

			m_blackScreenFadeIn->start();
			CGameManager::getInstance()->fadeOutHUD();
		}else{
			Common::Sound::CSound::getSingletonPtr()->playSound("fail");
		}

		return true;
	}

	bool CGalaxyState::accessSystem(const CEGUI::EventArgs& e)
	{
		m_app->setState("system");
		CGameManager::getInstance()->setPlanet("");
		return true;
	}

	Ogre::Vector2 CGalaxyState::GetScreenspaceCoords(const Ogre::Vector3& iPoint, const Ogre::Camera& iCamera)
	{
		Ogre::Vector3 point = iCamera.getProjectionMatrix() * (iCamera.getViewMatrix() * iPoint);

		Ogre::Vector2 screenSpacePoint = Ogre::Vector2::ZERO;
		screenSpacePoint.x = (point.x / 2.f) + 0.5f;
		screenSpacePoint.y = 0.5f - (point.y / 2.f);

		return screenSpacePoint;

	}

	void CGalaxyState::drawCircle(Logic::CScene* scene,Ogre::SceneNode *centerNode, float radius, const char* str)
	{
		Ogre::ManualObject* circle = scene->getSceneManager()->createManualObject(str);  
		circle->begin("GreyLine",Ogre::RenderOperation::OT_LINE_LIST);
		circle->setCastShadows(false);
		unsigned int accuracy = 35;
		unsigned int index = 0;
		for(float theta = 0; theta <= 2 * Ogre::Math::PI; theta += Ogre::Math::PI / accuracy) {
			circle->position(radius * cos(theta), radius * sin(theta), 0.0);
			circle->index(index++);
		}

		circle->index(0);
		circle->end();

		circle->setQueryFlags(Common::Data::NULL_MASK);

		Ogre::SceneNode* subNode = centerNode->createChildSceneNode(std::string(str)+"_subNode");
		subNode->setInheritScale(false);
		subNode->attachObject(circle);
		m_rad = radius;
	}
}