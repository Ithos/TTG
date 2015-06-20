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

#ifndef __COMMON_DATA__SCENETYPE_H
#define __COMMON_DATA__SCENETYPE_H

#include <string>
#include <vector>
#include <map>

namespace Common
{
	namespace Data
	{
		enum SceneType 
		{
			UNASSIGNED = 0xFFFFFFFF,
			BATTLE     = 0x00000000,
			GALAXY,
			SYSTEM,
			DUMMY
		};

        enum GameType
        {
            MENU_STATE = 0,
            GAME_STATE,
			TMP_STATE
        };

		enum SelectionMask
		{
			NULL_MASK = 0,
            STAR_MASK = 1<<0,
            PLANET_MASK = 1<<1,
			ORBIT_MASK = 1<<2,
			SYSTEM_MASK = 1<<3
		};

		enum CameraMask
		{
			NULL_CAMERA_MASK = 0,
            NON_TEXTURE_MASK = 1<<0,
			ONLY_TEXTURE_MASK = 1<<1
		};

        typedef enum Weapons_t
        {
            ALL = -1, 
            LASER = 0, 
            MISSILE_LINEAR,
            END
        };

		namespace Blueprints
		{
			struct TXMLComponent
			{
				int number;
				int priority;
				std::string name;
			};
			typedef std::vector<TXMLComponent> TComponentsVector;
			typedef std::map<std::string,TComponentsVector> TBluePrintsMap;

		}

		const int Y_COOR = -300;

        // Components
        const char* const TRANSFORM_COMP        = "CTransform";
        const char* const GRAPHICS_COMP         = "CGraphics";
        const char* const ROTATION_COMP         = "CRotation";
        const char* const ORBIT_MOV_COMP        = "COrbitMovement";
        const char* const CAMERA_COMP           = "CCamera";
        const char* const INTERPOLATION_COMP    = "CInterpolation";
        const char* const MOVEMENT_COMP         = "CMovement";
        const char* const CAMERA_COMPONENT_COMP = "CCameraComponent"; // ???
		const char* const STEERING_MOV			= "CSteeringMovement";
        const char* const CONTROLLER_COMP       = "CController";
        const char* const WEAPONS_COMP          = "CWeapons";
        const char* const LIFE_COMP             = "CLife";
        const char* const SHIELD_COMP           = "CShield";
        const char* const SHIELD_TRIGGER        = "CShieldTrigger";
        const char* const MISSILE_TRIGGER       = "CMissileTrigger";
		const char* const STATE_MACHINE_COMP    = "CStateMachineExecutor";

		//Physic
		const char* const PHYSIC_CONTROLLER_COMP = "CPhysicController";
		const char* const PHYSIC_STATIC_COMP     = "CPhysicStaticActor";
		const char* const PHYSIC_DYNAMIC_COMP    = "CPhysicDynamicActor";
		const char* const PHYSIC_KINEMATIC_COMP  = "CPhysicKinematicActor";
		//Console
		const char* const GRAPHIC_PRIMITIVE_COMP = "CGraphicPrimitive";

	} // data namespace
}

#endif