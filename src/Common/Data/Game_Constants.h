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

#include <array>
#include <map>
#include <string>
#include <tuple>


namespace Common
{
	namespace Data
	{
		namespace Game
		{
			/// TODO -- Internationalization -- /// Begin{
			//Resources
			const char* const GAME_FUEL	= ">>Fuel:";
			const char* const GAME_TIME	= ">>Time:";
			const char* const GAME_ORE	= ">>Ore:";

			//Equipment
			const char* const GAME_ITEM					= ">>Item:";
			const char* const GAME_PRIMARY_WEAPON		= ">>Primary Weapon:";
			const char* const GAME_SECONDARY_WEAPON		= ">>Secondary Weapon:";
			const char* const GAME_ENGINE				= ">>Engine:";
			const char* const GAME_SENSORS				= ">>Sensors:";

			//Special equipment
			const char* const GAME_REGENERATOR			= ">>Shield Energy Regenerator";
			const char* const GAME_STEALTH				= ">>Stealth Module";

			//State
			const char* const GAME_HULL				= ">>Hull State:";
			const char* const GAME_SHIELD			= ">>Shield:";
			const char* const GAME_ENERGY			= ">>Energy:";

			//Crew
			const char* const GAME_MILITARY			= ">>Military:";
			const char* const GAME_ENGINEERS		= ">>Engineers:";
			const char* const GAME_SCIENTIFICS		= ">>Scientifics:";

			//HUD
			const char* const GAME_HUD_EQUIPMENT		= "---- Equipment ----";
			const char* const GAME_HUD_OTHERS			= "---- Others ----";
			const char* const GAME_HUD_SHIP_STATE		= "---- Ship State ----";
			const char* const GAME_HUD_CREW				= "---- Crew Members ----";
			const char* const GAME_HUD_RESOURCES		= "---- Resources ----";
			const char* const GAME_HUD_CARGO			= "---- Cargo ----";
			const char* const GAME_HUD_INFO				= "---- Info ----";
			const char* const GAME_HUD_NOTES			= ">>Notes:";
			const char* const GAME_HUD_LEFT_WINDOW		= "LeftWindow";
			const char* const GAME_HUD_RIGHT_WINDOW		= "RightWindow";

			//Shop
			const char* const GAME_EVENT_SHOP		= "---- Shop ----";
			const char* const GAME_EVENT_COST		= "-- Cost --";

			//String keys
			const char* const GAME_HUD_DESCRIPTION		= "description";

			//Mission description
			const char* const GAME_MISSION_NAME			= "Main mission";
			const char* const GAME_MISSION_TEXT			= "Planets visited: ";

			//Shop
			const char* const GAME_SHOP_REPAIR			= ">>Repair Hull";

			// Galaxy System names
			const char* const GAME_GALAXY_SYSTEM_NAME[] = {
												"System0","System1","System2","System3",
												"System4","System5","System6","System7",
												"System8","System9","System10","System11",
												"System12","System13","System14","System15",
												"System16","System17","System18","System19",
												"System20","System21","System22","System23",
												"System24","System25","System26","System27",
												"System28","System29","System30","System31",
												"System32","System33","System34","System35",
												"System36","System37","System38","System39"
											};

			// Planet descriptions
			const char* const GAME_MOVPLANET_DESCTYPE1[] = {
												"Desert planet 1",
												"Desert planet 2",
												"Desert planet 3",
												"Desert planet 4",
												"Desert planet 5",
												"Desert planet 6",
												"Desert planet 7",
												"Desert planet 8",
												"Desert planet 9",
												"Desert planet 10"
											};
			const char* const GAME_MOVPLANET_DESCTYPE2[] = {
													"Lava planet 1",
													"Lava planet 2",
													"Lava planet 3",
													"Lava planet 4",
													"Lava planet 5",
													"Lava planet 6",
													"Lava planet 7",
													"Lava planet 8",
													"Lava planet 9",
													"Lava planet 10"
												};
			const char* const GAME_MOVPLANET_DESCTYPE3[] = {
													"Red planet 1",
													"Red planet 2",
													"Red planet 3",
													"Red planet 4",
													"Red planet 5",
													"Red planet 6",
													"Red planet 7",
													"Red planet 8",
													"Red planet 9",
													"Red planet 10"
												};
			const char* const GAME_MOVPLANET_DESCTYPE4[] = {
													"Earth-like planet 1",
													"Earth-like planet 2",
													"Earth-like planet 3",
													"Earth-like planet 4",
													"Earth-like planet 5",
													"Earth-like planet 6",
													"Earth-like planet 7",
													"Earth-like planet 8",
													"Earth-like planet 9",
													"Earth-like planet 10"
												};
			const char* const GAME_MOVPLANET_DESCTYPE5[] = {
													"Ocean planet 1",
													"Ocean planet 2",
													"Ocean planet 3",
													"Ocean planet 4",
													"Ocean planet 5",
													"Ocean planet 6",
													"Ocean planet 7",
													"Ocean planet 8",
													"Ocean planet 9",
													"Ocean planet 10"
												};
			const char* const GAME_MOVPLANET_DESCTYPE6[] = {
													"Gas planet 1",
													"Gas planet 2",
													"Gas planet 3",
													"Gas planet 4",
													"Gas planet 5",
													"Gas planet 6",
													"Gas planet 7",
													"Gas planet 8",
													"Gas planet 9",
													"Gas planet 10"
												};
			const char* const GAME_MOVPLANET_DESCTYPE7[] = {
													"Ice planet 1",
													"Ice planet 2",
													"Ice planet 3",
													"Ice planet 4",
													"Ice planet 5",
													"Ice planet 6",
													"Ice planet 7",
													"Ice planet 8",
													"Ice planet 9",
													"Ice planet 10"
												};
			const char* const GAME_MOVPLANET_DESCTYPE8[] = {
													"Barren planet 1",
													"Barren planet 2",
													"Barren planet 3",
													"Barren planet 4",
													"Barren planet 5",
													"Barren planet 6",
													"Barren planet 7",
													"Barren planet 8",
													"Barren planet 9",
													"Barren planet 10"
												};

			const std::array<const char* const*,8> GAME_MOVPLANET_DESC = {
													GAME_MOVPLANET_DESCTYPE1,
													GAME_MOVPLANET_DESCTYPE2,
													GAME_MOVPLANET_DESCTYPE3,
													GAME_MOVPLANET_DESCTYPE4,
													GAME_MOVPLANET_DESCTYPE5,
													GAME_MOVPLANET_DESCTYPE6,
													GAME_MOVPLANET_DESCTYPE7,
													GAME_MOVPLANET_DESCTYPE8
											};

			// Planet presentations
			const char* const GAME_PLANET_EVENT_INTRO[] = {"Unoccupied Desert planet.","Unoccupied Lava planet.","Unoccupied Red planet.","Unoccupied Earth-like planet.",
														"Unoccupied Ocean planet.","Unoccupied Gas planet.","Unoccupied Ice planet.","Unoccupied Barren planet.",
														"Inhabited Desert planet.","Inhabited Lava planet.","Inhabited Red planet.","Inhabited Earth-like planet.",
														"Inhabited Ocean planet.","Inhabited Gas planet.","Inhabited Ice planet.","Inhabited Barren planet."
													};

			// --Planets Names--

			//Planets by system
			const char* const GAME_PLANETS_SYSTEM0[] = {
				"Planet_0__0_","Planet_0__1_","Planet_0__2_","Planet_0__3_","Planet_0__4_","Planet_0__5_","Planet_0__6_","Planet_0__7_","Planet_0__8_","Planet_0__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM1[] = {
				"Planet_1__0_","Planet_1__1_","Planet_1__2_","Planet_1__3_","Planet_1__4_","Planet_1__5_","Planet_1__6_","Planet_1__7_","Planet_1__8_","Planet_1__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM2[] = {
				"Planet_2__0_","Planet_2__1_","Planet_2__2_","Planet_2__3_","Planet_2__4_","Planet_2__5_","Planet_2__6_","Planet_2__7_","Planet_2__8_","Planet_2__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM3[] = {
				"Planet_3__0_","Planet_3__1_","Planet_3__2_","Planet_3__3_","Planet_3__4_","Planet_3__5_","Planet_3__6_","Planet_3__7_","Planet_3__8_","Planet_3__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM4[] = {
				"Planet_4__0_","Planet_4__1_","Planet_4__2_","Planet_4__3_","Planet_4__4_","Planet_4__5_","Planet_4__6_","Planet_4__7_","Planet_4__8_","Planet_4__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM5[] = {
				"Planet_5__0_","Planet_5__1_","Planet_5__2_","Planet_5__3_","Planet_5__4_","Planet_5__5_","Planet_5__6_","Planet_5__7_","Planet_5__8_","Planet_5__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM6[] = {
				"Planet_6__0_","Planet_6__1_","Planet_6__2_","Planet_6__3_","Planet_6__4_","Planet_6__5_","Planet_6__6_","Planet_6__7_","Planet_6__8_","Planet_6__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM7[] = {
				"Planet_7__0_","Planet_7__1_","Planet_7__2_","Planet_7__3_","Planet_7__4_","Planet_7__5_","Planet_7__6_","Planet_7__7_","Planet_7__8_","Planet_7__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM8[] = {
				"Planet_8__0_","Planet_8__1_","Planet_8__2_","Planet_8__3_","Planet_8__4_","Planet_8__5_","Planet_8__6_","Planet_8__7_","Planet_8__8_","Planet_8__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM9[] = {
				"Planet_9__0_","Planet_9__1_","Planet_9__2_","Planet_9__3_","Planet_9__4_","Planet_9__5_","Planet_9__6_","Planet_9__7_","Planet_9__8_","Planet_9__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM10[] = {
				"Planet_10__0_","Planet_10__1_","Planet_10__2_","Planet_10__3_","Planet_10__4_","Planet_10__5_","Planet_10__6_","Planet_10__7_","Planet_10__8_","Planet_10__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM11[] = {
				"Planet_11__0_","Planet_11__1_","Planet_11__2_","Planet_11__3_","Planet_11__4_","Planet_11__5_","Planet_11__6_","Planet_11__7_","Planet_11__8_","Planet_11__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM12[] = {
				"Planet_12__0_","Planet_12__1_","Planet_12__2_","Planet_12__3_","Planet_12__4_","Planet_12__5_","Planet_12__6_","Planet_12__7_","Planet_12__8_","Planet_12__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM13[] = {
				"Planet_13__0_","Planet_13__1_","Planet_13__2_","Planet_13__3_","Planet_13__4_","Planet_13__5_","Planet_13__6_","Planet_13__7_","Planet_13__8_","Planet_13__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM14[] = {
				"Planet_14__0_","Planet_14__1_","Planet_14__2_","Planet_14__3_","Planet_14__4_","Planet_14__5_","Planet_14__6_","Planet_14__7_","Planet_14__8_","Planet_14__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM15[] = {
				"Planet_15__0_","Planet_15__1_","Planet_15__2_","Planet_15__3_","Planet_15__4_","Planet_15__5_","Planet_15__6_","Planet_15__7_","Planet_15__8_","Planet_15__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM16[] = {
				"Planet_16__0_","Planet_16__1_","Planet_16__2_","Planet_16__3_","Planet_16__4_","Planet_16__5_","Planet_16__6_","Planet_16__7_","Planet_16__8_","Planet_16__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM17[] = {
				"Planet_17__0_","Planet_17__1_","Planet_17__2_","Planet_17__3_","Planet_17__4_","Planet_17__5_","Planet_17__6_","Planet_17__7_","Planet_17__8_","Planet_17__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM18[] = {
				"Planet_18__0_","Planet_18__1_","Planet_18__2_","Planet_18__3_","Planet_18__4_","Planet_18__5_","Planet_18__6_","Planet_18__7_","Planet_18__8_","Planet_18__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM19[] = {
				"Planet_19__0_","Planet_19__1_","Planet_19__2_","Planet_19__3_","Planet_19__4_","Planet_19__5_","Planet_19__6_","Planet_19__7_","Planet_19__8_","Planet_19__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM20[] = {
				"Planet_20__0_","Planet_20__1_","Planet_20__2_","Planet_20__3_","Planet_20__4_","Planet_20__5_","Planet_20__6_","Planet_20__7_","Planet_20__8_","Planet_20__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM21[] = {
				"Planet_21__0_","Planet_21__1_","Planet_21__2_","Planet_21__3_","Planet_21__4_","Planet_21__5_","Planet_21__6_","Planet_21__7_","Planet_21__8_","Planet_21__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM22[] = {
				"Planet_22__0_","Planet_22__1_","Planet_22__2_","Planet_22__3_","Planet_22__4_","Planet_22__5_","Planet_22__6_","Planet_22__7_","Planet_22__8_","Planet_22__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM23[] = {
				"Planet_23__0_","Planet_23__1_","Planet_23__2_","Planet_23__3_","Planet_23__4_","Planet_23__5_","Planet_23__6_","Planet_23__7_","Planet_23__8_","Planet_23__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM24[] = {
				"Planet_24__0_","Planet_24__1_","Planet_24__2_","Planet_24__3_","Planet_24__4_","Planet_24__5_","Planet_24__6_","Planet_24__7_","Planet_24__8_","Planet_24__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM25[] = {
				"Planet_25__0_","Planet_25__1_","Planet_25__2_","Planet_25__3_","Planet_25__4_","Planet_25__5_","Planet_25__6_","Planet_25__7_","Planet_25__8_","Planet_25__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM26[] = {
				"Planet_26__0_","Planet_26__1_","Planet_26__2_","Planet_26__3_","Planet_26__4_","Planet_26__5_","Planet_26__6_","Planet_26__7_","Planet_26__8_","Planet_26__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM27[] = {
				"Planet_27__0_","Planet_27__1_","Planet_27__2_","Planet_27__3_","Planet_27__4_","Planet_27__5_","Planet_27__6_","Planet_27__7_","Planet_27__8_","Planet_27__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM28[] = {
				"Planet_28__0_","Planet_28__1_","Planet_28__2_","Planet_28__3_","Planet_28__4_","Planet_28__5_","Planet_28__6_","Planet_28__7_","Planet_28__8_","Planet_28__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM29[] = {
				"Planet_29__0_","Planet_29__1_","Planet_29__2_","Planet_29__3_","Planet_29__4_","Planet_29__5_","Planet_29__6_","Planet_29__7_","Planet_29__8_","Planet_29__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM30[] = {
				"Planet_30__0_","Planet_30__1_","Planet_30__2_","Planet_30__3_","Planet_30__4_","Planet_30__5_","Planet_30__6_","Planet_30__7_","Planet_30__8_","Planet_30__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM31[] = {
				"Planet_31__0_","Planet_31__1_","Planet_31__2_","Planet_31__3_","Planet_31__4_","Planet_31__5_","Planet_31__6_","Planet_31__7_","Planet_31__8_","Planet_31__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM32[] = {
				"Planet_32__0_","Planet_32__1_","Planet_32__2_","Planet_32__3_","Planet_32__4_","Planet_32__5_","Planet_32__6_","Planet_32__7_","Planet_32__8_","Planet_32__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM33[] = {
				"Planet_33__0_","Planet_33__1_","Planet_33__2_","Planet_33__3_","Planet_33__4_","Planet_33__5_","Planet_33__6_","Planet_33__7_","Planet_33__8_","Planet_33__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM34[] = {
				"Planet_34__0_","Planet_34__1_","Planet_34__2_","Planet_34__3_","Planet_34__4_","Planet_34__5_","Planet_34__6_","Planet_34__7_","Planet_34__8_","Planet_34__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM35[] = {
				"Planet_35__0_","Planet_35__1_","Planet_35__2_","Planet_35__3_","Planet_35__4_","Planet_35__5_","Planet_35__6_","Planet_35__7_","Planet_35__8_","Planet_35__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM36[] = {
				"Planet_36__0_","Planet_36__1_","Planet_36__2_","Planet_36__3_","Planet_36__4_","Planet_36__5_","Planet_36__6_","Planet_36__7_","Planet_36__8_","Planet_36__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM37[] = {
				"Planet_37__0_","Planet_37__1_","Planet_37__2_","Planet_37__3_","Planet_37__4_","Planet_37__5_","Planet_37__6_","Planet_37__7_","Planet_37__8_","Planet_37__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM38[] = {
				"Planet_38__0_","Planet_38__1_","Planet_38__2_","Planet_38__3_","Planet_38__4_","Planet_38__5_","Planet_38__6_","Planet_38__7_","Planet_38__8_","Planet_38__9_" 
			};

			const char* const GAME_PLANETS_SYSTEM39[] = {
				"Planet_39__0_","Planet_39__1_","Planet_39__2_","Planet_39__3_","Planet_39__4_","Planet_39__5_","Planet_39__6_","Planet_39__7_","Planet_39__8_","Planet_39__9_" 
			};

			const std::array<const char* const*,40> GAME_PLANETS_NAMES = {
				GAME_PLANETS_SYSTEM0, GAME_PLANETS_SYSTEM1, GAME_PLANETS_SYSTEM2, GAME_PLANETS_SYSTEM3, GAME_PLANETS_SYSTEM4, GAME_PLANETS_SYSTEM5,
				GAME_PLANETS_SYSTEM6, GAME_PLANETS_SYSTEM7, GAME_PLANETS_SYSTEM8, GAME_PLANETS_SYSTEM9, GAME_PLANETS_SYSTEM10, GAME_PLANETS_SYSTEM11,
				GAME_PLANETS_SYSTEM12, GAME_PLANETS_SYSTEM13, GAME_PLANETS_SYSTEM14, GAME_PLANETS_SYSTEM15, GAME_PLANETS_SYSTEM16, GAME_PLANETS_SYSTEM17,
				GAME_PLANETS_SYSTEM18, GAME_PLANETS_SYSTEM19, GAME_PLANETS_SYSTEM20, GAME_PLANETS_SYSTEM21, GAME_PLANETS_SYSTEM22, GAME_PLANETS_SYSTEM23,
				GAME_PLANETS_SYSTEM24, GAME_PLANETS_SYSTEM25, GAME_PLANETS_SYSTEM26, GAME_PLANETS_SYSTEM27, GAME_PLANETS_SYSTEM28, GAME_PLANETS_SYSTEM29,
				GAME_PLANETS_SYSTEM30, GAME_PLANETS_SYSTEM31, GAME_PLANETS_SYSTEM32, GAME_PLANETS_SYSTEM33, GAME_PLANETS_SYSTEM34, GAME_PLANETS_SYSTEM35,
				GAME_PLANETS_SYSTEM36, GAME_PLANETS_SYSTEM37, GAME_PLANETS_SYSTEM38, GAME_PLANETS_SYSTEM39
			};

			// --Object definitions--

			// Primary Weapons
			const char* const GAME_HEAVY_LASER[2] = {
				"Heavy Laser", "Primary Weapon\nStandar heavy laser gun,\nused as primary weapon for nearly any kind of spaceship."
			}; 

			const char* const GAME_TESLA_CANNON[2] = {
				"Tesla Cannon","Primary Weapon\nThis weapon fires a continuous lightning ray.\nThough it`s technology it`s\nquite old this weapon is\nstill in use today."
			};

			const char* const GAME_ENERGY_BEAM[2] = {
				"Energy Beam", "Primary Weapon\nA short ranged but powerful and quick weapon."
			};

			const int TOTAL_PRIMARY_WEAPONS = 3;

			const std::pair<std::string,std::pair<int, int>> PRIMARY_WEAPON_COST[] =
			{
				std::make_pair (GAME_HEAVY_LASER[0],std::make_pair (50, 10)), // base cost, variance/10
				std::make_pair (GAME_TESLA_CANNON[0],std::make_pair (80, 20)),
				std::make_pair (GAME_ENERGY_BEAM[0],std::make_pair (200, 35))
			};

			const std::array<const char* const*,TOTAL_PRIMARY_WEAPONS> GAME_PRIMARY_WEAPONS_LIST = {
				GAME_HEAVY_LASER,
				GAME_TESLA_CANNON,
				GAME_ENERGY_BEAM
			};

			// Secondary Weapons
			const char* const GAME_MISSILE_LAUNCHER[2] = {
				"Missile Launcher", "Secondary Weapon\nPowerfull but slightly slow.\nThis missile launcher mounted on\na turret takes a lot of energy to use."
			};

			const char* const GAME_STATIC_MINES[2] = {
				"Static Mines", "Secondary Weapon\nMines that are left behind the ship\nand attach to any spaceship that gets\nclose enough."
			};

			const char* const GAME_EMP[2] = {
				"EMP Bombs", "Secondary Weapon\nThese bombs damage electronic devices\nand sensors in a wide area."
			};

			const int TOTAL_SECONDARY_WEAPONS = 3;

			const std::pair<std::string,std::pair<int, int>> SECONDARY_WEAPON_COST[] =
			{
				std::make_pair (GAME_MISSILE_LAUNCHER[0],std::make_pair (40, 15)), // base cost, variance/10
				std::make_pair (GAME_STATIC_MINES[0],std::make_pair (60, 20)),
				std::make_pair (GAME_EMP[0],std::make_pair (200, 10))
			};

			const std::array<const char* const*,TOTAL_SECONDARY_WEAPONS> GAME_SECONDARY_WEAPONS_LIST = {
				GAME_MISSILE_LAUNCHER,
				GAME_STATIC_MINES,
				GAME_EMP
			};

			// Engines
			const char* const GAME_PULSE_ENGINE[2] = {
				"Ion Pulse Engine", "Engine\nAs the most basic faster than light\nengine it provides a short range quantic jump\nand medium slower than light speed."
			};

			const char* const GAME_TEST_ENGINE[2] = {
				"Test Engine", "Engine\nTest."
			};

			const char* const GAME_PLASMA_ENGINE[2] = {
				"Plasma Stream Engine", "Engine\nA top notch engine that\nis both efficient and powerful."
			};

			const std::pair<std::string,std::pair<int, int>> ENGINE_DATA_MAP[] =
			{
				std::make_pair (GAME_PULSE_ENGINE[0],std::make_pair (35, 140)), // distance, cost
				std::make_pair (GAME_TEST_ENGINE[0],std::make_pair (50, 200)),
				std::make_pair (GAME_PLASMA_ENGINE[0],std::make_pair (85, 105))
			};

			const std::tuple<std::string, float, float, float> ENGINE_VEL_DATA[] = 
			{
				std::make_tuple(GAME_PULSE_ENGINE[0], 1.0,1.0,1.0),//max speed, acceleration, rotation speed
				std::make_tuple(GAME_TEST_ENGINE[0], 1.0,1.5,1.25),
				std::make_tuple(GAME_PLASMA_ENGINE[0], 2.0,1.5,1.5)
			};

			const int TOTAL_ENGINES = 3;

			const std::pair<std::string,std::pair<int, int>> ENGINE_COST[] =
			{
				std::make_pair (GAME_PULSE_ENGINE[0],std::make_pair (30, 6)), // base cost, variance/10
				std::make_pair (GAME_TEST_ENGINE[0],std::make_pair (800, 200)),
				std::make_pair (GAME_PLASMA_ENGINE[0],std::make_pair (300, 50))
			};

			const std::array<const char* const*,TOTAL_ENGINES> GAME_ENGINES_LIST = {
				GAME_PULSE_ENGINE,
				GAME_TEST_ENGINE,
				GAME_PLASMA_ENGINE
			};

			// Sensors
			const char* const GAME_RADAR[2] = {
				"Radar", "Lvl 1 Sensor\nThis common sensor measures\nelectromagnetic echoes.\nIt detects some enemies and\nit`s also able to get some information\nfrom planets."
			};

			const int TOTAL_SENSORS = 1;

			const std::array<const char* const*,TOTAL_SENSORS> GAME_SENSORS_LIST = {
				GAME_RADAR
			};

			// Special equipment
			const char* const GAME_ITEM_REGENERATOR[3] = {
				GAME_REGENERATOR, "Shield Energy Regenerator", "Special Device\nA quantic reverse\nenthropy device\nthat slowly regenerates\nthe shield energy over time."
			};

			const char* const GAME_ITEM_STEALTH[3] = {
				GAME_STEALTH, "Stealth Module", "Special Device\nHides the ship from enemies sensors.\nIt has a high energy cost."
			};

			const int TOTAL_SPECIAL = 2;

			const std::pair<std::string,std::pair<int, int>> SPECIAL_EQUIPMENT_COST[] =
			{
				std::make_pair (GAME_ITEM_REGENERATOR[0],std::make_pair (200, 20)), // base cost, variance/10
				std::make_pair (GAME_ITEM_STEALTH[0],std::make_pair (300, 10))
			};

			const std::array<const char* const*,TOTAL_SPECIAL> GAME_SPECIAL_EQUIPMENT_LIST = {
				GAME_ITEM_REGENERATOR,
				GAME_ITEM_STEALTH
			};

			// Items
			const char* const GAME_REWARD[2] = {
				"Reward Item", "Item\nTest item to be given as a reward\nfor completing missions."
			};

			const char* const GAME_USELESS[2] = {
				"Useless Item", "Item\nUseless item found in a planet."
			};

			const int TOTAL_ITEMS = 2;

			const std::pair<std::string,std::pair<int, int>> ITEM_COST[] =
			{
				std::make_pair (GAME_REWARD[0],std::make_pair (-1, -1)), // base cost, variance/10
				std::make_pair (GAME_USELESS[0],std::make_pair (-1, -1))
			};

			const std::array<const char* const*,TOTAL_ITEMS> GAME_ITEM_LIST = {
				GAME_REWARD,
				GAME_USELESS
			};

			const int REPAIR_HULL_COST = 100;

			/// TODO -- shop costs -- ///

			/// TODO -- Internationalization -- /// }End
		}

	}

}
