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

#ifndef __LOIGC_COMPONENT_PHYSICENTITY_H
#define __LOIGC_COMPONENT_PHYSICENTITY_H

#include "IPhysic.h"

#include <Common/Util/Math.h>

namespace physx
{
	class PxRigidActor;
	class PxRigidStatic;
}

namespace Common { namespace Physic {
	class CPhysicManager;
}}

namespace Logic { namespace Component {

	class CPhysicEntity : public IPhysic
	{
		DEC_FACTORY(CPhysicEntity);
	public:
		CPhysicEntity();
		virtual ~CPhysicEntity();

		virtual bool spawn(CEntity* entity, CScene *scene, const Map::CMapEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool activate();
		virtual void deactivate();

		virtual void onOverlapBegin(IPhysic* otherComponent);
		virtual void onOverlapEnd(IPhysic* otherComponent);
		virtual void onContact(IPhysic* otherComponent);

		physx::PxRigidActor* m_actor;

	protected:
	private:
		physx::PxRigidActor* createActor(const Map::CMapEntity *entityInfo);

		physx::PxRigidStatic* createPlane(const Map::CMapEntity *entityInfo);
				
		physx::PxRigidActor* createRigid(const Map::CMapEntity *entityInfo);
		
		physx::PxRigidActor* createFromFile(const Map::CMapEntity *entityInfo);

		Common::Physic::CPhysicManager* m_physicMng;

		Vector3 m_movement;

		Matrix4 m_lastPosition;

		bool m_onContact;
	};
	REG_FACTORY(CPhysicEntity);
}}

#endif