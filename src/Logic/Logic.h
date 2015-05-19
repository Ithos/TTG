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

#ifndef __LOGIG_LOGIC_H
#define __LOGIG_LOGIC_H

#include <string>

#include "../Common/Data/TTG_Types.h"

namespace Logic 
{
	class CScene;
	class CEntity;
}

namespace Ogre
{
	class Root;
	class RenderWindow;
}

namespace Logic
{
	class CLogic
	{
	public:
		static CLogic* getInstance(){return m_instance;}
		static bool init();
		static void release();

		void tick(unsigned int msecs);
		bool activateScene();
		void deactivateScene();
		CScene *getScene(){return m_scene;}

		bool loadLevel(const std::string &filename,
			Ogre::Root* root,
			Ogre::RenderWindow* render,
			Common::Data::SceneType type);
		void unloadLevel();

	protected:
	private:
		CLogic();
		~CLogic();

		bool open();
		void close();

		CScene *m_scene;

		static CLogic* m_instance;
	};
}

#endif