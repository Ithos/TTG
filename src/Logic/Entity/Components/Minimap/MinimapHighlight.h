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

#ifndef __LOGIC_BLUE_HIGHLIGHT_H
#define __LOGIC_BLUE_HIGHLIGHT_H

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
		class CMinimapHighlight : public IComponent
		{
			DEC_FACTORY(CMinimapHighlight);

			CMinimapHighlight() : IComponent(), m_alphaEntity(nullptr),m_glowEntity(nullptr), m_ogreScene(nullptr), m_alphaNode(nullptr),
				m_glowNode(nullptr), m_scale(1.0), m_enemy(false)
			{}

			virtual bool activate();
			virtual void deactivate();

			virtual ~CMinimapHighlight();
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

		protected:
			virtual void createGraphicEntity(const Map::CMapEntity* entityInfo, CEntity* entity);

			Ogre::Entity* m_alphaEntity, *m_glowEntity;
			Ogre::SceneManager* m_ogreScene;
			Ogre::SceneNode* m_alphaNode, *m_glowNode;
			float m_scale;
			bool m_enemy;
		};

		REG_FACTORY(CMinimapHighlight);

	}
}


#endif