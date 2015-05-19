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

#include "SlowDown.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Movement/Controller.h"

namespace GUI
{
	namespace Controller
	{
		namespace Command
		{
			void CSlowDown::execute(){}
			void CSlowDown::execute(Logic::CEntity *entity){}

			void CSlowDown::execute(Common::Input::Action::TKeyMouseAction action,
					                 Logic::CEntity *entity)
			{
				//TODO call entity and send the message to the components
				// or just find the desire component and execute the command
				if(entity->isActivated()){
					Logic::Component::CController* comp = static_cast<Logic::Component::CController*>(entity->getComponentByName("CController"));
					comp->slowDown(action == Common::Input::Action::KEY_PRESSED);
				}
			}
		}
	}
}