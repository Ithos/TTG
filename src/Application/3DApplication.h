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

#ifndef __APPLICATION_3DAPPLICATION_H
#define __APPLICATION_3DAPPLICATION_H

#ifdef _DEBUG
#define NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
#endif

#include "BaseApplication.h"
#include <OgreCommon.h>

namespace Ogre
{
	class Timer;
	class SceneManager;
}

namespace OIS
{
	class InputManager;
	class Mouse;
	class Keyboard;
}

namespace CEGUI
{
	class XMLParser;
	class System;
}

namespace Application
{
	class CWindowEventListener;
    class CClock;
}

namespace Application
{

	class C3DApplication : public CBaseApplication
	{
	public:
        C3DApplication();
        ~C3DApplication();

        virtual bool init(); 
		virtual bool secondInit();
        virtual void release();

		virtual void reconfigure(Ogre::NameValuePairList& options);

		void reRenderCEGUI();

	protected:
        virtual void tick(unsigned int msecs);

		CEGUI::System*        m_GUISystem;

	private:
        bool initOgre();
        bool initOIS();
        bool initCEGUI();

        bool initOgreResources();
		bool secondInitOgre();

        void releaseOgre();
        void releaseOIS();
        void releaseCEGUI();

        //OIS
		OIS::InputManager*    m_inputSystem;
		OIS::Keyboard*        m_keyboard;
		OIS::Mouse*           m_mouse;
        // CEGUI
		CEGUI::XMLParser*     m_xmlParser;
        // listener
        CWindowEventListener* m_ogreListener;


	};
}

#endif