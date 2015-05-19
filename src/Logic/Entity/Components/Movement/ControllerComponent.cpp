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

#include "ControllerComponent.h"

#include <Common/Map/MapEntity.h>

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CControllerComponent);

	bool CControllerComponent::spawn(CEntity* entity, CScene *scene, const Map::CMapEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo)) return false;

		if(entityInfo->hasAttribute("speed"))
			m_speed = entityInfo->getFloatAttribute("speed");
		return true;
	}

	bool CControllerComponent::activate()
	{
		return true;
	}

	void CControllerComponent::deactivate()
	{

	}

	void CControllerComponent::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(m_moveForward){
			//TODO calculate the new direction vector
			//TODO emit msg to the components or just call the components
			/*WARNING right now there is NO physX so we're gonna move the 
			  player manually BUT if we use physX this should be do with
			  the PhysicComponent...
			*/
		}
	}

	bool CControllerComponent::accept(const IMessage & msg)
	{
		//TODO
		return true;
	}

	void CControllerComponent::process(const IMessage & msg)
	{
		//TODO switch msg and call the appropiate method
	}

	void CControllerComponent::moveForward()
	{
		m_moveForward = true;
		//TODO emit msg if needed for other components
	}

    void CControllerComponent::slowDown()
    {
        m_braking = true;
    }

    void CControllerComponent::primaryShoot()
    {
    }

    void CControllerComponent::secondaryShoot()
    {
    }

    void CControllerComponent::turnRight()
    {
    }

    void CControllerComponent::turnLeft()
    {
    }
}