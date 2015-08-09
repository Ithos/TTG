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

#include "OptionsMenu.h"

#include "../../Common/Sound/Sound.h"

#include <log.h>

#include <CEGUI/CEGUI.h>
#include <OGRE/OgreRoot.h>

namespace Application
{
	const char* const LOG_COPTIONSMENU = "Application::COptionsMenu";

	COptionsMenu::~COptionsMenu()
	{
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("CancelContainer")->getChildElement("CancelLabel")->removeAllEvents();
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ApplyContainer")->getChildElement("ApplyLabel")->removeAllEvents();
		static_cast<CEGUI::Combobox*>(m_menuWindow->getChild("InnerButtonsContainer/ResolutionCombobox"))->removeAllEvents();
		static_cast<CEGUI::Combobox*>(m_menuWindow->getChild("InnerButtonsContainer/ColourDepthCombobox"))->removeAllEvents();
		static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/SoundFXSlider"))->removeAllEvents();
		static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/MenuFXSlider"))->removeAllEvents();
		static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/MusicSlider"))->removeAllEvents();
	}

	bool COptionsMenu::init()
	{
		log_trace(LOG_COPTIONSMENU,"Init\n");

		CApplicationState::init();

		m_soundInstance = Common::Sound::CSound::getSingletonPtr();

		m_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Options.layout");

		setupWindows();

		setupAnimatons();

		//Sounds & music

		Common::Sound::CSound::getSingletonPtr()->addSound("Button2.wav","buttonSoundOptions");
		Common::Sound::CSound::getSingletonPtr()->addSound("SoundIntro2.wav","buttonHoverOptions");
		Common::Sound::CSound::getSingletonPtr()->addSound("Intro1.wav","introPauseSoundOptions");
		Common::Sound::CSound::getSingletonPtr()->addSound("Failed2.wav","failedSoundOptions");

		return true;
	}

	void COptionsMenu::release() 
	{
		log_trace(LOG_COPTIONSMENU,"Release\n");

		Common::Sound::CSound::getSingletonPtr()->releaseSound("buttonSoundOptions");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("buttonHoverOptions");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("introPauseSoundOptions");
		Common::Sound::CSound::getSingletonPtr()->releaseSound("failedSoundOptions");

		CApplicationState::release();

	}

	void COptionsMenu::activate() 
	{
		log_trace(LOG_COPTIONSMENU,"Activate\n");

		CApplicationState::activate();

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(m_menuWindow);
		m_menuWindow->setVisible(true);
		m_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		onActivate();
		Common::Sound::CSound::getSingletonPtr()->playDelayedSound("introPauseSoundOptions",400);
	}

	void COptionsMenu::deactivate() 
	{	
		log_trace(LOG_COPTIONSMENU,"Deactivate\n");

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		m_menuWindow->deactivate();
		m_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	}

	void COptionsMenu::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	}

	bool COptionsMenu::keyPressed(Common::Input::TKey key)
	{
		return false;
	} 

	bool COptionsMenu::keyReleased(Common::Input::TKey key)
	{
		return false;
	} 

	bool COptionsMenu::mouseMoved(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}
		
	bool COptionsMenu::mousePressed(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool COptionsMenu::mouseReleased(const GUI::InputListener::CMouseState &mouseState)
	{
		return false;

	}

	bool COptionsMenu::applyReleased(const CEGUI::EventArgs& e)
	{
		
		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){

			CEGUI::Scrollbar* Slider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/SoundFXSlider"));
			m_soundInstance->setSoundEffectsVolume(Slider->getScrollPosition() > 1.0f ? 1.0f : Slider->getScrollPosition());

			Slider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/MenuFXSlider"));
			m_soundInstance->setMenuSoundsVolume(Slider->getScrollPosition() > 1.0f ? 1.0f : Slider->getScrollPosition());

			Slider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/MusicSlider"));
			m_soundInstance->setMusicVolumeVolume(Slider->getScrollPosition() > 1.0f ? 1.0f : Slider->getScrollPosition());

			CEGUI::Combobox* Resolution( static_cast<CEGUI::Combobox*>(m_menuWindow->getChild("InnerButtonsContainer/ResolutionCombobox")) );
			CEGUI::Combobox* ColourDepth( static_cast<CEGUI::Combobox*>(m_menuWindow->getChild("InnerButtonsContainer/ColourDepthCombobox")) );
			if(Resolution->getSelectedItem() != nullptr && ColourDepth->getSelectedItem() != nullptr)
			{
				Ogre::ConfigOptionMap& options = m_app->getOgreRoot()->getRenderSystem()->getConfigOptions();
				Ogre::NameValuePairList newOptions;

				for(auto it = options.begin(); it != options.end(); ++it){
					if(it->first == "Colour Depth"){

						newOptions[it->first] = Ogre::String(ColourDepth->getSelectedItem()->getText().c_str());

					}else if(it->first == "Video Mode"){

						Ogre::String str(Ogre::String(Resolution->getSelectedItem()->getText().c_str()) + " @ " 
							+ Ogre::String(ColourDepth->getSelectedItem()->getText().c_str()) + "-bit colour");

						newOptions[it->first] = str;

					}else if(it->first == "Full Screen"){

						newOptions[it->first] = static_cast<CEGUI::ToggleButton*>(m_menuWindow->getChild("InnerButtonsContainer/FullScreenButton"))
							->isSelected() ? "Yes" : "No";

					}else if(it->first == "VSync"){/// TODO ///

						newOptions[it->first] = static_cast<CEGUI::ToggleButton*>(m_menuWindow->getChild("InnerButtonsContainer/VSyncButton"))
							->isSelected() ? "Yes" : "No";

					}else{

						newOptions[it->first] = it->second.currentValue;

					}
				}

				m_app->reconfigure(newOptions);

				Common::Sound::CSound::getSingletonPtr()->playSound("buttonSoundOptions");

			}else{

				m_menuWindow->getChild("BotBar/BotBarLabel")->setVisible(true);

				Common::Sound::CSound::getSingletonPtr()->playSound("failedSoundOptions");
			}

			

		}
		return false;
	}

	bool COptionsMenu::cancelReleased(const CEGUI::EventArgs& e)
	{
		if(static_cast<const CEGUI::MouseEventArgs&>(e).button == CEGUI::LeftButton){
			Common::Sound::CSound::getSingletonPtr()->playSound("buttonSoundOptions");
			m_app->popState();
		}
		return false;
	}

	bool COptionsMenu::onApplyLabelExited(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ApplyContainer")->getChildElement("ApplyLabel"))->
			setProperty("NormalTextColour","FF00A9FF");
		return false;
	}

	bool COptionsMenu::onApplyLabelEntered(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ApplyContainer")->getChildElement("ApplyLabel"))->
			setProperty("NormalTextColour","FFFFFFFF");
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonHover");
		return false;
	}
	bool COptionsMenu::onCancelLabelEntered(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("CancelContainer")->getChildElement("CancelLabel"))->
			setProperty("NormalTextColour","FFFFFFFF");
		Common::Sound::CSound::getSingletonPtr()->playSound("buttonHover");
		return false;
	}

	bool COptionsMenu::onCancelLabelExited(const CEGUI::EventArgs& e)
	{
		static_cast<CEGUI::DefaultWindow*>(m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("CancelContainer")->getChildElement("CancelLabel"))->
			setProperty("NormalTextColour","FF00A9FF");
		return false;
	}

	void COptionsMenu::setupWindows()
	{
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("CancelContainer")->getChildElement("CancelLabel")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&COptionsMenu::cancelReleased, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("CancelContainer")->getChildElement("CancelLabel")->
			subscribeEvent(CEGUI::Window::EventMouseEntersArea, 
			CEGUI::SubscriberSlot(&COptionsMenu::onCancelLabelEntered, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("CancelContainer")->getChildElement("CancelLabel")->
			subscribeEvent(CEGUI::Window::EventMouseLeavesArea, 
			CEGUI::SubscriberSlot(&COptionsMenu::onCancelLabelExited, this));

		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ApplyContainer")->getChildElement("ApplyLabel")->
			subscribeEvent(CEGUI::Window::EventMouseClick, 
				CEGUI::SubscriberSlot(&COptionsMenu::applyReleased, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ApplyContainer")->getChildElement("ApplyLabel")->
			subscribeEvent(CEGUI::Window::EventMouseEntersArea, 
			CEGUI::SubscriberSlot(&COptionsMenu::onApplyLabelEntered, this));
		m_menuWindow->getChildElement("InnerButtonsContainer")->getChildElement("ApplyContainer")->getChildElement("ApplyLabel")->
			subscribeEvent(CEGUI::Window::EventMouseLeavesArea, 
			CEGUI::SubscriberSlot(&COptionsMenu::onApplyLabelExited, this));

		// Resolution combobox
		CEGUI::Combobox* resolution = static_cast<CEGUI::Combobox*>(m_menuWindow->getChild("InnerButtonsContainer/ResolutionCombobox"));
		resolution->setReadOnly(true);

		CEGUI::ListboxTextItem* itemCombobox(new CEGUI::ListboxTextItem("1024 x 768", 0));
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		resolution->addItem(itemCombobox);

		itemCombobox = new CEGUI::ListboxTextItem("1280 x 720", 1);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		resolution->addItem(itemCombobox);

		itemCombobox = new CEGUI::ListboxTextItem("1366 x 768", 2);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		resolution->addItem(itemCombobox);

		itemCombobox = new CEGUI::ListboxTextItem("640 x 480", 3);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		resolution->addItem(itemCombobox);

		itemCombobox = new CEGUI::ListboxTextItem("720 x 480", 4);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		resolution->addItem(itemCombobox);

		itemCombobox = new CEGUI::ListboxTextItem("720 x 576", 5);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		resolution->addItem(itemCombobox);

		itemCombobox = new CEGUI::ListboxTextItem("800 x 600", 6);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		resolution->addItem(itemCombobox);

		//Colour depth combobox
		CEGUI::Combobox* colourDepth = static_cast<CEGUI::Combobox*>(m_menuWindow->getChild("InnerButtonsContainer/ColourDepthCombobox"));
		colourDepth->setReadOnly(true);

		itemCombobox = new CEGUI::ListboxTextItem("16", 0);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		colourDepth->addItem(itemCombobox);

		itemCombobox = new CEGUI::ListboxTextItem("32", 1);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		colourDepth->addItem(itemCombobox);

		//Volume sliders
		CEGUI::Scrollbar* SoundFXSlider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/SoundFXSlider"));
		SoundFXSlider->setDocumentSize(1.11f);
		SoundFXSlider->setPageSize(0.1f);
		SoundFXSlider->setStepSize(0.01f);
		SoundFXSlider->setScrollPosition(m_soundInstance->getSoundEffectsVolume());
		m_menuWindow->getChild("InnerButtonsContainer/SoundFXNum")->setText(std::to_string((int)(SoundFXSlider->getScrollPosition() * 100)));
		SoundFXSlider->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged,CEGUI::SubscriberSlot(&COptionsMenu::onSoundFXChanged, this));

		CEGUI::Scrollbar* MenuFXSlider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/MenuFXSlider"));
		MenuFXSlider->setDocumentSize(1.11f);
		MenuFXSlider->setPageSize(0.1f);
		MenuFXSlider->setStepSize(0.01f);
		MenuFXSlider->setScrollPosition(m_soundInstance->getMenuSoundsVolume());
		m_menuWindow->getChild("InnerButtonsContainer/MenuFXNum")->setText(std::to_string((int)(MenuFXSlider->getScrollPosition() * 100)));
		MenuFXSlider->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged,CEGUI::SubscriberSlot(&COptionsMenu::onMenuSoundsChanged, this));

		CEGUI::Scrollbar* MusicSlider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/MusicSlider"));
		MusicSlider->setDocumentSize(1.11f);
		MusicSlider->setPageSize(0.1f);
		MusicSlider->setStepSize(0.01f);
		MusicSlider->setScrollPosition(m_soundInstance->getMusicVolume());
		m_menuWindow->getChild("InnerButtonsContainer/MusicNum")->setText(std::to_string((int)(MusicSlider->getScrollPosition() * 100)));
		MusicSlider->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged,CEGUI::SubscriberSlot(&COptionsMenu::onMusicChanged, this));
	}

	void COptionsMenu::setupAnimatons()
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
	}

	bool COptionsMenu::onSoundFXChanged(const CEGUI::EventArgs& e)
	{
		CEGUI::Scrollbar* Slider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/SoundFXSlider"));
		m_menuWindow->getChild("InnerButtonsContainer/SoundFXNum")->setText(std::to_string((int)(Slider->getScrollPosition() * 100 > 100 ? 100 : Slider->getScrollPosition() * 100)));

		return true;
	}

	bool COptionsMenu::onMenuSoundsChanged(const CEGUI::EventArgs& e)
	{
		CEGUI::Scrollbar* Slider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/MenuFXSlider"));
		m_menuWindow->getChild("InnerButtonsContainer/MenuFXNum")->setText(std::to_string((int)(Slider->getScrollPosition() * 100 > 100 ? 100 : Slider->getScrollPosition() * 100)));

		return true;
	}

	bool COptionsMenu::onMusicChanged(const CEGUI::EventArgs& e)
	{
		CEGUI::Scrollbar* Slider = static_cast<CEGUI::Scrollbar*>(m_menuWindow->getChild("InnerButtonsContainer/MusicSlider"));
		m_menuWindow->getChild("InnerButtonsContainer/MusicNum")->setText(std::to_string((int)(Slider->getScrollPosition() * 100 > 100 ? 100 : Slider->getScrollPosition() * 100)));

		return true;
	}

	void COptionsMenu::onActivate()
	{
		m_topBarAnimInst->start();
		m_botBarAnimInst->start();
		m_insideAnimInst->start();
	}
}