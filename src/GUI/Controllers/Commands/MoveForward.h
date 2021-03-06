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

#ifndef __GUI_MOVEFORWARD_H
#define __GUI_MOVEFORWARD_H

#include "Command.h"

namespace GUI
{
	namespace Controller
	{
		namespace Command
		{
			class CMoveForward : public ICommand
			{
			public:
				CMoveForward() : ICommand(){}
				virtual ~CMoveForward(){}

				virtual void execute();
				virtual void execute(Logic::CEntity *entity);
				virtual void execute(Common::Input::Action::TKeyMouseAction action, Logic::CEntity *entity);
			};
		}
	}
}

#endif