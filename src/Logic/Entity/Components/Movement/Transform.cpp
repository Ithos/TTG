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

#include "Transform.h"

#include <OgreSceneNode.h>

#include <Common/Map/MapEntity.h>
#include <Common/Util/Math.h>
#include <Common/Data/Spawn_Constants.h>

#include "Logic/Entity/Entity.h"

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CTransform);


	bool CTransform::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

        using namespace Common::Data::Spawn;

		if(entityInfo->hasAttribute(COMMON_POSITION)) {
			Vector3 position = entityInfo->getVector3Attribute(COMMON_POSITION);
			m_transform.setTrans(position);
		}

		if(entityInfo->hasAttribute(COMMON_ORIENTATION)) {
			Vector3 orientation(entityInfo->getVector3Attribute(COMMON_ORIENTATION));//x=yaw,y=pitch,z=roll
			orientation.x = Common::Util::Math::fromDegreesToRadians(orientation.x);
			orientation.y = Common::Util::Math::fromDegreesToRadians(orientation.y);
			orientation.z = Common::Util::Math::fromDegreesToRadians(orientation.z);
			Common::Util::Math::setRotation(orientation,m_transform);
		}
		
        return true;
	}
}
