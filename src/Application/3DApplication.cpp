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

#include "3DApplication.h"
#include "Clock.h" // contiene include OgreRoot

//a#include <boost\log\trivial.hpp>
#include <log.h>
//#include <Common/Log/log.h>
#include <Common/Sound/Sound.h>

#include <string>

#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
#include <OgreException.h>
#include <OgreWindowEventUtilities.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include <OISInputManager.h>
#include <OISMouse.h>
#include <OISKeyboard.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/System.h>
#include <CEGUI/DefaultResourceProvider.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <Common/Physic/PhysicManager.h>
#include <Logic/Logic.h>
#include <Common/Configure/Configure.h>
#include <GUI/InputManager.h>
#include <GUI/GUI.h>
#include <AI/AI.h>


#include <Common/Language/Language.h>

#define RENDER_IN_BACKGROUND

namespace Application
{
	class CWindowEventListener : public Ogre::WindowEventListener
	{
	public:
		CWindowEventListener() { }

		bool windowClosing(Ogre::RenderWindow* renderWindow)
		{
			CBaseApplication::getInstance()->exitRequest();
			return false;
		}
	};

	const char* const LOG_C3DAPP = "Application::C3DApplication";

    //------------------------------------------------------------------------
	
	C3DApplication::C3DApplication(): CBaseApplication(),
									 m_ogreListener(nullptr),
									 m_inputSystem(nullptr),
									 m_keyboard(nullptr),
									 m_mouse(nullptr),
									 m_GUISystem(nullptr),
									 m_xmlParser(nullptr)
	{}
	
	C3DApplication::~C3DApplication(){}
	
	bool C3DApplication::init()
	{
		//First init

		if(!CBaseApplication::init()) return false;


		if(!initOgre() || !initCEGUI()) return false;

		log_trace(LOG_C3DAPP, "Creating clock %s", __FUNCTIONW__);
		m_clock = &CClock::getInstance(m_root);

		return true;
	}

	bool C3DApplication::secondInit()
	{
		if(!initOIS()||!secondInitOgre())
			return false;

		if(!GUI::CInputManager::init(m_inputSystem,m_keyboard,m_mouse)) return false;


		GUI::CInputManager* input = GUI::CInputManager::getInstance();
		input->addKeyListener(this);
		input->addMouseListener(this);

		if(!GUI::CGUI::init(m_GUISystem)) return false;

		if(!Common::Physic::CPhysicManager::Init()) return false;

		if(!Logic::CLogic::init()) return false;

		Logic::CLogic::getInstance()->loadLevel("dummy",m_root,m_renderWindow,Common::Data::DUMMY);

		if(!AI::CAI::init()) return false;

		if(!Common::Sound::CSound::init())return false;

		Common::Language::changeLanguage("en");

		return true;
	}
	
	void C3DApplication::release()
	{
		Common::Sound::CSound::release();

		Logic::CLogic::release();

		AI::CAI::release();

		Common::Physic::CPhysicManager::Release();

		GUI::CGUI::release();
		GUI::CInputManager* input = GUI::CInputManager::getInstance();
		input->removeKeyListener(this);
		input->removeMouseListener(this);
		input->release();

		releaseCEGUI();
		releaseOIS();
		releaseOgre();
		
		CBaseApplication::release();
	}
	
	void C3DApplication::tick(unsigned int msecs)
	{
		CBaseApplication::tick(msecs);

		CEGUI::System::getSingleton().injectTimePulse( msecs/1000.0f );

        CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(msecs/1000.0f);

		GUI::CInputManager::getInstance()->tick();

		Common::Sound::CSound::getSingletonPtr()->tick(msecs);

		if(m_root){
			Ogre::WindowEventUtilities::messagePump();
			m_root->renderOneFrame(msecs/1000.0f);
		}
	}
	
	bool C3DApplication::initOgre()
	{
		log_trace(LOG_C3DAPP,"Creating Ogre root...\n");
#ifdef _DEBUG
		m_root = new Ogre::Root("plugins_d.cfg");//config file
#else
		m_root = new Ogre::Root("plugins.cfg");//config file
#endif

		log_trace(LOG_C3DAPP,"Loading resources paths...\n");
		Ogre::ConfigFile cf;
		cf.load("resources.cfg");//config file

		Ogre::ConfigFile::SectionIterator itSec = cf.getSectionIterator();
		std::string sSection, sType, sArch;
		while(itSec.hasMoreElements()){
			sSection = itSec.peekNextKey();

			Ogre::ConfigFile::SettingsMultiMap *mapSettings = itSec.getNext();
			Ogre::ConfigFile::SettingsMultiMap::const_iterator itSetting = mapSettings->begin();
			while(itSetting != mapSettings->end()){
				sType = itSetting->first;
				sArch = itSetting->second;

				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(sArch,sType,sSection);
				++itSetting;
			}
		}
		log_trace(LOG_C3DAPP,"Resources paths loaded...\n");
		log_trace(LOG_C3DAPP,"Loading config...\n");
		if(!m_root->restoreConfig()){
			log_trace(LOG_C3DAPP,"Failed while restoring config...\n");
			if(!m_root->showConfigDialog()){
				log_trace(LOG_C3DAPP,"Failed to show config dialog!\n");
				return false;
			}
		}

		log_trace(LOG_C3DAPP,"Config Loaded\n");
		log_trace(LOG_C3DAPP,"Creating render window\n");
		m_renderWindow = m_root->initialise(true,"TTG");//config file
		log_trace(LOG_C3DAPP,"Render window created\n");

		return true;
	}
	
	bool C3DApplication::initOgreResources()
	{
		try{
			
			m_ogreListener = new CWindowEventListener();
			Ogre::WindowEventUtilities::addWindowEventListener(m_renderWindow,m_ogreListener);

#if defined RENDER_IN_BACKGROUND
			m_renderWindow->setDeactivateOnFocusChange(false);
#endif
			Ogre::ResourceGroupManager::getSingletonPtr()->initialiseResourceGroup("General");

		}catch(Ogre::Exception e){
			return false;
		}
		return true;
	}
	
	bool C3DApplication::initOIS()
	{
		unsigned int width, height, windowHnd;
		if(!m_renderWindow) return false;

		width = m_renderWindow->getWidth();
		height = m_renderWindow->getHeight();
		m_renderWindow->getCustomAttribute("WINDOW",&windowHnd);

		OIS::ParamList paramList;
		std::ostringstream windowHndStr;

		windowHndStr << windowHnd;
		paramList.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));

#if defined NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
		if(!m_renderWindow->isFullScreen())
		{
#if defined OIS_WIN32_PLATFORM 
			paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" ))); 
			paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE"))); 
			paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND"))); 
			paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE"))); 
#elif defined OIS_LINUX_PLATFORM 
			paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false"))); 
			paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false"))); 
			paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false"))); 
			paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true"))); 
#endif 
		}
#endif

		try {
			m_inputSystem = OIS::InputManager::createInputSystem(paramList);
		} catch (std::exception e){
			return false;
		}

		if(m_inputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0){
			m_keyboard = static_cast<OIS::Keyboard*>(m_inputSystem->createInputObject(OIS::OISKeyboard, true));
		}

		if(m_inputSystem->getNumberOfDevices(OIS::OISMouse) > 0){
			m_mouse = static_cast<OIS::Mouse*>(m_inputSystem->createInputObject(OIS::OISMouse, true));

			bool fullScreen = m_renderWindow->isFullScreen();
			m_renderWindow->setFullscreen(fullScreen,width,height);

			const OIS::MouseState &mouseStateOIS = m_mouse->getMouseState();
			mouseStateOIS.width  = width;
			mouseStateOIS.height = height;
		}

		return true;
	}

	bool C3DApplication::secondInitOgre()
	{
		log_trace(LOG_C3DAPP,"Initializing ogre resources...\n");
		if(!initOgreResources()){
			log_trace(LOG_C3DAPP,"Failed while initializing resources!\n");
			if(!m_root->showConfigDialog()){
				log_trace(LOG_C3DAPP,"Failed to show config dialog!\n");
				return false;
			}
			log_trace(LOG_C3DAPP,"Trying again to initialize ogre resources...\n");
			if(!initOgreResources()){
				log_trace(LOG_C3DAPP,"Failed while initializing resources!\n");
				return false;
			}
		}

		return true;
	}
	
	bool C3DApplication::initCEGUI()
	{
		log_trace(LOG_C3DAPP,"Creating CEGUI OgreRenderer\n");
		CEGUI::OgreRenderer& CEGUIRender = CEGUI::OgreRenderer::create(*m_renderWindow);

		CEGUI::System::create(CEGUIRender);

		m_GUISystem = CEGUI::System::getSingletonPtr();

		log_trace(LOG_C3DAPP,"Initializing CEGUI resources\n");
		CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
			(m_GUISystem->getResourceProvider());

		//configuration...
		rp->setResourceGroupDirectory("fonts", "media/gui/fonts/");
		rp->setResourceGroupDirectory("imagesets", "media/gui/imagesets/");
		rp->setResourceGroupDirectory("layouts", "media/gui/layouts/");
		rp->setResourceGroupDirectory("looknfeels", "media/gui/looknfeel/");
		rp->setResourceGroupDirectory("schemes", "media/gui/schemes/");
		rp->setResourceGroupDirectory("xml_schemas", "media/gui/xml_schemas/");
		rp->setResourceGroupDirectory("animations", "media/gui/animations/");

		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::AnimationManager::setDefaultResourceGroup("animations");

		log_trace(LOG_C3DAPP,"Creating CEGUI xml parser\n");

		m_xmlParser = m_GUISystem->getXMLParser();
		if (m_xmlParser->isPropertyPresent("SchemaDefaultResourceGroup"))
			m_xmlParser->setProperty("SchemaDefaultResourceGroup", "xml_schemas");

		log_trace(LOG_C3DAPP,"Parsing CEGUI xml resources\n");

		CEGUI::SchemeManager::getSingleton().createFromFile("GameMenu.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");

		log_trace(LOG_C3DAPP,"CEGUI first initialization done\n");

		return true;
	}

	void C3DApplication::releaseOgre()
	{
		if(m_renderWindow){
			m_renderWindow->removeAllViewports();
			m_root->detachRenderTarget(m_renderWindow);
			delete m_renderWindow;
			m_renderWindow = nullptr;
		}

		if(m_ogreListener){
			delete m_ogreListener;
			m_ogreListener = nullptr;
		}

		if(m_root){
			delete m_root;
			m_root = nullptr;
		}
	}
	
	void C3DApplication::releaseOIS() {
		
		if (m_mouse) {
			m_inputSystem->destroyInputObject(m_mouse);
			m_mouse = nullptr;
		}

		if (m_keyboard) {
			m_inputSystem->destroyInputObject(m_keyboard);
			m_keyboard = nullptr;
		}

		if (m_inputSystem)	{
			m_inputSystem->destroyInputSystem(m_inputSystem);
			m_inputSystem = nullptr;
		}
	}

	void C3DApplication::releaseCEGUI()
	{
		//m_xmlParser ???
		if(m_GUISystem){
			CEGUI::System::destroy();
			m_GUISystem = nullptr;
		}
	}
}