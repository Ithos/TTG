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
			const char* const GAME_ORE	= ">>Scrap:";

			//Equipment
			const char* const GAME_ITEM					= ">>Item:";
			const char* const GAME_PRIMARY_WEAPON		= ">>Primary Weapon:";
			const char* const GAME_SECONDARY_WEAPON		= ">>Secondary Weapon:";
			const char* const GAME_ENGINE				= ">>Engine:";
			const char* const GAME_SENSORS				= ">>Sensors:";

			//Special equipment
			const char* const GAME_REGENERATOR			= ">>Shield Energy Regenerator";
			const char* const GAME_STEALTH				= ">>Stealth Module";
			const char* const GAME_SHIELD_ENHANCER		= ">>Shield Regeneration Enhancer";
			const char* const GAME_ARMOR_LIGHT			= ">>Light Hull Reinforcement";
			const char* const GAME_ARMOR_MEDIUM			= ">>Medium Hull Reinforcement";
			const char* const GAME_ARMOR_HEAVY			= ">>Heavy Hull Reinforcement";
			const char* const GAME_SMALL_PROCESSOR		= ">>Small Mineral Processor";
			const char* const GAME_MEDIUM_PROCESSOR		= ">>Medium Mineral Processor";
			const char* const GAME_BIG_PROCESSOR		= ">>Big Mineral Processor";
			const char* const GAME_SMALL_COMPRESSOR		= ">>Small Fuel Compressor";
			const char* const GAME_MEDIUM_COMPRESSOR	= ">>Medium Fuel Compressor";
			const char* const GAME_BIG_COMPRESSOR		= ">>Big Fuel Compressor";

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

			//Planet description aditional information
			const char* const GAME_PLANET_INHABITED			= "Inhabited";
			const char* const GAME_PLANET_UNNOCUPIED		= "Unnocupied";

			const char* const GAME_PLANET_NO_RISK			= ">>no risk<<";
			const char* const GAME_PLANET_LOW_RISK			= ">>low risk<<";
			const char* const GAME_PLANET_MEDIUM_RISK		= ">>medium risk<<";
			const char* const GAME_PLANET_HIGH_RISK			= ">>high risk<<";
			const char* const GAME_PLANET_VERY_HIGH_RISK	= ">>very high risk<<";

			const char* const GAME_PLANET_SIGNAL			= "-->signal source detected<--";

			// HUD description text

			const char* const GAME_HUD_EQUIPMENT_DESCRIPTION		= "Equipment of the ship.\nYou can change them by selecting\nan item from the cargo\nand clicking on Equip.";
			const char* const GAME_HUD_OTHERS_DESCRIPTION			= "Special equipment.\nItems that provide special habilities.";
			const char* const GAME_HUD_SHIP_STATE_DESCRIPTION		= "Current state of the ship.";
			const char* const GAME_HUD_HULL_STATE_DESCRIPTION		= "Current hull resistance state and total hull resistance.";
			const char* const GAME_HUD_SHIELD_STATE_DESCRIPTION		= "Total shield resistance.";
			const char* const GAME_HUD_ENERGY_STATE_DESCRIPTION		= "Total energy available.";
			const char* const GAME_HUD_CREW_DESCRIPTION				= "Other people working in the ship.";
			const char* const GAME_HUD_MILITARY_DESCRIPTION			= "Weapon experts with military experience.\nIncrease the damage of the weapons\nand the velocity of\nenergy regeneration.";
			const char* const GAME_HUD_ENGINEERS_DESCRIPTION		= "Mechanichs able to repair the ship.\nIncrease the resistance of the shield\nand the engine transport distance.";
			const char* const GAME_HUD_SCIENTIFICS_DESCRIPTION		= "Investigators exploring the universe.\nIncrease the energy for the weapons\nand the level of the sensors.";
			const char* const GAME_HUD_RESOURCES_DESCRIPTION		= "The amount of resources in the ship.";
			const char* const GAME_HUD_FUEL_DESCRIPTION				= "The amount of fuel in the ship.\nIt's consumed by the engine when the ship\ntleports between solar systems.";
			const char* const GAME_HUD_SCRAP_DESCRIPTION			= "Valuable minerals and other objects\nused as currency by explorers and outcasts.";
			const char* const GAME_HUD_CARGO_DESCRIPTION			= "Items carried in the cargo compartment.\nSome of them may be installed\non the ship.";
			const char* const GAME_HUD_NOTES_DESCRIPTION			= "Additional information.";
			const char* const GAME_HUD_INFO_DESCRIPTION				= "Information provided by the ship's AI.";


			// Galaxy System names
			const char* const GAME_GALAXY_SYSTEM_NAME[] = {
												"Sirrah","Cih","Canopus","Navi",
												"Kurhah","Garnet","Deneb_Kaitos","Mira",
												"Tso_Hea","Gienah","Algorab","Minkar",
												"Acrux","Gacrux","CanopusII","Edasich",
												"Dziban","Acamar","Castor","Al_Na'ir",
												"Al_Dhanab","Kornephoros","Ukdah","Cor_Koraelis",
												"Zozma","Ras_Elased","Tsze_Tseang","Vega",
												"Sulafat","Yed","Zed","Hiades",
												"Betelgeuse","Rigel","Saiph","Fomalhaut",
												"Sirius","Arcturus","Aldebaran","Tseen_Ke"
											};

			// Planet descriptions
			const char* const GAME_MOVPLANET_DESCTYPE1[] = {
												"Sensors indicate little to no humidity in this planet.",
												"This planet seems to be formed mainly by silicium.",
												"This planet reflects lots of radiation coming from the star.",
												"Lectures suggest some spaceship activity around the planet.",
												"A preliminary examination shows that wild life in this planet is scarce.",
												"The atmosphere of the planet shows some signs of terraformation.",												
												"Little to no vegetation has been detected on the planet.",
												"Some parts of the planet show the signs of the use of planetary weapons in the past."
											};
			const char* const GAME_MOVPLANET_DESCTYPE2[] = {
													"The volcanic activity of this planet seems to be high.",
													"Lectures shows that this planet formed just about a million years ago.",
													"Sensors indicate that the core of this planet emits great amounts of energy.",
													"Lectures suggest some spaceship activity around the planet.",
													"An expectrum analyses suggests the presence of a great amount of molten materials.",													
													"No relevant wildlife has been detected on the planet.",
													"Images from the planet show clouds of ashes running through the surface of the planet.",
													"The database contains information about some old stations on the planet."
												};
			const char* const GAME_MOVPLANET_DESCTYPE3[] = {
													"The atmosphere of the planet shows some signs of terraformation.",
													"The planet seems to contain great amounts of metals.",
													"A surface analysis indicates the presence of cavities under the surface of the planet.",
													"Lectures suggest some spaceship activity around the planet.",
													"A small amount of wildlife has been detected on the planet.",
													"The database contains information about some old mining stations on the planet.",													
													"Sensors indicate deposits of magnetic materials somewhere in the planet.",
													"Some parts of the planet show the signs of the use of planetary weapons in the past."
												};
			const char* const GAME_MOVPLANET_DESCTYPE4[] = {
													"Great amounts of wildlife have been detected on the planet.",
													"The atmosphere of the planet shows signs of succesfull terraformation.",
													"Some parts of the planet seem to be covered by vegetation.",
													"Lectures suggest some spaceship activity around the planet.",
													"Sensors indicate significant humidity in this planet.",
													"The planet shows some signs of colonization.",
													"Conditions on this planet seem similar to those in erath.",
													"Several signals have been received coming from the planet,\nwhich may indicate the presence of inteligent lifeforms."
												};
			const char* const GAME_MOVPLANET_DESCTYPE5[] = {
													"Great amounts of water have been detected on this planet.",
													"The database contains information about some old investigation stations on the planet.",
													"Great amounts of wildlife have been detected on the planet.",
													"Lectures suggest some spaceship activity around the planet.",
													"The atmosphere of the planet shows some signs of terraformation.",													
													"Sensors cannot detect any significant amount of minerals on the surface of the planet.",
													"Water vapor clouds can be detected running through the atmorphere of the planet.",
													"Some data collectig probes have been detected in the planet."
												};
			const char* const GAME_MOVPLANET_DESCTYPE6[] = {
													"The thick atmosphere of the planet makes any analysis of its surface\nnearly impossible.",
													"More than 60% of the planet's volume ir formed by different types of gases.",
													"No wildlife has been detected on the planet.",
													"Lectures suggest some spaceship activity around the planet.",
													"The central part of the atmosphere seems to have been artificially modified.",
													"Clouds of different gases move quickly through the atmosphere forming storms.",
													"All water in this planet seems to exist in the form of vapor.",
													"Automated gas extraction drones have been detected in the planet."
												};
			const char* const GAME_MOVPLANET_DESCTYPE7[] = {
													"Great amounts of water have been detected on this planet.",
													"The atmosphere of the planet seems to be formed by gases that\ndo not accumulate much solar radiation.",
													"Lectures suggest some spaceship activity around the planet.",
													"No wildlife has been detected on the planet.",
													"All water in this planet seems to exist in the form of ice\ndue to low temperatures.",
													"The surface of the planet seems to be covered by frozen materials.",
													"Automated mining drones have been detected in the planet.",
													"A surface analysis indicates the presence of cavities under the surface of the planet."
												};
			const char* const GAME_MOVPLANET_DESCTYPE8[] = {
													"Sensors cannot detect any atmosphere in this planet.",
													"Impacts of asteroids can be seen on the surface of the planet.",
													"Automated mining drones have been detected in the planet.",
													"Lectures suggest some spaceship activity around the planet.",
													"The database contains information about some old mining stations on the planet.",
													"A surface analysis indicates the presence of cavities under the surface of the planet.",
													"Many valuable materials have been detected on the surface of the planet.",
													"Some parts of the planet show the signs of the use of planetary weapons in the past."
												};

			const int MAX_DESCTYPE_TEXT = 7;

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
			const char* const GAME_PLANET_EVENT_INTRO[] = {"Earth-like planet with a much drier condition.\nDespite the almost total lack of water,\ndesert planets have large habitable zones and can be highly life-sustaining.\nThe first earthling nomads thought of desert planets,\ntogether with ocean ones, as primary colonizing targets,\ndue to their terraformation posibilities.",
															"Type of planet formed mostly or entirely with molten minerals.\nLava planets are usually found orbiting extremely close to their system star.\nBecause of the extremely high temperatures and harsh conditions,\nno many settlers have ventured into stablishing a colony\nor even mining these kind of planets.",
															"Called red planets because of it's caracteristic color,\nwhich is caused by the accumulaiton of rust on it's surface.\nThese kind of planets contain high amounts of metals\nwhich are used in the construction of spaceships.\nThe abundance of mining attracts many who\nare in search of a way to make ends meet.",
															"Due to the enviromental similarities between Earth and\nthese kind of planets, they are often referred as\n\"Earth-like planets\" or \"Earth analogue\".\nEarth-like planets are planets which were succesfully terraformed during\nthe first days of the colonization,\nthis provided a chance for early travelers to stablish self-sufficient stable colonies,\nwhich were key for the colonization of harsher planets.",
															"Ocean planets are very similar to Earth-like ones,\nbut with its surface totally covered by water.\nThis kind of planets have lots of different lifeforms which attract\ninvestigators willing to uncover their mysteries.\nAlthough theoretically easier to colonize than, say desert planets,\nthe oceans vast deepness makes mineral resources mining harder,\nand may create a devastating greenhouse effect and variable climatology.",
															"Planets formed mainly by gases are difficult to colonize\nbecause the process that modifies the atmosphere\nis quite slow.\nNevertheless since this planets are the main source of various\ngases some gas \"mines\" have been stablished.",
															"Ice planets have hostile low-temperature enviroments,\nspecially in the surface, which rarely surpasses 80K degrees,\ndepending on their composition.\nTheoretically, ice planets could host life in subsurface oceans,\nbut not many colonies have wasted resources on stablishing life-support\nor mining stations due to the surface hostility.",
															"Rocky planet with a thin to non-existant atmosphere,\nwhich makes it very difficult to colonize.\nAfter human colonies developed an artificial self-sustain system,\nsome barren planets have been colonized with closed-enviroment stations.\nUsually, only mineral-rich or strategically well situated planets are worth colonizing.",
															"Though colonies in desert planets used to be common\nthe fight over resources and the hars conditions\nof this kind of planets have made them almos dissapear.\nNowadays the only big settlements which can be found\nare mining stations controlled by local warlords.",
															"Any settlement attempt in this kind of planet is extremely difficult.\nThere are though some stations that try to take advantage\nof the molten metals by creating factories on this planets.",
															"Most big cities found on this planets were originally\nmining stations, that's why many of them have big mining facilities\nat it's center.\nThe people working on the mines tend to live near those facilities while\nthe richest people of the city live in the outer ring.",
															"The most succesfull and technological colonial cities\nare found in succesfully terramorfed planets\nmostly because this planets were the first to be colonized.\nNormally this planets only have one megalopolis\nwhile the rest of the planet is covered by vegetation.",
															"Though this planets do not normally have big settelments,\ndue to the absence of soil, some big investigation facilities can be found.\nOcean planets have attracted the attention of scientist\ndue to the variety of the lifeforms that can be found in its oceans.",
															"Built as gas extracting stations the settlements stablished in gaseous planets\nthey grew as independent state cities due to their hability\nto hide themselves at will under the thick atmosphere of the planet\nfinally becoming common hiding places for many independent explorers.",
															"Though not many of this planets were initially colonized\nsome refugee camps were stablished under the surface during the second colonization\nwhich were later turned into actual cities.",
															"Initial stations stablished in this planets were mining facilities,\nbut during the second colonization many of them turned into military bases,\ndue to the strategic value of this planets and in order to protect\nthe asteroids containing the most valuable materials."
													};

			//Events


			//Nothing found
			const char* const GAME_DESERT_EVENT_NOTHING[] = {
				"After searching for several hours the sensors haven't detected anything useful.",
				"Several fierce sand storms have made impossible any kind of investigation on this planet.",
				"The only relevant signal was caused by some ruins buried under the sand\nbut sadly nothing usefull was found there."
			};

			const char* const GAME_LAVA_EVENT_NOTHING[] = {
				"After searching for several hours the sensors haven't detected anything useful.",
				"The magamatic nature of the planet has caused an overheating of the sensors.",
				"Toxic fumes and eruptions have made impossible to land on the planet."
			};

			const char* const GAME_RED_EVENT_NOTHING[] = {
				"After searching for several hours the sensors haven't detected anything useful.",
				"Interferences caused by the magnetic minerals of the planet have prevented any exploration.",
				"Strong winds and lightning storms have made the landing impossible."
			};

			const char* const GAME_EARTH_EVENT_NOTHING[] = {
				"After searching for several hours the sensors haven't detected anything useful.",
				"The dense vegentation has prevented the sensors from detecting any signal.",
				"A failiure in the sensoring system has mistaken a granitic structure for\na valuble mineral deposit."
			};

			const char* const GAME_OCEAN_EVENT_NOTHING[] = {
				"After searching for several hours the sensors haven't detected anything useful.",
				"No solid land was detected so the landing was aborted.",
				"The only promising signals detected were too far underwater for the ship to investigate."
			};

			const char* const GAME_GAS_EVENT_NOTHING[] = {
				"After searching for several hours the sensors haven't detected anything useful.",
				"The atmosphere contains too high concentration of toxic gases for anyone to explore.",
				"An specturm analysis of the gases composing the atmosphere of the planet has been unconclusive."
			};

			const char* const GAME_ICE_EVENT_NOTHING[] = {
				"After searching for several hours the sensors haven't detected anything useful.",
				"Several ice storms have prevented any attempts of landing in the planet.",
				"Ice growing on the sensors has blocked any signal."
			};

			const char* const GAME_BARREN_EVENT_NOTHING[] = {
				"After searching for several hours the sensors haven't detected anything useful.",
				"A meteor shower has prevented any exploration on the planet.",
				"Interferences due to cosmic radiation have blocked any signal coming from the planet."
			};

			const int MAX_EVENT_NOTHING = 2;

			const std::array<const char* const*,8> GAME_EVENT_NOTHING_TEXT = {
				GAME_DESERT_EVENT_NOTHING,
				GAME_LAVA_EVENT_NOTHING,
				GAME_RED_EVENT_NOTHING,
				GAME_EARTH_EVENT_NOTHING,
				GAME_OCEAN_EVENT_NOTHING,
				GAME_GAS_EVENT_NOTHING,
				GAME_ICE_EVENT_NOTHING,
				GAME_BARREN_EVENT_NOTHING
			};

			//Something valuable found

			const char* const GAME_DESERT_EVENT_VALUABLE[] = {
				"After analizing the surface a reserve of rare minerals has been found.",
				"An old buried investigation station has been found. Some items are still\nfunctional and can be sold.",
				"A deposit of titanium and technecium sand has been found."
			};

			const char* const GAME_LAVA_EVENT_VALUABLE[] = {
				"The sensors have found a crashed spaceship whose materials can be reused.",
				"The temperature lectures indicates the presence of a pool of molten metal.",
				"An eruption has released some valuble gases."
			};

			const char* const GAME_RED_EVENT_VALUABLE[] = {
				"After analizing the surface a reserve of rare minerals has been found.",
				"Lectures indicate great amounts of iron under the surface.",
				"There is an abandoned mining station on the planet which still has some stuff in it."
			};

			const char* const GAME_EARTH_EVENT_VALUABLE[] = {
				"After analizing the surface a reserve of rare minerals has been found.",
				"Various kinds of metal ore can be found under the mountain formations.",
				"The ruins of an old city contains several metal devices which can be melted."
			};

			const char* const GAME_OCEAN_EVENT_VALUABLE[] = {
				"The sensors have found a crashed spaceship whose materials can be reused.",
				"An analysis of the whater has shown the presence of valuble types of salt dissolved in it.",
				"The remainings of an abandoned investigation sation have provided some materials that can be sold."
			};

			const char* const GAME_GAS_EVENT_VALUABLE[] = {
				"The sensors have found a crashed spaceship whose materials can be reused.",
				"The gases that form the atmosphere of this planet can be filtered in order to obtain some valuable gases.",
				"The atmosphere contains some valuable solid particles in suspension that can be obtained."
			};

			const char* const GAME_ICE_EVENT_VALUABLE[] = {
				"The sensors have found a crashed spaceship whose materials can be reused.",
				"Buried in the ice there are some mineral deposits that can be exploited.",
				"The remainings of an artificial satelite found on the surface of the planet can be scavenged."
			};

			const char* const GAME_BARREN_EVENT_VALUABLE[] = {
				"The sensors have found a crashed spaceship whose materials can be reused.",
				"Some of the asteroids floating close aroud the planet contain valuable minerals.",
				"After analizing the surface a reserve of rare minerals has been found."
			};

			const int MAX_EVENT_VALUABLE = 2;

			const std::array<const char* const*,8> GAME_EVENT_VALUABLE_TEXT = {
				GAME_DESERT_EVENT_VALUABLE,
				GAME_LAVA_EVENT_VALUABLE,
				GAME_RED_EVENT_VALUABLE,
				GAME_EARTH_EVENT_VALUABLE,
				GAME_OCEAN_EVENT_VALUABLE,
				GAME_GAS_EVENT_VALUABLE,
				GAME_ICE_EVENT_VALUABLE,
				GAME_BARREN_EVENT_VALUABLE
			};

			// New Crew-Member

			const char* const GAME_EVENT_CREW_TEXT[] = {
				"You found a lost mercenary who agrees to work for you in echange for getting out of the planet.",
				"In a lost mining station there is a mechanic that agrees to join your crew.",
				"While extracting minerals from the planet the ship has detected a great magnetic anomaly.",
				""
			};

			//Ship crashed

			const char* const GAME_DESERT_EVENT_CRASH[] = {
				"A failure in the stabilizers has caused the ship to crash.",
				"The hull refrigeration system overheated causing some damage on the hull.",
				"The ship got caught in a sand storm."
			};

			const char* const GAME_LAVA_EVENT_CRASH[] = {
				"The hull refrigeration system overheated causing some damage on the hull.",
				"The ship was hit by a spray of magma.",
				"An eruption released acid gases near the ship."
			};

			const char* const GAME_RED_EVENT_CRASH[] = {
				"A failure in the stabilizers has caused the ship to crash.",
				"The ship was hit by a lightning during a storm.",
				"Magnetic disturbances damaged several electronic systems in the ship."
			};

			const char* const GAME_EARTH_EVENT_CRASH[] = {
				"A failure in the stabilizers has caused the ship to crash.",
				"The intense wind has caused the ship to crash against the vegetation.",
				"The high humidity of the atmosphere cause the engine to malfunction."
			};

			const char* const GAME_OCEAN_EVENT_CRASH[] = {
				"The high humidity of the atmosphere cause the engine to malfunction.",
				"A failure in the stabilizers has caused the ship to crash.",
				"The ship was hit by a lightning during a storm."
			};

			const char* const GAME_GAS_EVENT_CRASH[] = {
				"The ship was hit by a lightning during a storm.",
				"An accumulation of inflammable gases exploded.",
				"The lower atmosphere of this planet is composed by acid gases."
			};

			const char* const GAME_ICE_EVENT_CRASH[] = {
				"The refrigeration system froze, causing some damage to the hull.",
				"The ice plate on which the ship had landed collapsed.",
				"The ship got caught in an ice storm."
			};

			const char* const GAME_BARREN_EVENT_CRASH[] = {
				"The refrigeration system froze, causing some damage to the hull.",
				"The ship was hit by several asteroids during a meteor shower.",
				"Magnetic disturbances damaged several electronic systems in the ship."
			};

			const int MAX_EVENT_CRASH = 2;

			const std::array<const char* const*,8> GAME_EVENT_CRASH_TEXT = {
				GAME_DESERT_EVENT_CRASH,
				GAME_LAVA_EVENT_CRASH,
				GAME_RED_EVENT_CRASH,
				GAME_EARTH_EVENT_CRASH,
				GAME_OCEAN_EVENT_CRASH,
				GAME_GAS_EVENT_CRASH,
				GAME_ICE_EVENT_CRASH,
				GAME_BARREN_EVENT_CRASH
			};

			// Item Event Text

			const char* const GAME_EVENT_ITEM_TEXT[] = {
				"Wile examining a magnetic signal from some buried ruins you discover an statue.",
				"One of the lava pools that the ship analizes contains some rare cristals.",
				"Magnetic disturbances damaged several electronic systems in the ship.",
				"Hidden under dense vegetation an old device has been found.",
				"During an examination of the water the sensors have detected some rare type of algae.",
				"There are signals of a huge ship floating in the lower atmosphere of the planet.\nNo life has been detected but maybe there is something interesting inside.",
				"The tachyon sensors react to a device buried under the ice.",
				"An old mining station still contains some supplies."
			};

			// Item Event Text

			const char* const GAME_EVENT_DEATH_TEXT[] = {
				"Desert planets are famous for their tricky quicksands and sudden sand storms.",
				"If there is an adjective that cannot be applied to lava planets that is \"safe\".",
				"Many red planets have experienced very aggresive mining, which makes the surface sometimes unstable.",
				"There are many dangers lurking in a planet sized jungle.",
				"Sudden and violent storms are not uncommon in this kind of planets.",
				"Some gas planets have toxic atmospheres, which meke them very dangerous.",
				"The unstable and slippy surface of the planet can sometimes take its toll.",
				"Meteor showers and space suits malfuctions are the most common dangers in planets without a thick atmosphere."
			};

			// Gather resources text

			const char* const GAME_DESERT_EVENT_FUEL[] = {
				"Fusion elements have been sithesized from the materials found in the planet.",
				"Fusion elements can usually be found under the sand.",
				"The dark matter contained in the sand can be obtained by compressing and processing it."
			};

			const char* const GAME_LAVA_EVENT_FUEL[] = {
				"A meteorite containing dark matter has been found.",
				"The lava contains high concentrations of dark matter.",
				"An abandoned station still had some fuel."
			};

			const char* const GAME_RED_EVENT_FUEL[] = {
				"An abandoned mining station still had some fuel.",
				"The metals of the planet can be processed and turned into fuel.",
				"You found a mine of fusion material."
			};

			const char* const GAME_EARTH_EVENT_FUEL[] = {
				"The water of the planet can be processed in order to obtain fusion materials.",
				"The biomass of the planet can be a source of primary materials for the dark matter reactor.",
				"Some dark matter has been processed from the atmosphere of the planet."
			};

			const char* const GAME_OCEAN_EVENT_FUEL[] = {
				"The water of the planet can be processed in order to obtain fusion materials.",
				"The biomass of the planet can be a source of primary materials for the dark matter reactor.",
				"Some dark matter has been processed from the atmosphere of the planet."
			};

			const char* const GAME_GAS_EVENT_FUEL[] = {
				"Some dark matter has been processed from the atmosphere of the planet.",
				"It's easy to obtain fusable hidrogen from the atmosphere of the planet.",
				"The secondary fusion reactor may have troubles fusing heavy gases,\nbut they can be used as low quality fuel."
			};

			const char* const GAME_ICE_EVENT_FUEL[] = {
				"Ice planets sometimes hide dar matter under their frozen surface.",
				"The ice can be processed the same way as water in order to obtain\nhidrogen for the secondary reactor.",
				"A meteorite containing dark matter has been found."
			};

			const char* const GAME_BARREN_EVENT_FUEL[] = {
				"A meteorite containing dark matter has been found.",
				"Due to their absence of atmosphere this kind of planets are a good place\nto find asteroids containing minerals that can be used as fuel.",
				"An abandoned station still had some fuel."
			};

			const int MAX_EVENT_FUEL = 2;

			const std::array<const char* const*,8> GAME_EVENT_FUEL_TEXT = {
				GAME_DESERT_EVENT_FUEL,
				GAME_LAVA_EVENT_FUEL,
				GAME_RED_EVENT_FUEL,
				GAME_EARTH_EVENT_FUEL,
				GAME_OCEAN_EVENT_FUEL,
				GAME_GAS_EVENT_FUEL,
				GAME_ICE_EVENT_FUEL,
				GAME_BARREN_EVENT_FUEL
			};

			// Mission Text

			const char* const GAME_MISSION_AQUIRED_TEXT[] = {
				"A random person sits near you\nin a pub and says:\n\"I've heard you have a ship. ¿Are you intrested in taking a job?\"",
				"\"We have had some problems with pirates lately,\nand it would be nice if they'd dissapear, ¿Are you in?\"",
				"\"I just need to get out of the system\nquietly.\"",
				"Some investigators are willing\nto buy any iteresting item without\nquestioning its origins.",
				"\"I need some fuel for my ship,\nand i`m willing to pay extra if no questions are asked.\""
			};

			// --Planets Names--

			//Planets by system
			const char* const GAME_PLANETS_SYSTEM0[] = {
				"Sirrah_0_","Sirrah_1_","Sirrah_2_","Sirrah_3_","Sirrah_4_","Sirrah_5_","Sirrah_6_","Sirrah_7_","Sirrah_8_","Sirrah_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM1[] = {
				"Cih_0_","Cih_1_","Cih_2_","Cih_3_","Cih_4_","Cih_5_","Syprah","Cih_7_","Cih_8_","Cih_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM2[] = {
				"Canopus_0_","Canopus_1_","Uylea","Canopus_3_","Canopus_4_","Canopus_5_","Canopus_6_","Canopus_7_","Canopus_8_","Canopus_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM3[] = {
				"Navi_0_","Navi_1_","Navi_2_","Navi_3_","Navi_4_","Navi_5_","Navi_6_","Navi_7_","Navi_8_","Navi_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM4[] = {
				"Kurhah_0_","Kurhah_1_","Kurhah_2_","Kurhah_3_","Kurhah_4_","Kurhah_5_","Kurhah_6_","Kurhah_7_","Kurhah_8_","Kurhah_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM5[] = {
				"Garnet_0_","Garnet_1_","Iosconoe","Garnet_3_","Garnet_4_","Garnet_5_","Garnet_6_","Garnet_7_","Garnet_8_","Garnet_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM6[] = {
				"Deneb_Kaitos_0_","Deneb_Kaitos_1_","Deneb_Kaitos_2_","Deneb_Kaitos_3_","Deneb_Kaitos_4_","Deneb_Kaitos_5_","Osmone","Deneb_Kaitos_7_","Deneb_Kaitos_8_","Deneb_Kaitos_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM7[] = {
				"Mira_0_","Mira_1_","Mira_2_","Mira_3_","Mira_4_","Mira_5_","Mira_6_","Mira_7_","Mira_8_","Mira_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM8[] = {
				"Tso_Hea_0_","Tso_Hea_1_","Tso_Hea_2_","Tso_Hea_3_","Tso_Hea_4_","Tso_Hea_5_","Tso_Hea_6_","Tso_Hea_7_","Tso_Hea_8_","Tso_Hea_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM9[] = {
				"Gienah_0_","Gienah_1_","Gienah_2_","Prun","Gienah_4_","Gienah_5_","Gienah_6_","Gienah_7_","Gienah_8_","Gienah_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM10[] = {
				"Algorab_0_","Algorab_1_","Algorab_2_","Algorab_3_","Algorab_4_","Algorab_5_","Algorab_6_","Algorab_7_","Algorab_8_","Algorab_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM11[] = {
				"Minkar_0_","Minkar_1_","Minkar_2_","Minkar_3_","Minkar_4_","Minkar_5_","Minkar_6_","Minkar_7_","Minkar_8_","Minkar_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM12[] = {
				"Acrux_0_","Acrux_1_","Acrux_2_","Acrux_3_","Acrux_4_","Acrux_5_","Acrux_6_","Acrux_7_","Acrux_8_","Acrux_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM13[] = {
				"Gacrux_0_","Gacrux_1_","Gacrux_2_","Gacrux_3_","Gacrux_4_","Gacrux_5_","Gacrux_6_","Gacrux_7_","Gacrux_8_","Gacrux_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM14[] = {
				"CanopusII_0_","CanopusII_1_","CanopusII_2_","CanopusII_3_","CanopusII_4_","CanopusII_5_","CanopusII_6_","Chenon","CanopusII_8_","CanopusII_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM15[] = {
				"Edasich_0_","Edasich_1_","Edasich_2_","Edasich_3_","Edasich_4_","Edasich_5_","Edasich_6_","Edasich_7_","Edasich_8_","Edasich_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM16[] = {
				"Edasich_0_","Edasich_1_","Youwei","Edasich_3_","Edasich_4_","Edasich_5_","Edasich_6_","Edasich_7_","Edasich_8_","Edasich_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM17[] = {
				"Acamar_0_","Acamar_1_","Acamar_2_","Acamar_3_","Acamar_4_","Acamar_5_","Acamar_6_","Acamar_7_","Acamar_8_","Acamar_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM18[] = {
				"Castor_0_","Castor_1_","Castor_2_","Castor_3_","Castor_4_","Castor_5_","Castor_6_","Castor_7_","Castor_8_","Castor_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM19[] = {
				"Al_Na'ir_0_","Al_Na'ir_1_","Al_Na'ir_2_","Al_Na'ir_3_","Al_Na'ir_4_","Al_Niyat","Al_Na'ir_6_","Al_Na'ir_7_","Al_Na'ir_8_","Al_Na'ir_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM20[] = {
				"Al_Dhanab_0_","Al_Dhanab_1_","Al_Dhanab_2_","Al_Dhanab_3_","Al_Dhanab_4_","Al_Dhanab_5_","Al_Dhanab_6_","Al_Dhanab_7_","Al_Dhanab_8_","Al_Dhanab_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM21[] = {
				"Kornephoros_0_","Kornephoros_1_","Kornephoros_2_","Kornephoros_3_","Kornephoros_4_","Kornephoros_5_","Kornephoros_6_","Kornephoros_7_","Kornephoros_8_","Kornephoros_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM22[] = {
				"Ukdah_0_","Ukdah_1_","Ukdah_2_","Ukdah_3_","Ukdah_4_","Ukdah_5_","Rhodesh","Ukdah_7_","Ukdah_8_","Ukdah_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM23[] = {
				"Cor_Koraelis_0_","Cor_Koraelis_1_","Cor_Koraelis_2_","Cor_Koraelis_3_","Cor_Koraelis_4_","Cor_Koraelis_5_","Cor_Koraelis_6_","Cor_Koraelis_7_","Koerus","Cor_Koraelis_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM24[] = {
				"Zozma_0_","Ilorus","Zozma_2_","Zozma_3_","Zozma_4_","Zozma_5_","Zozma_6_","Zozma_7_","Zozma_8_","Zozma_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM25[] = {
				"Ras_Elased_0_","Ras_Elased_1_","Ras_Elased_2_","Ras_Elased_3_","Ras_Elased_4_","Secutania","Ras_Elased_6_","Ras_Elased_7_","Ras_Elased_8_","Ras_Elased_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM26[] = {
				"Tsze_Tseang_0_","Tsze_Tseang_1_","Tsze_Tseang_2_","Kuma","Tsze_Tseang_4_","Tsze_Tseang_5_","Tsze_Tseang_6_","Tsze_Tseang_7_","Tsze_Tseang_8_","Tsze_Tseang_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM27[] = {
				"Vega_0_","Vega_1_","Vega_2_","Vega_3_","Vega_4_","Vega_5_","Vega_6_","Aquila","Vega_8_","Vega_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM28[] = {
				"Sulafat_0_","Sulafat_1_","Sulafat_2_","Sulafat_3_","Sulafat_4_","Sulafat_5_","Sulafat_6_","Ashurn","Sulafat_8_","Sulafat_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM29[] = {
				"Yed_0_","Yed_1_","Yed_2_","Yed_3_","Yesmanu","Yed_5_","Yed_6_","Yed_7_","Yed_8_","Yed_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM30[] = {
				"Zed_0_","Zed_1_","Zed_2_","Zed_3_","Zed_4_","Zed_5_","Zed_6_","Zed_7_","Zed_8_","Zed_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM31[] = {
				"Hastur","Hiades_1_","Hiades_2_","Hiades_3_","Hiades_4_","Hiades_5_","Hiades_6_","Carcossa","Hiades_8_","Hiades_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM32[] = {
				"Betelgeuse_0_","Betelgeuse_1_","Betelgeuse_2_","Betelgeuse_3_","Betelgeuse_4_","Azathoth","Betelgeuse_6_","Betelgeuse_7_","Nyarlathoteph","Betelgeuse_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM33[] = {
				"Rigel_0_","Rigel_1_","Rigel_2_","Rigel_3_","Rigel_4_","Rigel_5_","Rigel_6_","Rigel_7_","Rigel_8_","Rigel_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM34[] = {
				"Saiph_0_","Telara","Saiph_2_","Saiph_3_","Saiph_4_","Saiph_5_","Saiph_6_","Saiph_7_","Saiph_8_","Saiph_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM35[] = {
				"Fomalhaut_0_","Fomalhaut_1_","Fomalhaut_2_","Fomalhaut_3_","Fomalhaut_4_","Fomalhaut_5_","Fomalhaut_6_","Fomalhaut_7_","Fomalhaut_8_","Yuggoth" 
			};

			const char* const GAME_PLANETS_SYSTEM36[] = {
				"Sirius_0_","Sirius_1_","Mere","Sirius_3_","Sirius_4_","Sirius_5_","Sirius_6_","Sirius_7_","Sirius_8_","Sirius_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM37[] = {
				"Arcturus_0_","Arcturus_1_","Arcturus_2_","Arcturus_3_","Arcturus_4_","Arcturus_5_","Arcturus_6_","Arcturus_7_","Arcturus_8_","Arcturus_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM38[] = {
				"Aldebaran_0_","Aldebaran_1_","Aldebaran_2_","Aldebaran_3_","Aldebaran_4_","Aldebaran_5_","Hali","Aldebaran_7_","Aldebaran_8_","Aldebaran_9_" 
			};

			const char* const GAME_PLANETS_SYSTEM39[] = {
				"Tseen_Ke_0_","Tseen_Ke_1_","Tseen_Ke_2_","Thonoe","Tseen_Ke_4_","Tseen_Ke_5_","Tseen_Ke_6_","Tseen_Ke_7_","Tseen_Ke_8_","Tseen_Ke_9_" 
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
				"Ion Pulse Engine", "Engine\nAs the most basic faster than light\nengine it provides a short range quantic jump\nand medium slower than light speed.\nJump Distance:35\nFuel Cost:140\nMax speed: 1.0 -- Acceleration: 1.0 -- Rotation speed: 1.0"
			};

			const char* const GAME_RESONANCE_ENGINE[2] = {
				"Electromagnetic Resonance Engine", "Engine\nThis engine is composed by an electromagnetic resonator\nwich creates a diferential force by creating\na constructive interference between several electromagnetic waves.\nJump Distance:50\nFuel Cost:200\nMax speed: 1.0 -- Acceleration: 1.5 -- Rotation speed: 1.25"
			};

			const char* const GAME_SPATIAL_ENGINE[2] = {
				"Point Zero Engine", "Engine\nAn engine that uses the expansion\nof the space as a source of impulse.\nIt's very efficient, but its slower than light capabilities are not great.\nJump Distance:60\nFuel Cost:100\nMax speed: 0.75 -- Acceleration: 1.0 -- Rotation speed: 0.75"
			};

			const char* const GAME_REPULSION_ENGINE[2] = {
				"Repulsion Engine", "Engine\nThe most used engine in cargo ships.\nIts jump distance its quite high, but its fuel consume too.\nJump Distance:85\nFuel Cost:300\nMax speed: 0.75 -- Acceleration: 1.0 -- Rotation speed: 1.5"
			};

			const char* const GAME_GRAVITY_ENGINE[2] = {
				"Gravity Engine", "Engine\nThis engine was used during the colonization.\nIt uses gravitational waves to curve the space.\nJump Distance:65\nFuel Cost:150\nMax speed: 1.5 -- Acceleration: 1.0 -- Rotation speed: 1.5"
			};

			const char* const GAME_SOLAR_ENGINE[2] = {
				"Solar Wind Wing", "Engine\nNormally considered just a expensive toy\nthis \"engine\" captures ions theat escape from stars\nrather than generating any kind of impulse.\nJump Distance:45\nFuel Cost:90\nMax speed: 1.75 -- Acceleration: 1.75 -- Rotation speed: 1.5"
			};

			const char* const GAME_COPOSITE_ENGINE[2] = {
				"Composite Engine", "Engine\nA resonance engine combined with an old fashioned\ngas combustion engine to solve its\nslower than light speed issues.\nJump Distance:60\nFuel Cost:100\nMax speed: 1.25 -- Acceleration: 1.25 -- Rotation speed: 1.0"
			};

			const char* const GAME_WORMHOLE_ENGINE[2] = {
				"Wormhole Engine", "Engine\nThought to be just a rumour this engine appeared\nin some ships some years ago.\nIt's said that it's the result of some\nillegal teleportation experiments.\nJump Distance:70\nFuel Cost:250\nMax speed: 2.0 -- Acceleration: 1.0 -- Rotation speed: 1.0"
			};

			const char* const GAME_GAMMA_ENGINE[2] = {
				"Gamma Ray Engine", "Engine\nInspired by the gamma rays created by the black holes\nthis engine creates two gamma ray streams.\nJump Distance:55\nFuel Cost:150\nMax speed: 1.75 -- Acceleration: 2.0 -- Rotation speed: 1.75"
			};

			const char* const GAME_PLASMA_ENGINE[2] = {
				"Plasma Stream Engine", "Engine\nA top notch engine that\nis both efficient and powerful.\nJump Distance:85\nFuel Cost:105\nMax speed: 2.0 -- Acceleration: 1.5 -- Rotation speed: 1.5"
			};

			const std::pair<std::string,std::pair<int, int>> ENGINE_DATA_MAP[] =
			{
				std::make_pair (GAME_PULSE_ENGINE[0],std::make_pair (35, 140)), // distance, cost
				std::make_pair (GAME_RESONANCE_ENGINE[0],std::make_pair (50, 200)),
				std::make_pair (GAME_SPATIAL_ENGINE[0],std::make_pair (60, 100)),
				std::make_pair (GAME_REPULSION_ENGINE[0],std::make_pair (85, 300)),
				std::make_pair (GAME_GRAVITY_ENGINE[0],std::make_pair (65, 150)),
				std::make_pair (GAME_SOLAR_ENGINE[0],std::make_pair (45, 90)),
				std::make_pair (GAME_COPOSITE_ENGINE[0],std::make_pair (60, 100)),
				std::make_pair (GAME_WORMHOLE_ENGINE[0],std::make_pair (70, 250)),
				std::make_pair (GAME_GAMMA_ENGINE[0],std::make_pair (55, 150)),
				std::make_pair (GAME_PLASMA_ENGINE[0],std::make_pair (85, 105))
			};

			const std::tuple<std::string, float, float, float> ENGINE_VEL_DATA[] = 
			{
				std::make_tuple(GAME_PULSE_ENGINE[0], 1.0,1.0,1.0),//max speed, acceleration, rotation speed
				std::make_tuple(GAME_RESONANCE_ENGINE[0], 1.0,1.5,1.25),
				std::make_tuple(GAME_SPATIAL_ENGINE[0], 0.75,1.0,0.75),
				std::make_tuple(GAME_REPULSION_ENGINE[0], 0.75,1.0,1.5),
				std::make_tuple(GAME_GRAVITY_ENGINE[0], 0.75,1.0,1.5),
				std::make_tuple(GAME_SOLAR_ENGINE[0], 1.75,1.75,1.5),
				std::make_tuple(GAME_COPOSITE_ENGINE[0], 1.25,1.25,1.0),
				std::make_tuple(GAME_WORMHOLE_ENGINE[0], 2.0,1.0,1.0),
				std::make_tuple(GAME_GAMMA_ENGINE[0], 1.75,2.0,1.75),
				std::make_tuple(GAME_PLASMA_ENGINE[0], 2.0,1.5,1.5)
			};

			const int TOTAL_ENGINES = 10;

			const std::pair<std::string,std::pair<int, int>> ENGINE_COST[] =
			{
				std::make_pair (GAME_PULSE_ENGINE[0],std::make_pair (30, 6)), // base cost, variance/10
				std::make_pair (GAME_RESONANCE_ENGINE[0],std::make_pair (100, 20)),
				std::make_pair (GAME_SPATIAL_ENGINE[0],std::make_pair (150, 35)),
				std::make_pair(GAME_REPULSION_ENGINE[0], std::make_pair (250, 10)),
				std::make_pair(GAME_GRAVITY_ENGINE[0], std::make_pair (230, 40)),
				std::make_pair(GAME_SOLAR_ENGINE[0], std::make_pair (300, 20)),
				std::make_pair(GAME_COPOSITE_ENGINE[0], std::make_pair (260, 50)),
				std::make_pair(GAME_WORMHOLE_ENGINE[0], std::make_pair (360, 30)),
				std::make_pair(GAME_GAMMA_ENGINE[0], std::make_pair (420, 30)),
				std::make_pair (GAME_PLASMA_ENGINE[0],std::make_pair (400, 70))
			};

			const std::array<const char* const*,TOTAL_ENGINES> GAME_ENGINES_LIST = {
				GAME_PULSE_ENGINE,
				GAME_RESONANCE_ENGINE,
				GAME_SPATIAL_ENGINE,
				GAME_REPULSION_ENGINE,
				GAME_GRAVITY_ENGINE,
				GAME_SOLAR_ENGINE,
				GAME_COPOSITE_ENGINE,
				GAME_WORMHOLE_ENGINE,
				GAME_GAMMA_ENGINE,
				GAME_PLASMA_ENGINE
			};

			// Sensors
			const char* const GAME_RADAR[2] = {
				"Radar", "Lvl 1 Sensor\nThis common sensor measures\nelectromagnetic echoes.\nIt detects some enemies and\nit`s also able to get some information\nfrom planets."
			};

			const char* const GAME_ION_SENSOR[2] = {
				"Ion Trail Detector", "Lvl 2 Sensor\nThis device gives a rough\naproximation of the number of\nenemy ships in a given area\nby measuring the electromagnetic\nfield caused by spaceships\nion trails."
			};

			const char* const GAME_LIFE_SENSOR[2] = {
				"Life Sensor", "Lvl 3 Sensor\nThis type of sensor can detect\nmost of lifeforms what makes it able\nto determine if a planet\nis inhabited or not."
			};

			const char* const GAME_TACHYON_SENSOR[2] = {
				"Tachyon Resonator", "Lvl 4 Sensor\nAn advanced tachyon signal detector\nthat reacts to specific frequencies."
			};

			const int TOTAL_SENSORS = 4;

			const std::array<const char* const*,TOTAL_SENSORS> GAME_SENSORS_LIST = {
				GAME_RADAR,
				GAME_ION_SENSOR,
				GAME_LIFE_SENSOR,
				GAME_TACHYON_SENSOR
			};

			// Special equipment
			const char* const GAME_ITEM_REGENERATOR[3] = {
				GAME_REGENERATOR, "Shield Energy Regenerator", "Special Device\nA quantic reverse\nenthropy device\nthat slowly regenerates\nthe shield energy over time."
			};

			const char* const GAME_ITEM_STEALTH[3] = {
				GAME_STEALTH, "Stealth Module", "Special Device\nHides the ship from enemies sensors.\nIt has a high energy cost."
			};

			const char* const GAME_ITEM_ENHANCER[3] = {
				GAME_SHIELD_ENHANCER, "Shield Regeneration Enhancer", "Special Device\nA quantic amplificator\nthat used in combination with\na shield regenerator increases the shield regeneration\nspeed."
			};

			const char* const GAME_ITEM_ARMOR_LIGHT[3] = {
				GAME_ARMOR_LIGHT, "Light Hull Reinforcement", "Special Device\nMetal plates used to reinforce\nthe weaker parts\nof the hull."
			};

			const char* const GAME_ITEM_ARMOR_MEDIUM[3] = {
				GAME_ARMOR_MEDIUM, "Medium Hull Reinforcement", "Special Device\nCeramic cover for the hull\ndesigend to disperse heat."
			};

			const char* const GAME_ITEM_ARMOR_HEAVY[3] = {
				GAME_ARMOR_HEAVY, "Heavy Hull Reinforcement", "Special Device\nHeavy titanium based\nreinforcement plates."
			};

			const char* const GAME_ITEM_PROCESSOR_SMALL[3] = {
				GAME_SMALL_PROCESSOR, "Small Mineral Processor", "Special Device\nStandar mining machinery\nused by most colonial fortune-seekers.\n+5% Scrap"
			};

			const char* const GAME_ITEM_PROCESSOR_MEDIUM[3] = {
				GAME_MEDIUM_PROCESSOR, "Medium Mineral Processor", "Special Device\nA chemical mineral refiner\nconsidered dangerous by most\nminers because it uses strong acids.\n+10% Scrap"
			};

			const char* const GAME_ITEM_PROCESSOR_BIG[3] = {
				GAME_BIG_PROCESSOR, "Big Mineral Processor", "Special Device\nA furnace that melts both metal\nand mineral making them easier to purify.\n+15% Scrap"
			};

			const char* const GAME_ITEM_COMPRESSOR_SMALL[3] = {
				GAME_SMALL_COMPRESSOR, "Fuel Compressor", "Special Device\nDark matter compressor\nused by many ships as a way to save\n some fuel.\n-5% Fuel Cost"
			};

			const char* const GAME_ITEM_COMPRESSOR_MEDIUM[3] = {
				GAME_MEDIUM_COMPRESSOR, "Automated Fuel Feeder", "Special Device\nThis device optimizes the\nfuel feeding rate of the engine\nbased on the linear momentum of the ship.\n-10% Fuel Cost"
			};

			const char* const GAME_ITEM_COMPRESSOR_BIG[3] = {
				GAME_BIG_COMPRESSOR, "Heat Cogenerator", "Special Device\nModified engine refrigeration system that\nuses the heat produced by the engine\nto pre-heat the fuel.\n-15% Fuel Cost"
			};

			const int TOTAL_SPECIAL = 12;

			const std::pair<std::string,std::pair<int, int>> SPECIAL_EQUIPMENT_COST[] =
			{
				std::make_pair (GAME_ITEM_REGENERATOR[0],std::make_pair (200, 20)), // base cost, variance/10
				std::make_pair (GAME_ITEM_ENHANCER[0],std::make_pair (100, 10)),
				std::make_pair (GAME_ITEM_ARMOR_LIGHT[0],std::make_pair (100, 10)),
				std::make_pair (GAME_ITEM_ARMOR_MEDIUM[0],std::make_pair (200, 20)),
				std::make_pair (GAME_ITEM_ARMOR_HEAVY[0],std::make_pair (400, 10)),
				std::make_pair (GAME_ITEM_PROCESSOR_SMALL[0],std::make_pair (70, 7)),
				std::make_pair (GAME_ITEM_PROCESSOR_MEDIUM[0],std::make_pair (140, 14)),
				std::make_pair (GAME_ITEM_PROCESSOR_BIG[0],std::make_pair (280, 28)),
				std::make_pair (GAME_ITEM_COMPRESSOR_SMALL[0],std::make_pair (80, 11)),
				std::make_pair (GAME_ITEM_COMPRESSOR_MEDIUM[0],std::make_pair (160, 22)),
				std::make_pair (GAME_ITEM_COMPRESSOR_BIG[0],std::make_pair (360, 33)),
				std::make_pair (GAME_ITEM_STEALTH[0],std::make_pair (300, 10))
			};

			const std::array<const char* const*,TOTAL_SPECIAL> GAME_SPECIAL_EQUIPMENT_LIST = {
				GAME_ITEM_REGENERATOR,
				GAME_ITEM_ENHANCER,
				GAME_ITEM_ARMOR_LIGHT,
				GAME_ITEM_ARMOR_MEDIUM,
				GAME_ITEM_ARMOR_HEAVY,
				GAME_ITEM_PROCESSOR_SMALL,
				GAME_ITEM_PROCESSOR_MEDIUM,
				GAME_ITEM_PROCESSOR_BIG,
				GAME_ITEM_COMPRESSOR_SMALL,
				GAME_ITEM_COMPRESSOR_MEDIUM,
				GAME_ITEM_COMPRESSOR_BIG,
				GAME_ITEM_STEALTH
			};

			// Items
			const char* const GAME_REWARD[2] = {
				"Multipass", "Item\nAn electronic keycard\nwhich gives acces to various locations."
			};

			const char* const GAME_USELESS_DESERT[2] = {
				"Sandstone Totem", "Item\nAn ancient totem\ncarved in sandstone\nfound in a desert planet.\nSeems that belonged\nto an ancient civilization."
			};

			const char* const GAME_USELESS_LAVA[2] = {
				"Volcanic Cristal", "Item\n.A strange cristal\nfound in a volcanic planet.\nIts different faces reflect the\nlight in different indescribable colors."
			};

			const char* const GAME_USELESS_RED[2] = {
				"Magnetic Stone", "Item\n.A big stone that creates a powerfull\nmagnetic field.\nIn order to manipulate it\nhas to be magnetically isolated."
			};

			const char* const GAME_USELESS_EARTH[2] = {
				"Terraforming Capsule", "Item\n.An old spacial probe equipped\nmodify the atmosphere of a planet.\nMany of these were used in order\nto make planets suitable for\ncolonizers."
			};

			const char* const GAME_USELESS_OCEAN[2] = {
				"Regenerating Algae", "Item\n.A rare type of algae\nwhich has the ability to regenerate itself.\nOne of the multiple species that\nlive in the ocean planets."
			};

			const char* const GAME_USELESS_GAS[2] = {
				"Crio-Stasis Chamber", "Item\n.A crio-stasis module\nprobably used during the first colonization."
			};

			const char* const GAME_USELESS_ICE[2] = {
				"Unactive Beacon", "Item\n.One of the many beacons that were launched\nduring the first colonization wave.\nIt's damaged and ist information\nhas been lost."
			};

			const char* const GAME_USELESS_BARREN[2] = {
				"Mining Robot", "Item\n.An old mining robot.\nIt's really dated but its still functional."
			};

			const int TOTAL_ITEMS = 9;

			const std::pair<std::string,std::pair<int, int>> ITEM_COST[] =
			{
				std::make_pair (GAME_REWARD[0],std::make_pair (-1, -1)), // base cost, variance/10
				std::make_pair (GAME_USELESS_DESERT[0],std::make_pair (-1, -1)),
				std::make_pair (GAME_USELESS_LAVA[0],std::make_pair (-1, -1)),
				std::make_pair (GAME_USELESS_RED[0],std::make_pair (-1, -1)),
				std::make_pair (GAME_USELESS_EARTH[0],std::make_pair (-1, -1)),
				std::make_pair (GAME_USELESS_OCEAN[0],std::make_pair (-1, -1)),
				std::make_pair (GAME_USELESS_GAS[0],std::make_pair (-1, -1)),
				std::make_pair (GAME_USELESS_ICE[0],std::make_pair (-1, -1)),
				std::make_pair (GAME_USELESS_BARREN[0],std::make_pair (-1, -1))
			};

			const std::array<const char* const*,TOTAL_ITEMS> GAME_ITEM_LIST = {
				GAME_REWARD,
				GAME_USELESS_DESERT,
				GAME_USELESS_LAVA,
				GAME_USELESS_RED,
				GAME_USELESS_EARTH,
				GAME_USELESS_OCEAN,
				GAME_USELESS_GAS,
				GAME_USELESS_ICE,
				GAME_USELESS_BARREN
			};

			const int REPAIR_HULL_COST = 100;

			// Crew member hire cost & description
			
			const char* const GAME_SHOP_MILITARY[2] = {
				"Weapon Expert", "Military trained weapon expert\nthat you can hire.\n+1 Military"
			};

			const char* const GAME_SHOP_ENGINEER[2] = {
				"Mechanic", "A mechanichal engineer\nthat is seeking for a ship to work at.\n+1 Engineer"
			};

			const char* const GAME_SHOP_SCIENTIFIC[2] = {
				"Rogue Investigator", "An intrepid scientist willing to travel\nthrough the galaxy in order\nto discover its secrets.\n+1 Scientific"
			};

			const std::pair<std::string,std::pair<int, int>> CREW_SHOP_COST[] =
			{
				std::make_pair (GAME_SHOP_MILITARY[0],std::make_pair (150, 10)), // base cost, variance/10
				std::make_pair (GAME_SHOP_ENGINEER[0],std::make_pair (150, 10)),
				std::make_pair (GAME_SHOP_SCIENTIFIC[0],std::make_pair (150, 10))
			};

			const std::array<const char* const*,3> GAME_CREW_SHOP_LIST = {
				GAME_SHOP_MILITARY,
				GAME_SHOP_ENGINEER,
				GAME_SHOP_SCIENTIFIC
			};

			const char* const GAME_CREW_TYPES[] = {
				GAME_MILITARY,
				GAME_ENGINEERS,
				GAME_SCIENTIFICS
			};

			/// TODO -- Internationalization -- /// }End
		}

	}

}
