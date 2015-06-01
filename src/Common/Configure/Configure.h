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

#ifndef __COMMON_CONFIGURATION_CONFIGURE_H
#define __COMMON_CONFIGURATION_CONFIGURE_H

#include <string>
#include <map>
#include <sstream>
#include <array>

const char* const CONFIGURE_FILE = "./conf/configure.conf";

const char* const MAPS_PATH = "maps_path";
const char* const BLUEPRINTS_PATH = "blueprints_path";
const char* const LOG_LEVEL = "log_level";
const char* const SOUNDS_PATH = "sounds_path";

const char* const CONF_GENERATOR_PATH = "conf_generator_path";
const char* const CONF_CONSOLE_PATH = "conf_console_path";

// Console

const char* const CON_UI = "console_ui";

/// ----------------- GALAXY --------------------------///

// System
const char* const GEN_SYSTEM_MINGEN = "system_mingen";
const char* const GEN_SYSTEM_MAXGEN = "system_maxgen";
const char* const GEN_SYSTEM_BASENUM = "system_basenum";
const char* const GEN_SYSTEM_TYPE = "system_type";
const char* const GEN_SYSTEM_RAD = "system_rad";
const char* const GEN_SYSTEM_ZPOS = "system_zpos";
const char* const GEN_SYSTEM_MINDIST = "system_mindist";
const char* const GEN_SYSTEM_STATIC = "system_static";
const char* const GEN_SYSTEM_ORIENTATION = "system_orientation";
const char* const GEN_SYSTEM_MASK = "system_mask";
const char* const GEN_SYSTEM_GLOW = "system_glow";
const char* const GEN_SYSTEM_GLOW_SCALE = "system_glow_scale";
const char* const GEN_SYSTEM_MODEL = "system_model";


// Galaxy SkyDome
const char* const GEN_GAL_SKYDOME_NAME = "gl_skydome_name";
const char* const GEN_GAL_SKYDOME_TYPE = "gl_skydome_type";

// Static Camera
const char* const GEN_STATIC_CAMERA_ENTIYNAME   = "static_camera_entityname";
const char* const GEN_STATIC_CAMERA_ENTIYTYPE   = "static_camera_entitytype";
const char* const GEN_STATIC_CAMERA_NAME        = "static_camera_name";
const char* const GEN_STATIC_CAMERA_NEAR        = "static_camera_near";
const char* const GEN_STATIC_CAMERA_FAR         = "static_camera_far";
const char* const GEN_STATIC_CAMERA_FOV         = "static_camera_fov";
const char* const GEN_STATIC_CAMERA_POSITION    = "static_camera_position";
const char* const GEN_STATIC_CAMERA_ORIENTATION = "static_camera_orientation";
const char* const GEN_STATIC_CAMERA_LOOKAT      = "static_camera_lookat";

// Galaxy Ambient Light
const char* const GEN_GALAXY_AMBIENT_NAME = "galaxy_ambient_name";
const char* const GEN_GALAXY_AMBIENT_TYPE = "galaxy_ambient_type";
const char* const GEN_GALAXY_AMBIENT_COLOUR = "galaxy_ambient_colour";

/// ----------------- SYSTEM --------------------------///

// Star
const char* const GEN_STAR_NAME = "star_name";
const char* const GEN_STAR_TYPE = "star_type";
const char* const GEN_STAR_STATIC = "star_static";
const char* const GEN_STAR_POSITION = "star_position";
const char* const GEN_STAR_ORIENTATION = "star_orientation";
const char* const GEN_STAR_SCALE = "star_scale";
const char* const GEN_STAR_SELECTION_MASK = "star_mask";
const char* const GEN_STAR_MODEL = "star_model";
const char* const GEN_STAR_LIGHT_STATIC = "star_light_static";
const char* const GEN_STAR_LIGHT_STATIC_VALUE = "star_light_static_value";
const char* const GEN_STAR_LIGHT_DIFFUSE = "star_light_diffuse";
const char* const GEN_STAR_LIGHT_SPECULAR = "star_light_specular";

// Movable Planet
const char* const GEN_MOVPLANET_MIN_ROT = "movplanet_min_rot";
const char* const GEN_MOVPLANET_MAX_ROT= "movplanet_max_rot";
const char* const GEN_MOVPLANET_SEPARATION = "movplanet_separation";
const char* const GEN_MOVPLANET_MAX_VEL = "movplanet_max_vel";
const char* const GEN_MOVPLANET_MAX_SCALE = "movplanet_max_scale";
const char* const GEN_MOVPLANET_NAME = "movplanet_name";
const char* const GEN_MOVPLANET_TYPE = "movplanet_type";
const char* const GEN_MOVPLANET_STATIC = "movplanet_static";
const char* const GEN_MOVPLANET_SELECTION_MASK = "movplanet_selection_mask";
const char* const GEN_MOVPLANET_CENTRAL_POS = "movplanet_central_pos";
const char* const GEN_MOVPLANET_CENTER = "movplanet_center";
const char* const GEN_MOVPLANET_ANGLE = "movplanet_angle";
const char* const GEN_MOVPLANET_DISTANCE = "movplanet_distance";
const char* const GEN_MOVPLANET_ANGULAR_VEL = "movplanet_angular_vel";
const char* const GEN_MOVPLANET_GLOW = "movplanet_glow";
const char* const GEN_MOVPLANET_GREEN_GLOW = "movplanet_greenglow";
const char* const GEN_MOVPLANET_YAW = "movplanet_yaw";
const char* const GEN_MOVPLANET_CENTER_NODE_NAME = "movplanet_center_node_name";
const char* const GEN_MOVPLANET_CENTER_NAME = "movplanet_center_name";
const char* const GEN_MOVPLANET_DIRECTION = "movplanet_direction";

// Ambient Light
const char* const GEN_SYS_AMBIENT_NAME = "sysambient_name";
const char* const GEN_SYS_AMBIENT_TYPE = "sysambient_type";
const char* const GEN_SYS_AMBIENT_COLOUR = "sysambient_colour";

// Static Camera

const char* const GEN_SYS_STATIC_CAMERA_ENTIYNAME   = "sys_static_camera_entityname";
const char* const GEN_SYS_STATIC_CAMERA_ENTIYTYPE   = "sys_static_camera_entitytype";
const char* const GEN_SYS_STATIC_CAMERA_NAME        = "sys_static_camera_name";
const char* const GEN_SYS_STATIC_CAMERA_NEAR        = "sys_static_camera_near";
const char* const GEN_SYS_STATIC_CAMERA_FAR         = "sys_static_camera_far";
const char* const GEN_SYS_STATIC_CAMERA_FOV         = "sys_static_camera_fov";
const char* const GEN_SYS_STATIC_CAMERA_POSITION    = "sys_static_camera_position";
const char* const GEN_SYS_STATIC_CAMERA_ORIENTATION = "sys_static_camera_orientation";
const char* const GEN_SYS_STATIC_CAMERA_LOOKAT      = "sys_static_camera_lookat";

//Texture Camera

const char* const GEN_SYS_TEXTURE_CAMERA_ENTITYNAME	= "sys_texture_camera_entityname";
const char* const GEN_SYS_TEXTURE_CAMERA_ENTITYTYPE	= "sys_texture_camera_entitytype";
const char* const GEN_SYS_TEXTURE_CAMERA_NAME		= "sys_texture_camera_name";
const char* const GEN_SYS_TEXTURE_CAMERA_NEAR		= "sys_texture_camera_near";
const char* const GEN_SYS_TEXTURE_CAMERA_FAR		= "sys_texture_camera_far";
const char* const GEN_SYS_TEXTURE_CAMERA_FOV		= "sys_texture_camera_fov";
const char* const GEN_SYS_TEXTURE_CAMERA_POSITION	= "sys_texture_camera_position";
const char* const GEN_SYS_TEXTURE_CAMERA_LOOKAT		= "sys_texture_camera_lookat";
/// ----------------- PLANET --------------------------///

// Asteroid
const char* const GEN_ASTEROID_TYPE = "asteroids_type";
const char* const GEN_ASTEROID_NUM = "asteroids_num";
const char* const GEN_ASTEROID_MIN_NUM = "asteroids_min_num";
const char* const GEN_ASTEROID_Y_AXIS_POS = "asteroids_y_axis_position";
const char* const GEN_ASTEROID_POS = "asteroids_position";
const char* const GEN_ASTEROID_ORI = "asteroids_orientation";
const char* const GEN_ASTEROID_DIM = "asteroids_dimension";//NUM DIMENSIONS
const char* const GEN_ASTEROID_MODEL = "asteroids_model_";
const char* const GEN_ASTEROID_PHYSX_SHAPE     = "asteroids_physx_shape";
const char* const GEN_ASTEROID_PHYSX_RADIUS    = "asteroids_physx_radius_";
const char* const GEN_ASTEROID_GRAPHIC_STATIC  = "asteroids_graphic_static";
const char* const GEN_ASTEROID_GRAPHIC_SCALE   = "asteroids_graphic_scale";
const char* const GEN_ASTEROID_VISIBILITY_MASK = "asteroids_visibility_mask";
const char* const GEN_ASTEROID_LIFE            = "asteroids_max_life";

// Static Asteroid
const char* const GEN_STATIC_ASTEROID_TYPE = "static_asteroids_type";
const char* const GEN_STATIC_ASTEROID_NUM = "static_asteroids_num";
const char* const GEN_STATIC_ASTEROID_MIN_NUM = "static_asteroids_min_num";
const char* const GEN_STATIC_ASTEROID_Y_AXIS_POS = "static_asteroids_y_axis_position";
const char* const GEN_STATIC_ASTEROID_POS = "static_asteroids_position";
const char* const GEN_STATIC_ASTEROID_ORI = "static_asteroids_orientation";
const char* const GEN_STATIC_ASTEROID_DIM = "static_asteroids_dimension";//NUM DIMENSIONS
const char* const GEN_STATIC_ASTEROID_MODEL = "static_asteroids_model_";
const char* const GEN_STATIC_ASTEROID_GRAPHIC_STATIC  = "static_asteroids_graphic_static";
const char* const GEN_STATIC_ASTEROID_GRAPHIC_SCALE   = "static_asteroids_graphic_scale";
const char* const GEN_STATIC_ASTEROID_VISIBILITY_MASK = "static_asteroids_visibility_mask";

// Player
const char* const GEN_PLAYER_NAME              = "player_name";
const char* const GEN_PLAYER_TYPE              = "player_type";
const char* const GEN_PLAYER_IS_PLAYER         = "player_is_player";
const char* const GEN_PLAYER_START_POSITION    = "player_start_position";
const char* const GEN_PLAYER_START_ORIENTATION = "player_start_orientation";
const char* const GEN_PLAYER_SPEED             = "player_aceleration";
const char* const GEN_PLAYER_ROTATION_SPEED    = "player_rotation_speed";
const char* const GEN_PLAYER_MODEL             = "player_model";
const char* const GEN_PLAYER_GRAPHIC_STATIC    = "player_graphic_static";
const char* const GEN_PLAYER_PHYSX_SHAPE       = "player_physx_shape";
const char* const GEN_PLAYER_PHYSX_RADIUS      = "player_physx_radius";
const char* const GEN_PLAYER_PHYSX_HEIGHT      = "player_physx_height";
const char* const GEN_PLAYER_MINIMAP_SCALE     = "player_minimap_entity_scale";
const char* const GEN_PLAYER_MAXROLL           = "player_maxRoll";
const char* const GEN_PLAYER_ROLL_SPEED        = "player_rollSpeed";
const char* const GEN_PLAYER_FRICTION          = "player_friction";
const char* const GEN_PLAYER_DESACELERATION    = "player_desaceleration";
const char* const GEN_PLAYER_MAX_SPEED         = "player_maxSpeed";
const char* const GEN_PLAYER_LIFE              = "player_life";
const char* const GEN_PLAYER_SHIELD            = "player_shield";
const char* const GEN_PLAYER_SHIELD_RESISTANCE = "player_shield_resistance";

// Enemy
const char* const GEN_ENEMY_NAME			  = "enemy_name";
const char* const GEN_ENEMY_TYPE			  = "enemy_type";
const char* const GEN_ENEMY_Y_PLANE			  = "enemy_y_plane";
const char* const GEN_ENEMY_POS_BOUNDARY	  = "enemy_pos_boundary";
const char* const GEN_ENEMY_NEG_BOUNDARY	  = "enemy_neg_boundary";
const char* const GEN_ENEMY_START_POSITION	  = "enemy_start_position";
const char* const GEN_ENEMY_START_ORIENTATION = "enemy_start_orientation";
const char* const GEN_ENEMY_MAX_SPEED		  = "enemy_maxSpeed";
const char* const GEN_ENEMY_ROTATION_SPEED	  = "enemy_rotation_speed";
const char* const GEN_ENEMY_GRAPHIC_STATIC	  = "enemy_graphic_static";
const char* const GEN_ENEMY_MODEL			  = "enemy_model";
const char* const GEN_ENEMY_PHYSX_SHAPE		  = "enemy_physx_shape";
const char* const GEN_ENEMY_PHYSX_RADIUS	  = "enemy_physx_radius";
const char* const GEN_ENEMY_PHYSX_HEIGHT	  = "enemy_physx_height";
const char* const GEN_WANDER_ENEMY_NUM		  = "wander_enemy_num";
const char* const GEN_ENEMY_MINIMAP_SCALE	  = "enemy_minimap_entity_scale";
const char* const GEN_ENEMY_MINIMAP_ENEMY	  = "enemy_minimap_entity_enemy";
const char* const GEN_ENEMY_MAXROLL			  = "enemy_maxRoll";
const char* const GEN_ENEMY_ROLL_SPEED		  = "enemy_rollSpeed";
const char* const GEN_ENEMY_ACCEL			  = "enemy_accel";
const char* const GEN_ENEMY_ROTATION_ACCEL	  = "enemy_rotation_accel";
const char* const GEN_ENEMY_MOV_TOLERANCE	  = "enemy_mov_tolerance";
const char* const GEN_ENEMY_LIFE			  = "enemy_life";
const char* const GEN_ENEMY_BEHAVIOR		  = "enemy_behavior";
const char* const GEN_ENEMY_SHIELD            = "enemy_shield"; // 100, 200, ...
const char* const GEN_ENEMY_SHIELD_RESISTANCE = "enemy_shield_resistance"; // [0..1]

// Shield
const char* const GEN_SHIELD_TYPE             = "shield_type";
const char* const GEN_SHIELD_TRIGGER_ENTITY   = "shield_trigger_entity";
const char* const GEN_SHIELD_TRIGGER_RADIUS   = "shield_trigger_radius";
const char* const GEN_SHIELD_TRIGGER_ISTRIGGER = "shield_trigger_istrigger";
const char* const GEN_SHIELD_GRAPHIC_MODEL     = "shield_model";

// weapons
const char* const GEN_LASER_RANGE  = "laser_range";
const char* const GEN_LASER_DAMAGE = "laser_damage";
const char* const GEN_MISSILE_LINEAR_TYPE              = "missile_linear_type";
const char* const GEN_MISSILE_LINEAR_TRIGGER_ENTITY    = "missile_linear_trigger_entity";
const char* const GEN_MISSILE_LINEAR_TRIGGER_RADIUS    = "missile_linear_trigger_radius";
const char* const GEN_MISSILE_LINEAR_TRIGGER_ISTRIGGER = "missile_linear_trigger_istrigger";
const char* const GEN_MISSILE_LINEAR_DAMAGE            = "missile_linear_damage";
const char* const GEN_MISSILE_LINEAR_RANGE             = "missile_linear_range";
const char* const GEN_MISSILE_LINEAR_SPEED             = "missile_linear_speed";
const char* const GEN_MISSILE_LINEAR_AMMO              = "missile_linear_ammo";
const char* const GEN_MISSILE_LINEAR_MAXCHARGER        = "missile_linear_maxCharger";

// Camera
const char* const GEN_CAMERA_NAME				= "camera_name";
const char* const GEN_CAMERA_NEAR				= "camera_near";
const char* const GEN_CAMERA_FAR				= "camera_far";
const char* const GEN_CAMERA_FOV				= "camera_fov";
const char* const GEN_CAMERA_POSITION			= "camera_position";
const char* const GEN_CAMERA_ORIENTATION		= "camera_orientation";
const char* const GEN_CAMERA_HEIGHT				= "camera_height";
const char* const GEN_CAMERA_DIST_CONSTANT      = "camera_dist_constant";
const char* const GEN_CAMERA_MIN_DISTANCE		= "camera_min_distance";
const char* const GEN_CAMERA_ACCEL_CONST		= "camera_accel_const";

// Texture Camera
const char* const GEN_PLANET_TEXTURE_CAMERA_ENTITYNAME	= "planet_texture_camera_entityname";
const char* const GEN_PLANET_TEXTURE_CAMERA_ENTITYTYPE	= "planet_texture_camera_entitytype";
const char* const GEN_PLANET_TEXTURE_CAMERA_NAME		= "planet_texture_camera_name";
const char* const GEN_PLANET_TEXTURE_CAMERA_NEAR		= "planet_texture_camera_near";
const char* const GEN_PLANET_TEXTURE_CAMERA_FAR			= "planet_texture_camera_far";
const char* const GEN_PLANET_TEXTURE_CAMERA_FOV			= "sys_texture_camera_fov";
const char* const GEN_PLANET_TEXTURE_CAMERA_ORTHO		= "planet_texture_camera_ortho";
const char* const GEN_PLANET_TEXTURE_CAMERA_ORTHO_HEIGHT= "planet_texture_camera_height";
const char* const GEN_PLANET_TEXTURE_CAMERA_ORTHO_WIDTH = "planet_texture_camera_width";
const char* const GEN_PLANET_TEXTURE_CAMERA_POSITION	= "planet_texture_camera_position";
const char* const GEN_PLANET_TEXTURE_CAMERA_LOOKAT		= "planet_texture_camera_lookat";

// SkyBox
const char* const GEN_SKYBOX_NAME = "skybox_name";
const char* const GEN_SKYBOX_TYPE = "skybox_type";

// Static Planet
const char* const GEN_PLANET_NAME = "planet_name";
const char* const GEN_PLANET_TYPE = "planet_type";
const char* const GEN_PLANET_STATIC = "planet_static";
const char* const GEN_PLANET_POSITION = "planet_position";
const char* const GEN_PLANET_ORIENTATION = "planet_orientation";
const char* const GEN_PLANET_SCALE_MULT = "planet_scale_mult";
const char* const GEN_PLANET_SCALE_ADD = "planet_scale_add";
const char* const GEN_PLANET_ROLL = "planet_roll";
const char* const GEN_PLANET_MODELS[] = { 
											"planet_model1",
											"planet_model2",
											"planet_model3",
											"planet_model4",
											"planet_model5",
											"planet_model6",
											"planet_model7",
											"planet_model8"
										}; //This is used for both types of planets

const char* const GEN_PLANET_INH_PROB[] = {
												"planet_inhabitation_prob1",
												"planet_inhabitation_prob2",
												"planet_inhabitation_prob3",
												"planet_inhabitation_prob4"
											};
 
// Planet Directional Light
const char* const GEN_PLDIRLIGHT_NAME = "pldirlight_name";
const char* const GEN_PLDIRLIGHT_TYPE = "pldirlight_type";
const char* const GEN_PLDIRLIGHT_POSITION = "pldirlight_position";
const char* const GEN_PLDIRLIGHT_ORIENTATION = "pldirlight_direction";
const char* const GEN_PLDIRLIGHT_DIFFUSE = "pldirlight_diffuse";
const char* const GEN_PLDIRLIGHT_SPECULAR = "pldirlight_specular";

// Planet Ambient Light
const char* const GEN_PLAMBLIGHT_NAME = "plamblight_name";
const char* const GEN_PLAMBLIGHT_TYPE = "plamblight_type";
const char* const GEN_PLAMBLIGHT_COLOUR = "plamblight_colour";

//	Planet Trigger
const char* const GEN_PLANET_TRIGGER_NAME = "planet_trigger_name";
const char* const GEN_PLANET_TRIGGER_TYPE = "planet_trigger_type";
const char* const GEN_PLANET_TRIGGER_POSITION = "planet_trigger_position";
const char* const GEN_PLANET_TRIGGER_ORIENTATION = "planet_trigger_orientation";
const char* const GEN_PLANET_TRIGGER_ENTITY = "planet_trigger_entity";
const char* const GEN_PLANET_TRIGGER_RADIUS = "planet_trigger_radius";
const char* const GEN_PLANET_TRIGGER_ISTRIGGER = "planet_trigger_istrigger";

// Scene Limit Trigger 1
const char* const GEN_PLANET_LIMIT_TRIGGER1_NAME = "planet_limit_trigger1_name";
const char* const GEN_PLANET_LIMIT_TRIGGER1_TYPE = "planet_limit_trigger1_type";
const char* const GEN_PLANET_LIMIT_TRIGGER1_POSITION = "planet_limit_trigger1_position";
const char* const GEN_PLANET_LIMIT_TRIGGER1_ORIENTATION = "planet_limit_trigger1_orientation";
const char* const GEN_PLANET_LIMIT_TRIGGER1_ENTITY = "planet_limit_trigger1_entity";
const char* const GEN_PLANET_LIMIT_TRIGGER1_DIMENSIONS = "planet_limit_trigger1_dimensions";
const char* const GEN_PLANET_LIMIT_TRIGGER1_ISTRIGGER = "planet_limit_trigger1_istrigger";

// Scene Limit Trigger 2
const char* const GEN_PLANET_LIMIT_TRIGGER2_NAME = "planet_limit_trigger2_name";
const char* const GEN_PLANET_LIMIT_TRIGGER2_TYPE = "planet_limit_trigger2_type";
const char* const GEN_PLANET_LIMIT_TRIGGER2_POSITION = "planet_limit_trigger2_position";
const char* const GEN_PLANET_LIMIT_TRIGGER2_ORIENTATION = "planet_limit_trigger2_orientation";
const char* const GEN_PLANET_LIMIT_TRIGGER2_ENTITY = "planet_limit_trigger2_entity";
const char* const GEN_PLANET_LIMIT_TRIGGER2_DIMENSIONS = "planet_limit_trigger2_dimensions";
const char* const GEN_PLANET_LIMIT_TRIGGER2_ISTRIGGER = "planet_limit_trigger2_istrigger";

// Scene Limit Trigger 3
const char* const GEN_PLANET_LIMIT_TRIGGER3_NAME = "planet_limit_trigger3_name";
const char* const GEN_PLANET_LIMIT_TRIGGER3_TYPE = "planet_limit_trigger3_type";
const char* const GEN_PLANET_LIMIT_TRIGGER3_POSITION = "planet_limit_trigger3_position";
const char* const GEN_PLANET_LIMIT_TRIGGER3_ORIENTATION = "planet_limit_trigger3_orientation";
const char* const GEN_PLANET_LIMIT_TRIGGER3_ENTITY = "planet_limit_trigger3_entity";
const char* const GEN_PLANET_LIMIT_TRIGGER3_DIMENSIONS = "planet_limit_trigger3_dimensions";
const char* const GEN_PLANET_LIMIT_TRIGGER3_ISTRIGGER = "planet_limit_trigger3_istrigger";

// Scene Limit Trigger 4
const char* const GEN_PLANET_LIMIT_TRIGGER4_NAME = "planet_limit_trigger4_name";
const char* const GEN_PLANET_LIMIT_TRIGGER4_TYPE = "planet_limit_trigger4_type";
const char* const GEN_PLANET_LIMIT_TRIGGER4_POSITION = "planet_limit_trigger4_position";
const char* const GEN_PLANET_LIMIT_TRIGGER4_ORIENTATION = "planet_limit_trigger4_orientation";
const char* const GEN_PLANET_LIMIT_TRIGGER4_ENTITY = "planet_limit_trigger4_entity";
const char* const GEN_PLANET_LIMIT_TRIGGER4_DIMENSIONS = "planet_limit_trigger4_dimensions";
const char* const GEN_PLANET_LIMIT_TRIGGER4_ISTRIGGER = "planet_limit_trigger4_istrigger";

// Scene End Trigger 1
const char* const GEN_PLANET_END_TRIGGER1_NAME = "planet_end_trigger1_name";
const char* const GEN_PLANET_END_TRIGGER1_TYPE = "planet_end_trigger1_type";
const char* const GEN_PLANET_END_TRIGGER1_POSITION = "planet_end_trigger1_position";
const char* const GEN_PLANET_END_TRIGGER1_ORIENTATION = "planet_end_trigger1_orientation";
const char* const GEN_PLANET_END_TRIGGER1_ENTITY = "planet_end_trigger1_entity";
const char* const GEN_PLANET_END_TRIGGER1_DIMENSIONS = "planet_end_trigger1_dimensions";
const char* const GEN_PLANET_END_TRIGGER1_ISTRIGGER = "planet_end_trigger1_istrigger";

// Scene End Trigger 2
const char* const GEN_PLANET_END_TRIGGER2_NAME = "planet_end_trigger2_name";
const char* const GEN_PLANET_END_TRIGGER2_TYPE = "planet_end_trigger2_type";
const char* const GEN_PLANET_END_TRIGGER2_POSITION = "planet_end_trigger2_position";
const char* const GEN_PLANET_END_TRIGGER2_ORIENTATION = "planet_end_trigger2_orientation";
const char* const GEN_PLANET_END_TRIGGER2_ENTITY = "planet_end_trigger2_entity";
const char* const GEN_PLANET_END_TRIGGER2_DIMENSIONS = "planet_end_trigger2_dimensions";
const char* const GEN_PLANET_END_TRIGGER2_ISTRIGGER = "planet_end_trigger2_istrigger";

// Scene End Trigger 3
const char* const GEN_PLANET_END_TRIGGER3_NAME = "planet_end_trigger3_name";
const char* const GEN_PLANET_END_TRIGGER3_TYPE = "planet_end_trigger3_type";
const char* const GEN_PLANET_END_TRIGGER3_POSITION = "planet_end_trigger3_position";
const char* const GEN_PLANET_END_TRIGGER3_ORIENTATION = "planet_end_trigger3_orientation";
const char* const GEN_PLANET_END_TRIGGER3_ENTITY = "planet_end_trigger3_entity";
const char* const GEN_PLANET_END_TRIGGER3_DIMENSIONS = "planet_end_trigger3_dimensions";
const char* const GEN_PLANET_END_TRIGGER3_ISTRIGGER = "planet_end_trigger3_istrigger";

// Scene End Trigger 4
const char* const GEN_PLANET_END_TRIGGER4_NAME = "planet_end_trigger4_name";
const char* const GEN_PLANET_END_TRIGGER4_TYPE = "planet_end_trigger4_type";
const char* const GEN_PLANET_END_TRIGGER4_POSITION = "planet_end_trigger4_position";
const char* const GEN_PLANET_END_TRIGGER4_ORIENTATION = "planet_end_trigger4_orientation";
const char* const GEN_PLANET_END_TRIGGER4_ENTITY = "planet_end_trigger4_entity";
const char* const GEN_PLANET_END_TRIGGER4_DIMENSIONS = "planet_end_trigger4_dimensions";
const char* const GEN_PLANET_END_TRIGGER4_ISTRIGGER = "planet_end_trigger4_istrigger";

namespace Common
{
	namespace Configuration 
	{

		template<typename T>
		static T to(const std::string &str)
		{
			std::stringstream ss(str);
			T ret;
			ss >> ret;
			return (ret);
		}

		bool setDefaultFile(const char* path);

		std::string getDefaultValue(const std::string &name);

		template<typename Return>
		inline Return defaultValue(const std::string &name)
		{
			return (to<Return> (getDefaultValue(name)));
		}

		void removeConfiguration(const char* path = 0);

		class CConfigure
		{
		public:
			static CConfigure& getConfiguration(const char* path = 0);
			static CConfigure& getConfiguration(const std::string &path);

			~CConfigure(){}

			bool checkProperty(const std::string &name) const;
			std::string getValue(const std::string& name) const;

			template <typename Return>
			inline Return value (const std::string &name)const
			{
				return (to<Return> (this->getValue(name)));
			}
		protected:
		private:
			CConfigure(const std::string& path);
			CConfigure(const CConfigure& copy){}
			CConfigure& operator=(CConfigure copy){}

			const std::string m_path;

			typedef std::map<std::string, std::string> TConfMap;
			TConfMap m_configurationValues;
		};
	}
}

#endif