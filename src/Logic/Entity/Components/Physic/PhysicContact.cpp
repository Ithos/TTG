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

#include "PhysicContact.h"
#include "../../Entity.h"
#include <Common\data\TTG_Types.h>
#include "../Gameplay/Life.h"
#include "../Cameras/Camera.h"

using namespace Common::Data;

namespace Logic { namespace Component {

	IMP_FACTORY(CPlayerPhysic);
	void CPlayerPhysic::onContact(IPhysic* otherComponent)
	{
        //TODO player contact
		CPhysicEntity::onContact(otherComponent);

		static_cast<CLife*>(getEntity()->getComponentByName(LIFE_COMP))->decreaseLife(10);
        CCamera::shake();
        //otherComponent->getEntity(); con la que choca*/
	}

	IMP_FACTORY(CEnemyPhysic);
	void CEnemyPhysic::onContact(IPhysic* otherComponent)
	{
		//TODO enemy contact
		CPhysicEntity::onContact(otherComponent);
	}
}}