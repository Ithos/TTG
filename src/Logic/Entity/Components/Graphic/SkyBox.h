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

#ifndef __LOGIC_SKYDOME_H
#define __LOGIC_SKYDOME_H

#include "../Component.h"

namespace Ogre
{
	class Entity;
	class SceneManager;
}

namespace Logic
{
    namespace Component 
    {
	    class CSkyBox : public IComponent
	    {
		    DEC_FACTORY(CSkyDome);
	    public:
		    CSkyBox() : IComponent(), m_ogreScene(nullptr) {}
            ~CSkyBox() {}
            bool spawn(CEntity*, CScene*, const Map::CMapEntity*);

	    protected:
		    Ogre::SceneManager* m_ogreScene;
	    };

	    REG_FACTORY(CSkyBox);
    }
}

#endif
