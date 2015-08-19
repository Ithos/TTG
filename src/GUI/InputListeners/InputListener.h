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

#ifndef __GUI_INPUTLISTENER_INPUTLISTENER_H
#define __GUI_INPUTLISTENER_INPUTLISTENER_H

#include "../../Common/Input/Input.h"

namespace GUI
{
	namespace InputListener
	{

		class CMouseState
		{
		public:
			CMouseState()
			{
				width = 800;
				height = 600;
				posAbsX = 0;
				posAbsY = 0;
				posRelX = 0;
				posRelY = 0;
				movX = 0;
				movY = 0;
				scrool = 0;
				button =  
					Common::Input::Button::TMouseButton::UNASSIGNED;
			}
			
			CMouseState(unsigned int width, unsigned int height,
				unsigned int posAbsX = 0, unsigned int posAbsY = 0)
			{
				setExtents(width,height);
				setPosition(posAbsX,posAbsY);
				movX = 0;
				movY = 0;
				scrool = 0;
				button = 
					Common::Input::Button::TMouseButton::UNASSIGNED;
			}

			void setExtents(unsigned int width, unsigned int height)
			{
				this->width = width;
				this->height = height;
			}

			void setPosition(unsigned int posAbsX, unsigned int posAbsY)
			{
				this->posAbsX = posAbsX;
				this->posAbsY = posAbsY;
				this->posRelX = (float) posAbsX / (float) width;
				this->posRelY = (float) posAbsY / (float) height;
			}



			unsigned int width;
			unsigned int height;
			
			unsigned int posAbsY;
			unsigned int posAbsX;
			
			float posRelX;
			float posRelY;
			
			int movX;
			int movY;
			
			int scrool;
			Common::Input::Button::TMouseButton button;
		};

		class CMouseListener
		{
		public:
			virtual bool mouseMoved(const CMouseState &mouseState){return false;}
			virtual bool mousePressed(const CMouseState &mouseState){return false;}
			virtual bool mouseReleased(const CMouseState &mouseState){return false;}
		};

		class CkeyBoardListener
		{
		public:
			virtual bool keyPressed(Common::Input::TKey key){return false;}
			virtual bool keyReleased(Common::Input::TKey key){return false;}
			/// TODO -- Polling methods -- ///
			virtual bool pollingKeyCheckPressed(Common::Input::TKey key){return false;}
			virtual bool pollingKeyCheckReleased(Common::Input::TKey key){return false;}
		};
	}
}

#endif