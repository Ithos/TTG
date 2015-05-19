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

#ifndef __LOGIC_GRAPHICS_H
#define __LOGIC_GRAPHICS_H

#include "../Component.h"

namespace Ogre
{
	class Entity;
	class SceneManager;
	class SceneNode;
}

namespace Logic
{
	namespace Component
	{
        class CInterpolation;

		class CGraphics : public IComponent
		{
			DEC_FACTORY(CGraphics);
		public:
			CGraphics() : IComponent(), m_ogreEntity(nullptr), m_ogreScene(nullptr), m_ogreNode(nullptr), m_model(""), m_scale(0.0f), m_interpolation(nullptr)
				,m_ogrePrimitive(nullptr),m_isOgrePrimitive(false)
			{}

			 ~CGraphics();
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);
			virtual void tick(unsigned int msecs);

			virtual bool activate();
			virtual void deactivate();

			//void switchOgreEntities();
			Ogre::SceneNode* getOgreNode(){return m_ogreNode;}
			Ogre::Entity* getOgreEntity(){return m_ogreEntity;}
            const float& getScale() { return m_scale; }

		protected:
			virtual Ogre::Entity* createGraphicEntity(const Map::CMapEntity* entityInfo);
			std::string m_model;
			Ogre::Entity* m_ogreEntity;
			Ogre::SceneManager* m_ogreScene;
			Ogre::SceneNode* m_ogreNode;
			float m_scale;

			Ogre::Entity* m_ogrePrimitive;
			bool m_isOgrePrimitive;

            CInterpolation* m_interpolation;
		};

	REG_FACTORY(CGraphics);

	}
}

#endif
