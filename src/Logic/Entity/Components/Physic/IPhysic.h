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

#ifndef __LOGIC_COMPONENT_IPHYIC_H
#define __LOGIC_COMPONENT_IPHYIC_H

#include "../Component.h"

namespace physx
{
	struct PxTriggerPair;
}

enum PGROUPS 
{
	DEFAULT = 0
};

namespace Logic { namespace Component {

	class IPhysic : public IComponent
	{
	public:
		virtual void onOverlapBegin(IPhysic* otherComponent) = 0;
		virtual void onOverlapEnd(IPhysic* otherComponent) = 0;
		virtual void onContact(IPhysic* otherComponent) = 0;
	};
}}

#endif