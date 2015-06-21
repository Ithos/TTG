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

namespace Common
{
	namespace Data
	{
		namespace Spawn
		{
			enum onContactFunction
			{
				PLAYER,
				ENEMY,
				METEOR
			};

			// Common
			const char* const COMMON_POSITION       = "position";
			const char* const COMMON_ORIENTATION    = "orientation";
			const char* const COMMON_IS_PLAYER      = "isPlayer";
			const char* const COMMON_SPEED          = "speed";
			const char* const COMMON_YAW			= "yaw";
			const char* const COMMON_PITCH          = "pitch";
            const char* const COMMON_ROLL           = "roll";
			const char* const COMMON_ROTATION_SPEED = "rotationSpeed";
			const char* const COMMON_DIFFUSE_COLOUR	= "diffuseColour";
			const char* const COMMON_SPECULAR_COLOUR= "specularColour";
			const char* const COMMON_COLOUR_VALUE	= "colourValue";
			const char* const COMMON_SELECTION_MASK	= "SelectionMask";
			const char* const COMMON_NO_TARGET		= "-";
			const char* const COMMON_TARGET			= "!";
			const char* const COMMON_INHABITED		= "$";
			const char* const COMMON_DESERTED		= "%";
            const char* const COMMON_MAXROLL        = "maxRoll";
            const char* const COMMON_ROLLSPEED      = "speedRoll";
            const char* const COMMON_FRICTION       = "friction";
            const char* const COMMON_DESACELERATION = "desaceleration";
            const char* const COMMON_MAX_SPEED      = "maxSpeed";
            const char* const COMMON_ACCEL			= "accel";
			const char* const COMMON_ROTATION_ACCEL	= "rotationAccel";
            const char* const COMMON_LIFE           = "life";
            const char* const COMMON_SHIELD_RESIS   = "shield_resistance";
            const char* const COMMON_SHIELD         = "shield";
            const char* const COMMON_PRIMARY_WEAPON = "primary";
            const char* const COMMON_SECONDARY_WEAPON = "secondary";
			//AI
			const char* const AI_TOLERANCE			= "tolerance";
			const char* const AI_BEHAVIOR			= "behavior";
			// Graphic
			const char* const GRAPHIC_STATIC		= "static";
			const char* const GRAPHIC_MODEL			= "model";
			const char* const GRAPHIC_SCALE			= "scale";
			const char* const GRAPHIC_GLOW			= "glow";
			const char* const GRAPHIC_GREEN_GLOW	= "greenglow";
			const char* const GRAPHIC_GLOW_SCALE	= "glow_scale";
			const char* const GRAPHIC_EXTRA			= "extra";
			const char* const GRAPHIC_VISIBILITY	= "VisibilityMask";
			// Physic
			const char* const PHYSIC_ENTITY				= "physic_entity";
			const char* const PHYSIC_FILE				= "physic_file";
			const char* const PHYSIC_GROUP				= "physic_group";
			const char* const PHYSIC_RADIUS				= "physic_radius";
			const char* const PHYSIC_HEIGHT				= "physic_height";
			const char* const PHYSIC_MASS				= "physic_mass";
			const char* const PHYSIC_TRIGGER			= "physic_trigger";
			const char* const PHYSIC_DIMENSION			= "physic_dimension";
			const char* const PHYSIC_NORMAL				= "physic_normal";
			const char* const PHYSIC_TRIGGER_ELEMENT    = "physic_trigger_element";
			const char* const PHYSIC_CONTACT_FUNCTION   = "physic_contact_function";
            // Camera
            const char* const CAMERA_TYPE			= "Camera"; // same as entity name
            const char* const CAMERA_NAME			= "name";
            const char* const CAMERA_NEAR			= "near";
            const char* const CAMERA_FAR			= "far";
            const char* const CAMERA_FOV			= "fov";
            const char* const CAMERA_HEIGHT			= "height";
            const char* const CAMERA_POSITION		= COMMON_POSITION;
            const char* const CAMERA_ORIENTATION	= COMMON_ORIENTATION;
			const char* const CAMERA_DIST_CONSTANT	= "dist_constant";
			const char* const CAMERA_MIN_DISTANCE	= "min_distance";
			const char* const CAMERA_ACCEL			= "accel";
            // Static Camera
            const char* const STATIC_CAMERA_TYPE        = "StaticCamera"; // same as entity name
            const char* const STATIC_CAMERA_NAME        = "name";
            const char* const STATIC_CAMERA_NEAR        = "static_near";
            const char* const STATIC_CAMERA_FAR         = "static_far";
            const char* const STATIC_CAMERA_FOV         = "static_fov";
            const char* const STATIC_CAMERA_POSITION    = COMMON_POSITION;
            const char* const STATIC_CAMERA_ORIENTATION = COMMON_ORIENTATION;
            const char* const STATIC_CAMERA_LOOKAT      = "static_lookat";
			// Texture Camera
			const char* const TEXTURE_CAMERA_ORTHO         = "texture_cam_ortho"; 
			const char* const TEXTURE_CAMERA_ORTHO_WIDTH   = "texture_cam_ortho_width";
			const char* const TEXTURE_CAMERA_ORTHO_HEIGHT  = "texture_cam_ortho_height";
			// Minimap
			const char* const MINIMAP_ENTITY_SCALE  = "minimap_entity_scale";
			const char* const MINIMAP_ENTITY_ENEMY  = "minimap_entity_enemy";
            // Weapons
            const char* const LASER_RANGE    = "laser_range";
            const char* const LASER_DAMAGE   = "laser_damage";
            const char* const LASER_COST     = "laser_cost";
            const char* const LASERBEAM_RANGE    = "lasserbeam_ran";
            const char* const LASERBEAM_DAMAGE   = "laserbeam_dam";
            const char* const LASERBEAM_COST     = "laserbeam_cost";
            const char* const LASERBEAM_INTERVAL = "laserbeam_interval";
            //-----
            const char* const MISSILE_RANGE  = "missile_range";
            const char* const MISSILE_DAMAGE = "missile_damage";
            const char* const MISSILE_SHIELD_PEMETRATION = "missile_shield_penetration";
            const char* const MISSILE_SPEED  = "missile_speed";
            const char* const MISSILE_AMMO   = "missile_ammo";
            // SHIELD
            const char* const SHIELD_TIMEOUT = "timeout";
            const char* const SHIELD_RELOADTIME = "reloadtime";
			// Mission
			const char* const MISSION_ENEMY_TYPE = "mission_enemy_type";
		}
	}
}