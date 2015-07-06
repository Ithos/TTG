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

#include "PlayerController.h"

#include <CEGUI/CEGUI.h>
#include <Logic/Entity/Entity.h>
#include <Logic/Scene/Scene.h>

#include "GUI\InputManager.h"
#include "Commands\MoveForward.h"
#include "Commands\SlowDown.h"
#include "Commands\TurnLeft.h"
#include "Commands\TurnRight.h"
#include "Commands\Shoots.h"

namespace GUI
{
	namespace Controller
	{
		CPlayerController::CPlayerController() : m_avatar(0)
		{
		}

		CPlayerController::~CPlayerController()
		{
			deactivate();
			m_avatar = 0;

            for (auto it = m_keyCommands.begin(); it != m_keyCommands.end(); ++it)
                delete it->second;

            m_keyCommands.clear();
		}

		bool CPlayerController::setConfiguration(const std::string &filename)
		{
			/*TODO loads the key/command pairs and inserts them in the m_commands map
			  It could be hardcoded but it will be better if the commands were
			  created by a factory like the components
			*/
            using namespace Common::Input::Key;
			m_keyCommands.insert(std::pair<TKeyID, Command::ICommand*>(TKeyID::W, new Command::CMoveForward()));
			m_keyCommands.insert(std::pair<TKeyID, Command::ICommand*>(TKeyID::S, new Command::CSlowDown()));
			m_keyCommands.insert(std::pair<TKeyID, Command::ICommand*>(TKeyID::A, new Command::CTurnLeft()));
			m_keyCommands.insert(std::pair<TKeyID, Command::ICommand*>(TKeyID::D, new Command::CTurnRight()));
           /* m_keyCommands.insert(std::pair<TKeyID, Command::ICommand*>(TKeyID::SPACE, new Command::CPrimaryShoot()));
			m_keyCommands.insert(std::pair<TKeyID, Command::ICommand*>(TKeyID::LSHIFT, new Command::CSecondaryShoot()));*/

			m_mouseCommands.insert(std::pair<Common::Input::Button::TMouseButton, Command::ICommand*>(Common::Input::Button::LEFT, new Command::CPrimaryShoot()));
			m_mouseCommands.insert(std::pair<Common::Input::Button::TMouseButton, Command::ICommand*>(Common::Input::Button::RIGHT, new Command::CSecondaryShoot()));

			return true;
		}

		void CPlayerController::activate()
		{
			CInputManager::getInstance()->addKeyListener(this);
			CInputManager::getInstance()->addMouseListener(this);
		}

		void CPlayerController::deactivate()
		{
			CInputManager::getInstance()->removeKeyListener(this);
			CInputManager::getInstance()->removeMouseListener(this);
		}

		bool CPlayerController::keyPressed(Common::Input::TKey key)
		{
			if(m_avatar){
				TKeyCommandMap::const_iterator it (m_keyCommands.find(key.id));
				if(it != m_keyCommands.end()){
					m_keyCommands[key.id]->execute
						(Common::Input::Action::KEY_PRESSED,m_avatar);
					return true;
				}
			}
			return false;
		}

		bool CPlayerController::keyReleased(Common::Input::TKey key)
		{
			if(m_avatar){
				TKeyCommandMap::const_iterator it (m_keyCommands.find(key.id));
				if(it != m_keyCommands.end()){
					m_keyCommands[key.id]->execute
						(Common::Input::Action::KEY_RELEASED,m_avatar);
					return true;
				}
			}
			return false;
		}

		bool CPlayerController::mouseMoved(const InputListener::CMouseState &mouseState)
		{
			return false;
		}

		bool CPlayerController::mousePressed(const InputListener::CMouseState &mouseState)
		{
			if(m_avatar){
				CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
				Ogre::Ray mouseRay = m_avatar->getScene()->getSceneCamera()->getCameraToViewportRay(mousePos.d_x/float(mouseState.width),mousePos.d_y/float(mouseState.height));

				 Ogre::Vector3 point = mouseRay.getPoint(Ogre::Real(1300));

				 float err =0.001;
                int i =0;
                while(point.y > -295.0 || point.y < -305.0 || i < 20){
                    point = mouseRay.getPoint(Ogre::Real(1300 + point.y + 0.01*err));
                    err+=point.y;
                    i+=1;
                }
                point.y = -300.0f;

				auto it (m_mouseCommands.find(mouseState.button));
				if(it != m_mouseCommands.end()){
					m_mouseCommands[mouseState.button]->execute
						(Common::Input::Action::MOUSE_PRESSED,m_avatar,point);
					//return true; //We're not returning true because we want the mouse event to get to the GUI
				}
				
			}
			return false;
		}
		
		bool CPlayerController::mouseReleased(const InputListener::CMouseState &mouseState)
		{

			if(m_avatar){
				auto it (m_mouseCommands.find(mouseState.button));
				if(it != m_mouseCommands.end()){
					m_mouseCommands[mouseState.button]->execute
						(Common::Input::Action::MOUSE_RELEASED,m_avatar);
					//return true; //We're not returning true because we want the mouse event to get to the GUI
				}
			}

			return false;
		}
	}
}