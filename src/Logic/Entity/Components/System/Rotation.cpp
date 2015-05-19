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
#include "Rotation.h"
#include "../Movement/Transform.h"

#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include <Common/Map/MapEntity.h>
#include <Common/Data/TTG_Types.h>

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CRotation);

	bool CRotation::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

		if(entityInfo->hasAttribute("yaw"))
			m_rot.x = entityInfo->getFloatAttribute("yaw");

		if(entityInfo->hasAttribute("pitch"))
			m_rot.y = entityInfo->getFloatAttribute("pitch");

		if(entityInfo->hasAttribute("roll"))
			m_rot.z = entityInfo->getFloatAttribute("roll");

		return true;
	}

	void CRotation::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
        CTransform* t_target = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));
		t_target->rotateDeg(m_rot.x * msecs, m_rot.y * msecs, m_rot.z * msecs);
	}

}