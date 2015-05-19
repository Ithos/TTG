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

#include "Movement.h"

#include "KinematicMovement.h"
#include "DynamicMovement.h"

namespace AI
{
	namespace Movement
	{
		const float IMovement::ATTENUATION = 50.0f;
		const float IMovement::SLOW_RADIUS = 200.0f;

		IMovement* IMovement::getMovement(int type, float maxLinearSpeed, float maxAngularSpeed, float maxLinearAccel, float maxAngularAccel)
		{
				switch (type) {
					case MOVEMENT_KINEMATIC_SEEK:
						return new CKinematicSeek(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
						break;
					case MOVEMENT_KINEMATIC_ARRIVE:
						return new CKinematicArrive(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
						break;
					case MOVEMENT_KINEMATIC_ALIGN_TO_SPEED:
						return new CKinematicAlignToSpeed(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
						break;
					case MOVEMENT_KINEMATIC_FLEE:
						return new CKinematicFlee(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
						break;
					case MOVEMENT_DYNAMIC_SEEK:
						return new CDynamicSeek(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
						break;
					case MOVEMENT_DYNAMIC_ARRIVE:
						return new CDynamicArrive(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
						break;
					case MOVEMENT_DYNAMIC_FLEE:
						return new CDynamicFlee(maxLinearSpeed, maxAngularSpeed, maxLinearAccel, maxAngularAccel);
						break;
					default:
						return 0;
				}
		}
	}
}