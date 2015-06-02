#include "MenuState.h"

#include "../../Common/Sound/Sound.h"

#include <log.h>

#include <CEGUI/CEGUI.h>

namespace Application
{
	const char* const LOG_CMENUSTATE = "Application::CMenuState";

	CMenuState::~CMenuState() 
		{
		} 

	bool CMenuState::init() 
	{
		log_trace(LOG_CMENUSTATE,"Init\n");

		CApplicationState::init();

		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("NewMenu.layout");

		setupWindows();
		setupAnimatons();

		//Sounds & music
		Common::Sound::CSound::getSingletonPtr()->addSound("Button1.wav","startButtonSound");
		Common::Sound::CSound::getSingletonPtr()->addSound("Button3.wav","innerButtonSound");
		Common::Sound::CSound::getSingletonPtr()->addSound("ButtonHover2.wav","rotateButtonHover");
		Common::Sound::CSound::getSingletonPtr()->addSound("Intro1.wav","introSound");
		Common::Sound::CSound::getSingletonPtr()->addSound("IntroMenu.wav","introMenu");

		Common::Sound::CSound::getSingletonPtr()->addMusic("TTG_Planetas.wav","MenuMusic",true);

		return true;

	}

	void CMenuState::release() 
	{
		log_trace(LOG_CMENUSTATE,"Release\n");

		Common::Sound::CSound::getSingletonPtr()->releaseSound("startButtonSound");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("innerButtonSound");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("rotateButtonHover");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("introSound");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("introMenu");

		Common::Sound::CSound::getSingletonPtr()->releaseMusic("MenuMusic");

		CApplicationState::release();

	}

	void CMenuState::activate() 
	{
		log_trace(LOG_CMENUSTATE,"Activate\n");

		CApplicationState::activate();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		onActivateMenu();
		Common::Sound::CSound::getSingletonPtr()->playDelayedSound("introSound",m_delay1);
		Common::Sound::CSound::getSingletonPtr()->playDelayedSound("introMenu",m_delay2);
		if(m_delay1!=1200)m_delay1=1200;
		if(m_delay2!=2300)m_delay2=2300;
		if(Common::Sound::CSound::getSingletonPtr()->isPlaying("MenuMusic"))
			Common::Sound::CSound::getSingletonPtr()->stopMusic("MenuMusic");
	}

	void CMenuState::deactivate() 
	{		
		log_trace(LOG_CMENUSTATE,"Deactivate\n");

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	}

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	}

	bool CMenuState::keyPressed(Common::Input::TKey key)
	{
		return false;

	}

	bool CMenuState::keyReleased(Common::Input::TKey key)
	{
		switch(key.id)
		{
		case Common::Input::Key::ESCAPE:
			m_app->exitRequest();
			break;
		case Common::Input::Key::RETURN:
			m_app->setState("galaxy");
			break;
		default:
			return false;
		}
		return true;

	}

	bool CMenuState::mouseMoved(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool CMenuState::mousePressed(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}


	bool CMenuState::mouseReleased(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool CMenuState::startReleased(const CEGUI::EventArgs& e)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("startButtonSound");

		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){
			Common::Sound::CSound::getSingletonPtr()->playMusic("MenuMusic");
			m_app->setState("galaxy");
		}
		return true;

	}

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){
			m_app->exitRequest();
		}
		return true;

	}

	bool CMenuState::handleStartPopupLinesShipsDisplay(const CEGUI::EventArgs& args)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("innerButtonSound");
		makeAllSelectionIconsInvisible();
		stopStartPopupLinesAnimations();

		m_popupLinesShipsAnimInst->start();

		m_menuWindow->getChild("InnerButtonsContainer/Name1SelectionIcon")->setVisible(true);
		m_menuWindow->getChild("InnerButtonsContainer/Name2SelectionIcon")->setVisible(true);
		m_menuWindow->getChild("InnerButtonsContainer/Name3SelectionIcon")->setVisible(true);

		return false;
	}

	bool CMenuState::handleStartPopupLinesLoadDisplay(const CEGUI::EventArgs& args)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("innerButtonSound");
		makeAllSelectionIconsInvisible();
		stopStartPopupLinesAnimations();

		m_popupLinesLoadAnimInst->start();

		m_menuWindow->getChild("InnerButtonsContainer/LoadSelectionIcon")->setVisible(true);

		return false;
	}

	bool CMenuState::handleStartPopupLinesQuitDisplay(const CEGUI::EventArgs& args)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("innerButtonSound");

		makeAllSelectionIconsInvisible();
		stopStartPopupLinesAnimations();

		m_popupLinesQuitAnimInst->start();

		m_menuWindow->getChild("InnerButtonsContainer/QuitSelectionIcon")->setVisible(true);

		return false;
	}

	bool CMenuState::handleStartPopupLinesOptionsDisplay(const CEGUI::EventArgs& args)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("innerButtonSound");

		makeAllSelectionIconsInvisible();
		stopStartPopupLinesAnimations();

		m_popupLinesOptionsAnimInst->start();

		m_menuWindow->getChild("InnerButtonsContainer/OptionsSelectionIcon")->setVisible(true);

		return false;
	}

	bool CMenuState::handleInnerButtonsLabelEntered(const CEGUI::EventArgs& args)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("rotateButtonHover");

		const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);

		CEGUI::Window* iconWindow = getIconWindowFromLabel(mouseArgs.window);

		CEGUI::EventArgs fireArgs;
		iconWindow->fireEvent("StartRotate", fireArgs);

		return false;
	}

	bool CMenuState::handleInnerButtonsLabelLeft(const CEGUI::EventArgs& args)
	{
		const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);

		CEGUI::Window* iconWindow = getIconWindowFromLabel(mouseArgs.window);
 
		CEGUI::EventArgs fireArgs;
		iconWindow->fireEvent("StopRotate", fireArgs);

		return false;
	}

	bool CMenuState::handleInnerButtonEntered(const CEGUI::EventArgs& args)
	{
		Common::Sound::CSound::getSingletonPtr()->playSound("rotateButtonHover");

		const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);

		CEGUI::Window* iconWindow =mouseArgs.window;

		CEGUI::EventArgs fireArgs;
		iconWindow->fireEvent("StartRotate", fireArgs);

		return false;
	}

	bool CMenuState::handleInnerButtonLeft(const CEGUI::EventArgs& args)
	{
		const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);

		CEGUI::Window* iconWindow =mouseArgs.window;

		CEGUI::EventArgs fireArgs;
		iconWindow->fireEvent("StopRotate", fireArgs);

		return false;
	}

	CEGUI::Window* CMenuState::getIconWindowFromLabel(CEGUI::Window* window)
	{
		const CEGUI::String windowPrefix = "Label";
		const CEGUI::String iconPostfix = "SelectionIcon";

		CEGUI::String windowName = window->getName();

		windowName = windowName.substr(windowPrefix.length());
		return window->getParent()->getParent()->getChild(windowName + iconPostfix);
	}

	void CMenuState::stopStartPopupLinesAnimations()
	{
		m_popupLinesShipsAnimInst->setPosition(m_popupLinesShipsAnimInst->getDefinition()->getDuration());
		m_popupLinesShipsAnimInst->apply();
		m_popupLinesShipsAnimInst->pause();
		m_popupLinesLoadAnimInst->setPosition(m_popupLinesLoadAnimInst->getDefinition()->getDuration());
		m_popupLinesLoadAnimInst->apply();
		m_popupLinesLoadAnimInst->pause();
		m_popupLinesQuitAnimInst->setPosition(m_popupLinesQuitAnimInst->getDefinition()->getDuration());
		m_popupLinesQuitAnimInst->apply();
		m_popupLinesQuitAnimInst->pause();
		m_popupLinesOptionsAnimInst->setPosition(m_popupLinesOptionsAnimInst->getDefinition()->getDuration());
		m_popupLinesOptionsAnimInst->apply();
		m_popupLinesOptionsAnimInst->pause();

		m_menuWindow->getChild("InnerButtonsContainer/PopupLinesCharacters")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/PopupLinesLoad")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/PopupLinesQuit")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/PopupLinesOptions")->setVisible(false);
	}

	void CMenuState::makeAllSelectionIconsInvisible()
	{
		m_menuWindow->getChild("InnerButtonsContainer/Name1SelectionIcon")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/Name2SelectionIcon")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/Name3SelectionIcon")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/LoadSelectionIcon")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/OptionsSelectionIcon")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/QuitSelectionIcon")->setVisible(false);
	}

	void CMenuState::setupWindows()
	{
		CEGUI::Window* buttonCharacters = m_menuWindow->getChild("InnerButtonsContainer/ButtonShips");
		buttonCharacters->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMenuState::handleStartPopupLinesShipsDisplay, this));
		CEGUI::Window* buttonLoad = m_menuWindow->getChild("InnerButtonsContainer/ButtonLoad");
		buttonLoad->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMenuState::handleStartPopupLinesLoadDisplay, this));
		CEGUI::Window* buttonQuit = m_menuWindow->getChild("InnerButtonsContainer/ButtonQuit");
		buttonQuit->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMenuState::handleStartPopupLinesQuitDisplay, this));
		CEGUI::Window* buttonOptions = m_menuWindow->getChild("InnerButtonsContainer/ButtonOptions");
		buttonOptions->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&CMenuState::handleStartPopupLinesOptionsDisplay, this));

		m_menuWindow->getChildElement("InnerButtonsContainer/QuitSelectionIcon")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&CMenuState::exitReleased, this));

		m_menuWindow->getChildElement("InnerPartContainer/InsideStartClickArea")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&CMenuState::startReleased, this));

		setupInnerButtonsSubOptionsLabels();
	}

	void CMenuState::setupInnerButtonsSubOptionsLabels()
	{
		CEGUI::Window* label;
		label = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesCharacters/LabelName1");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesCharacters/LabelName2");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesCharacters/LabelName3");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesLoad/LabelLoad");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesQuit/LabelQuit");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesOptions/LabelOptions");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonsLabelLeft, this));

		label = m_menuWindow->getChild("InnerButtonsContainer/LoadSelectionIcon");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/Name1SelectionIcon");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/Name2SelectionIcon");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/Name3SelectionIcon");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/QuitSelectionIcon");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonLeft, this));
		label = m_menuWindow->getChild("InnerButtonsContainer/OptionsSelectionIcon");
		label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonEntered, this));
		label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&CMenuState::handleInnerButtonLeft, this));
	}

	void CMenuState::setupAnimatons()
	{
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* startButtAnim = animMgr.getAnimation("StartButtonPulsating");
		startButtAnimInstance = animMgr.instantiateAnimation(startButtAnim);
		CEGUI::Window* startButtWindow = m_menuWindow->getChild("InnerPartContainer/InsideStartClickArea/StartButtonImage");
		startButtAnimInstance->setTargetWindow(startButtWindow);
		startButtAnimInstance->start();

		CEGUI::Animation* insideImg1Anim = animMgr.getAnimation("InsideImage1Pulsating");
		insideImg1AnimInst = animMgr.instantiateAnimation(insideImg1Anim);
		CEGUI::Window* insideImg1 = m_menuWindow->getChild("InnerPartContainer/InsideImage1");
		insideImg1AnimInst->setTargetWindow(insideImg1);
		insideImg1AnimInst->start();

		CEGUI::Animation* topBarAnim = animMgr.getAnimation("TopBarMoveInAnimation");
		m_topBarAnimInst = animMgr.instantiateAnimation(topBarAnim);
		CEGUI::Window* topBarWindow = m_menuWindow->getChild("TopBar");
		m_topBarAnimInst->setTargetWindow(topBarWindow);

		CEGUI::Animation* botBarAnim = animMgr.getAnimation("BotBarMoveInAnimation");
		m_botBarAnimInst = animMgr.instantiateAnimation(botBarAnim);
		CEGUI::Window* botBarWindow = m_menuWindow->getChild("BotBar");
		m_botBarAnimInst->setTargetWindow(botBarWindow);

		CEGUI::Animation* insideBlendInAnim = animMgr.getAnimation("InsideBlendIn");
		m_insideBlendInAnimInst = animMgr.instantiateAnimation(insideBlendInAnim);
		CEGUI::Window* innerPartContainer = m_menuWindow->getChild("InnerPartContainer");
		m_insideBlendInAnimInst->setTargetWindow(innerPartContainer);

		CEGUI::Animation* insideImage3RotateIn = animMgr.getAnimation("InsideImage3RotateIn");
		m_insideImage3RotateInInst = animMgr.instantiateAnimation(insideImage3RotateIn);
		CEGUI::Window* insideImage3 = m_menuWindow->getChild("InnerPartContainer/OuterRingsContainer/InsideImage3");
		m_insideImage3RotateInInst->setTargetWindow(insideImage3);

		CEGUI::Animation* insideImage4RotateIn = animMgr.getAnimation("InsideImage4RotateIn");
		m_insideImage4RotateInInst = animMgr.instantiateAnimation(insideImage4RotateIn);
		CEGUI::Window* insideImage4 = m_menuWindow->getChild("InnerPartContainer/OuterRingsContainer/InsideImage4");
		m_insideImage4RotateInInst->setTargetWindow(insideImage4);

		CEGUI::Animation* insideImageRingsContainerSizeIn = animMgr.getAnimation("RingsContainerSizeIn");
		m_insideImageRingsContainerSizeInInst = animMgr.instantiateAnimation(insideImageRingsContainerSizeIn);
		CEGUI::Window* insideImageContainer = m_menuWindow->getChild("InnerPartContainer/OuterRingsContainer");
		m_insideImageRingsContainerSizeInInst->setTargetWindow(insideImageContainer);

		CEGUI::Animation* buttonFadeInAnim = animMgr.getAnimation("ButtonFadeIn");
		m_buttonFadeInAnimInst1 = animMgr.instantiateAnimation(buttonFadeInAnim);
		CEGUI::Window* window = m_menuWindow->getChild("InnerButtonsContainer/ButtonOptions");
		m_buttonFadeInAnimInst1->setTargetWindow(window);
		m_buttonFadeInAnimInst2 = animMgr.instantiateAnimation(buttonFadeInAnim);
		window = m_menuWindow->getChild("InnerButtonsContainer/ButtonLoad");
		m_buttonFadeInAnimInst2->setTargetWindow(window);
		m_buttonFadeInAnimInst3 = animMgr.instantiateAnimation(buttonFadeInAnim);
		window = m_menuWindow->getChild("InnerButtonsContainer/ButtonShips");
		m_buttonFadeInAnimInst3->setTargetWindow(window);
		m_buttonFadeInAnimInst4 = animMgr.instantiateAnimation(buttonFadeInAnim);
		window = m_menuWindow->getChild("InnerButtonsContainer/ButtonQuit");
		m_buttonFadeInAnimInst4->setTargetWindow(window);

		CEGUI::Animation* blendInAnim = animMgr.getAnimation("BlendIn");
		m_startButtonBlendInAnimInst = animMgr.instantiateAnimation(blendInAnim);
		m_startButtonBlendInAnimInst->setTargetWindow(m_menuWindow->getChild("InnerPartContainer")->getChild("InsideStartClickArea"));


		CEGUI::Animation* partialBlendOutAnim = animMgr.getAnimation("PartialBlendOut");

		window = m_menuWindow->getChild("InnerButtonsContainer");
		m_centerButtonsPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
		m_centerButtonsPartialBlendOutInst->setTargetWindow(window);
		m_centerButtonsBlendInInst = animMgr.instantiateAnimation(blendInAnim);
		m_centerButtonsBlendInInst->setTargetWindow(window);

		//Maybe later
		/*CEGUI::Animation* blendOutAnim = animMgr.getAnimation("FullBlendOut");
		m_botBarLabelBlendOutInst = animMgr.instantiateAnimation(blendOutAnim);
		m_botBarLabelBlendOutInst->setTargetWindow(d_botBarLabel);*/

		setupPopupLinesAnimations();

		setupSelectionIconAnimations();

	}

	void CMenuState::setupPopupLinesAnimations()
	{
		CEGUI::Window* window;
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* sizeGrowth = animMgr.getAnimation("SizeGrowth");
		window = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesLoad");
		m_popupLinesLoadAnimInst = animMgr.instantiateAnimation(sizeGrowth);
		m_popupLinesLoadAnimInst->setTarget(window);
		window = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesCharacters");
		m_popupLinesShipsAnimInst = animMgr.instantiateAnimation(sizeGrowth);
		m_popupLinesShipsAnimInst->setTarget(window);
		window = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesQuit");
		m_popupLinesQuitAnimInst = animMgr.instantiateAnimation(sizeGrowth);
		m_popupLinesQuitAnimInst->setTarget(window);
		window = m_menuWindow->getChild("InnerButtonsContainer/PopupLinesOptions");
		m_popupLinesOptionsAnimInst = animMgr.instantiateAnimation(sizeGrowth);
		m_popupLinesOptionsAnimInst->setTarget(window);
	}

	void CMenuState::setupSelectionIconAnimations()
	{
		CEGUI::Window* window;
		CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

		CEGUI::Animation* iconAnimationLoop = animMgr.getAnimation("LoopRotateRight");
		CEGUI::Animation* iconAnimationStop = animMgr.getAnimation("StopRotate");
		CEGUI::AnimationInstance* iconAnimInst;

		window = m_menuWindow->getChild("InnerButtonsContainer/LoadSelectionIcon");
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationLoop);
		iconAnimInst->setTargetWindow(window);
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationStop);
		iconAnimInst->setTargetWindow(window);

		window = m_menuWindow->getChild("InnerButtonsContainer/Name1SelectionIcon");
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationLoop);
		iconAnimInst->setTargetWindow(window);
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationStop);
		iconAnimInst->setTargetWindow(window);
		window = m_menuWindow->getChild("InnerButtonsContainer/Name2SelectionIcon");
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationLoop);
		iconAnimInst->setTargetWindow(window);
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationStop);
		iconAnimInst->setTargetWindow(window);
		window = m_menuWindow->getChild("InnerButtonsContainer/Name3SelectionIcon");
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationLoop);
		iconAnimInst->setTargetWindow(window);
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationStop);
		iconAnimInst->setTargetWindow(window);
		window = m_menuWindow->getChild("InnerButtonsContainer/QuitSelectionIcon");
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationLoop);
		iconAnimInst->setTargetWindow(window);
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationStop);
		iconAnimInst->setTargetWindow(window);
		window = m_menuWindow->getChild("InnerButtonsContainer/OptionsSelectionIcon");
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationLoop);
		iconAnimInst->setTargetWindow(window);
		iconAnimInst = animMgr.instantiateAnimation(iconAnimationStop);
		iconAnimInst->setTargetWindow(window);
	}

	void CMenuState::onActivateMenu()
	{
		m_centerButtonsBlendInInst->setPosition(m_centerButtonsBlendInInst->getDefinition()->getDuration());
		m_centerButtonsBlendInInst->apply();

		m_menuWindow->getChild("InnerButtonsContainer/PopupLinesLoad")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/PopupLinesCharacters")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/PopupLinesQuit")->setVisible(false);
		m_menuWindow->getChild("InnerButtonsContainer/PopupLinesOptions")->setVisible(false);

		resetAnimations();

		m_menuWindow->getChild("InnerPartContainer")->getChild("InsideStartClickArea")->setVisible(false);

		startEntranceAnimations();

		makeAllSelectionIconsInvisible();
	}

	void CMenuState::resetAnimations()
	{
		//Set animations to end and apply their changes to the windows
		//For every animation instance that uses source properties
		//so the values will be taken correctly after starting them again
		m_buttonFadeInAnimInst1->setPosition(m_buttonFadeInAnimInst1->getDefinition()->getDuration());
		m_buttonFadeInAnimInst1->apply();
		m_buttonFadeInAnimInst2->setPosition(m_buttonFadeInAnimInst2->getDefinition()->getDuration());
		m_buttonFadeInAnimInst2->apply();
		m_buttonFadeInAnimInst3->setPosition(m_buttonFadeInAnimInst3->getDefinition()->getDuration());
		m_buttonFadeInAnimInst3->apply();
		m_buttonFadeInAnimInst4->setPosition(m_buttonFadeInAnimInst4->getDefinition()->getDuration());
		m_buttonFadeInAnimInst4->apply();
	}

	void CMenuState::startEntranceAnimations()
	{
		m_topBarAnimInst->start();
		m_botBarAnimInst->start();
		m_insideBlendInAnimInst->start();
		m_insideImage3RotateInInst->start();
		m_insideImage4RotateInInst->start();
		m_insideImageRingsContainerSizeInInst->start();
		m_buttonFadeInAnimInst1->start();
		m_buttonFadeInAnimInst2->start();
		m_buttonFadeInAnimInst3->start();
		m_buttonFadeInAnimInst4->start();

		startButtonsAnims();
	}

	void CMenuState::startButtonsAnims()
	{
		m_menuWindow->getChild("InnerPartContainer")->getChild("InsideStartClickArea")->setAlpha(0.0f);
		m_startButtonBlendInAnimInst->start();
		m_centerButtonsPartialBlendOutInst->pause();
		m_centerButtonsBlendInInst->start();
	}
}