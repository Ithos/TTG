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

#include "SkyBox.h"
#include <OgreSceneManager.h>

#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include <Common/Map/MapEntity.h>

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CSkyBox);

    bool CSkyBox::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
    {
        m_entity->getScene()->getSceneManager()->setSkyBox(true, "skybox_system", 500);
		return true;
    }
}