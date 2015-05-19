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

#ifndef __LOGIC_CONTROLLERCOMPONENT_H
#define __LOGIC_CONTROLLERCOMPONENT_H

#include "../Component.h"

namespace Logic
{
	namespace Component
	{
		class CControllerComponent : public IComponent
		{
			DEC_FACTORY(CControllerComponent);
		public:
			CControllerComponent() : IComponent(),m_speed(0.0f),m_moveForward(false),m_braking(false){}

			virtual bool spawn(CEntity* entity, CScene *scene, const Map::CMapEntity *entityInfo);
			virtual bool activate();
			virtual void deactivate();

			virtual void tick(unsigned int msecs);

			virtual bool accept(const IMessage & msg);
			virtual void process(const IMessage & msg);

			void moveForward();
			void slowDown();
			void primaryShoot();
			void secondaryShoot();
			void turnRight();
			void turnLeft();

		protected:
		private:
			float m_speed;
			bool m_moveForward;
			bool m_braking;
		};
	REG_FACTORY(CControllerComponent);
	}
}
#endif