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

#include <cassert>
#include <random>

#include <OgreVector3.h>

#include <log.h>

#include <Common/Util/Math.h>
#include <Common/Configure/Configure.h>
#include <Common/Data/Spawn_Constants.h>
#include <Common/Data/Game_Constants.h>
#include <Application/Manager/MissionManager.h>
#include <Application/Manager/GameManager.h>

#include "MapGenerator.h"
#include "../MapEntity.h"


using namespace Common::Data::Spawn;
using namespace Common::Configuration;

namespace Map
{
	const char* const LOG_CMAPGENERATOR = "Map::CMapGenerator";

	typedef std::list<Map::CMapEntity*> TEntityList;

	CMapGenerator::CMapGenerator()
	{

	}

	CMapGenerator::~CMapGenerator()
	{
		resetMaps();
	}

#pragma warning( push )
#pragma warning( disable : 4244 )

	TEntityList CMapGenerator::generateGalaxy(const std::string& descriptor)
	{	
		using namespace Common::Data::Spawn;

		log_trace(LOG_CMAPGENERATOR,"Generating a galaxy...\n");

		{
			using namespace Common::Configuration;
			setDefaultFile(CONFIGURE_FILE);
			setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());
		}

		std::time_t seed;

        setDefaultFile(CONFIGURE_FILE);
		setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());

		//If you already have a seed just use that 
		if(m_SeedMap.find(descriptor) != m_SeedMap.end())
			seed = m_SeedMap.find(descriptor)->second;
		else
		{
			//Else generate a new one and store it
			seed = std::chrono::system_clock::now().time_since_epoch().count();
			m_SeedMap[descriptor] = seed;
		}
        std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution(Common::Configuration::defaultValue<int>(GEN_SYSTEM_MINGEN),
														Common::Configuration::defaultValue<int>(GEN_SYSTEM_MAXGEN));
		int nodeNum = distribution(generator);

		/// TODO --Establish a minimum distance between stars-- ///

		TEntityList entityList;
		CMapEntity *entityInProgress;
		std::string tmpStr1,tmpStr2;

		std::vector<Ogre::Vector3> tmpPos;

		//Generate targets
		std::uniform_int_distribution<int> targetDist(0,Common::Configuration::defaultValue<int>(GEN_SYSTEM_BASENUM)+nodeNum);
		std::set<int> targets;
		
		for(int i=0;i<4;++i){
			int targ;
			do{
				targ = targetDist(generator);
			}while(targ == 1);

			targets.insert(targ);
		}

		for(int i=0;i<Common::Configuration::defaultValue<int>(GEN_SYSTEM_BASENUM)+nodeNum;++i){

			//Example galaxy generation

			std::string name = Common::Configuration::getDefaultValue(GEN_SYSTEM_TYPE);
			char str[16];
			sprintf(str,"%s%d",name.c_str(),i);

			entityInProgress = new CMapEntity(str);

			tmpStr1 = name;
			entityInProgress->setType(tmpStr1.c_str());

			std::uniform_int_distribution<int> neg(0,1);
			std::uniform_real_distribution<float> maxDist(0,Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD));
			Ogre::Vector3 pos(0.0f,0.0f,0.0f);

			if(i >= Common::Configuration::defaultValue<int>(GEN_SYSTEM_BASENUM)+nodeNum-2 /*&& checkDist(tmpPos,tmpPos[1],1.3*Common::Configuration::defaultValue<float>(GEN_SYSTEM_MINDIST), 1)*/ )
			{
				std::uniform_real_distribution<float> angle(0.0f,360.0f);
				std::uniform_real_distribution<float> dist(1.0f,1.8f);
				float rad(dist(generator));
				int j(0);
				do{
					if(j<100){
						pos.x = tmpPos[1].x + rad*Common::Configuration::defaultValue<float>(GEN_SYSTEM_MINDIST) * std::cos(angle(generator)) ;
						pos.y = tmpPos[1].y + rad*Common::Configuration::defaultValue<float>(GEN_SYSTEM_MINDIST) * std::sin(angle(generator));
						pos.z =	Common::Configuration::defaultValue<float>(GEN_SYSTEM_ZPOS);

						if(pos.y < -Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD)) pos.y = -Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD);
						if(pos.y > Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD)) pos.y = Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD);

						if(pos.x < -Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD)) pos.x = -Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD);
						if(pos.x > Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD)) pos.x = Common::Configuration::defaultValue<float>(GEN_SYSTEM_RAD);

						++j;
					}else{
							pos.x = neg(generator)?maxDist(generator):-maxDist(generator);
							pos.y = neg(generator)?maxDist(generator):-maxDist(generator);
							pos.z =	Common::Configuration::defaultValue<float>(GEN_SYSTEM_ZPOS);
					}

				}while(!checkDist(tmpPos,pos,Common::Configuration::defaultValue<float>(GEN_SYSTEM_MINDIST)));

			}else{

				do{
					pos.x = neg(generator)?maxDist(generator):-maxDist(generator);
					pos.y = neg(generator)?maxDist(generator):-maxDist(generator);
					pos.z =	Common::Configuration::defaultValue<float>(GEN_SYSTEM_ZPOS);
				}while(!checkDist(tmpPos,pos,Common::Configuration::defaultValue<float>(GEN_SYSTEM_MINDIST)));

			}

			tmpPos.push_back(pos);

			tmpStr1 = GRAPHIC_STATIC;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_SYSTEM_STATIC);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = COMMON_POSITION;
			tmpStr2 = "{" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "}";
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = COMMON_ORIENTATION;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_SYSTEM_ORIENTATION);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = COMMON_SELECTION_MASK;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_SYSTEM_MASK);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = GRAPHIC_GLOW;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_SYSTEM_GLOW);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = GRAPHIC_GLOW_SCALE;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_SYSTEM_GLOW_SCALE);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = GRAPHIC_GREEN_GLOW;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_SYSTEM_GLOW);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = GRAPHIC_MODEL;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_SYSTEM_MODEL);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = GRAPHIC_EXTRA;
			tmpStr2 = targets.find(i)==targets.end()?COMMON_NO_TARGET : COMMON_TARGET;
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			entityList.push_back(entityInProgress);
		}

		Application::CMissionManager::getInstance()->setSystemNumber(Common::Configuration::defaultValue<int>(GEN_SYSTEM_BASENUM)+nodeNum);

		//SkyDome

		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_GAL_SKYDOME_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_GAL_SKYDOME_TYPE);
		entityInProgress->setType(tmpStr1.c_str());


		entityList.push_back(entityInProgress);

		//Camera
        entityInProgress = new CMapEntity(getDefaultValue(GEN_SYS_STATIC_CAMERA_ENTIYNAME));
        entityInProgress->setType(getDefaultValue(GEN_SYS_STATIC_CAMERA_ENTIYTYPE));

        entityInProgress->setAttribute(STATIC_CAMERA_NAME, getDefaultValue(GEN_SYS_STATIC_CAMERA_NAME));
        entityInProgress->setAttribute(STATIC_CAMERA_NEAR, getDefaultValue(GEN_SYS_STATIC_CAMERA_NEAR));
        entityInProgress->setAttribute(STATIC_CAMERA_FAR,  getDefaultValue(GEN_SYS_STATIC_CAMERA_FAR));
        entityInProgress->setAttribute(STATIC_CAMERA_FOV,  getDefaultValue(GEN_SYS_STATIC_CAMERA_FOV));
        entityInProgress->setAttribute(COMMON_POSITION,    getDefaultValue(GEN_SYS_STATIC_CAMERA_POSITION));
        entityInProgress->setAttribute(COMMON_ORIENTATION, getDefaultValue(GEN_SYS_STATIC_CAMERA_ORIENTATION));

		entityList.push_back(entityInProgress);

		//Light
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_GALAXY_AMBIENT_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_GALAXY_AMBIENT_TYPE);
		entityInProgress->setType(tmpStr1.c_str());


		tmpStr1 = COMMON_COLOUR_VALUE;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_GALAXY_AMBIENT_COLOUR);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		entityList.push_back(entityInProgress);

		return entityList;

	}

	TEntityList CMapGenerator::generateSystem(const std::string& descriptor)
	{
		using namespace Common::Data::Spawn;

		log_trace(LOG_CMAPGENERATOR,"Generating a system...\n");
		{
			using namespace Common::Configuration;
			setDefaultFile(CONFIGURE_FILE);
			setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());
		}
		TEntityList entityList;
		CMapEntity *entityInProgress;
		std::string tmpStr1,tmpStr2;

		std::time_t seed;

        setDefaultFile(CONFIGURE_FILE);
		setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());

		//If you already have a seed just use that 
		if(m_SeedMap.find(descriptor) != m_SeedMap.end())
			seed = m_SeedMap.find(descriptor)->second;
		else
		{
			//Else generate a new one and store it
			seed = std::chrono::system_clock::now().time_since_epoch().count();
			m_SeedMap[descriptor] = seed;
		}

		//Several distributions are declared here
		std::default_random_engine generator(static_cast<unsigned>(seed));
		std::uniform_int_distribution<int> planetDist(3,10);
		std::uniform_int_distribution<int> descDist(0,Common::Data::Game::MAX_DESCTYPE_TEXT);

        std::set<int> nums;
		std::uniform_real_distribution<float> angleDist(0.0,2*Common::Util::Math::PI);
		std::uniform_real_distribution<float> rotDist(Common::Configuration::defaultValue<float>(GEN_MOVPLANET_MIN_ROT),
														Common::Configuration::defaultValue<float>(GEN_MOVPLANET_MAX_ROT));
		std::uniform_int_distribution<int> neg(0,1);
		std::uniform_real_distribution<float> velDist(0.0,1.0);
		std::uniform_real_distribution<float> scale(0.0,1.0);
		std::uniform_int_distribution<int> probability(1,100);

		const float dist = Common::Configuration::defaultValue<float>(GEN_MOVPLANET_SEPARATION);
		const float maxVel = Common::Configuration::defaultValue<float>(GEN_MOVPLANET_MAX_VEL);
		const float maxScale = Common::Configuration::defaultValue<float>(GEN_MOVPLANET_MAX_SCALE);
		
		{
			//Creation of the star at the center of the scene

			//TODO -Add aditional information extracted from the descriptor-- //

			entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_STAR_NAME));

			tmpStr1 = Common::Configuration::getDefaultValue(GEN_STAR_TYPE);
			entityInProgress->setType(tmpStr1.c_str());

			tmpStr1 = GRAPHIC_STATIC;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_STATIC);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = COMMON_POSITION;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_POSITION);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = COMMON_ORIENTATION;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_ORIENTATION);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = GRAPHIC_SCALE;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_SCALE);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = COMMON_SELECTION_MASK;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_SELECTION_MASK);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = GRAPHIC_MODEL;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_MODEL);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = Common::Configuration::getDefaultValue(GEN_STAR_LIGHT_STATIC);
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_LIGHT_STATIC_VALUE);;
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr1 = COMMON_DIFFUSE_COLOUR;
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_LIGHT_DIFFUSE);
			entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

			tmpStr2 = Common::Configuration::getDefaultValue(GEN_STAR_LIGHT_SPECULAR);
			entityInProgress->setAttribute(COMMON_SPECULAR_COLOUR, tmpStr2.c_str());

			entityList.push_back(entityInProgress);
		}

		float distance = 0;
		
		int numPlanets = planetDist(generator);

		std::vector<int> planetsPos;

		int targetPlanet = -1;

		if(descriptor.substr(descriptor.find("_")+1,1) == "1"){
			std::uniform_int_distribution<int> targetDist(0,numPlanets-1);
			targetPlanet = targetDist(generator);
		}

		for(int i=0;i<numPlanets;++i)
		{
			//TODO -Add aditional information extracted from the descriptor-- /
			std::string name(Common::Configuration::getDefaultValue(GEN_MOVPLANET_NAME));
			char str[16];
			sprintf(str,"%s%d",name.c_str(),i);

			for(int j=1;j<11;++j){

				if(neg(generator) && nums.find(j) == nums.end() ){

                    nums.insert(j);
					planetsPos.push_back(j);

					const int c = j+1;

					entityInProgress = new CMapEntity(str);

					float angle = angleDist(generator);

					tmpStr1 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_TYPE);
					entityInProgress->setType(tmpStr1.c_str());

					tmpStr1 = GRAPHIC_STATIC;
					tmpStr2 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_STATIC);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = COMMON_POSITION;
					tmpStr2 = "{" + std::to_string(c*std::cos(angle)*dist)+", 0, "+std::to_string(c*std::sin(angle)*dist)+"}";
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = COMMON_SELECTION_MASK;
					tmpStr2 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_SELECTION_MASK);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_CENTRAL_POS);
					tmpStr2 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_CENTER);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_ANGLE);
					tmpStr2 = std::to_string(angle);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_DISTANCE);
					float tmpDistance = c*dist;
					if(tmpDistance > distance) distance = tmpDistance;
					tmpStr2 = std::to_string(tmpDistance);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					int model;
					int limit;

					if(j<3){
						std::uniform_real_distribution<float>::param_type velParam(0.4f,1.0f);
						std::uniform_real_distribution<float>::param_type scaleParam(0.25f,0.4f);

						velDist.param(velParam);
						scale.param(scaleParam);

						neg(generator)?model=0:model=1;
						limit=Common::Configuration::defaultValue<int>(GEN_PLANET_INH_PROB[0]);

					}else if(j<6){

						std::uniform_real_distribution<float>::param_type velParam(0.4f,0.7f);
						std::uniform_real_distribution<float>::param_type scaleParam(0.4f,0.7f);

						velDist.param(velParam);
						scale.param(scaleParam);

						neg(generator)?model=2:model=3;
						limit=Common::Configuration::defaultValue<int>(GEN_PLANET_INH_PROB[1]);;

					}else if(j<8){

						std::uniform_real_distribution<float>::param_type velParam(0.1f,0.4f);
						std::uniform_real_distribution<float>::param_type scaleParam(0.5f,1.0f);

						velDist.param(velParam);
						scale.param(scaleParam);

						neg(generator)?model=4:model=5;
						limit=Common::Configuration::defaultValue<int>(GEN_PLANET_INH_PROB[2]);;

					}else{

						std::uniform_real_distribution<float>::param_type velParam(0.4f,1.0f);
						std::uniform_real_distribution<float>::param_type scaleParam(0.25f,0.4f);

						velDist.param(velParam);
						scale.param(scaleParam);

						neg(generator)?model=6:model=7;
						limit=Common::Configuration::defaultValue<int>(GEN_PLANET_INH_PROB[3]);;
					}

					tmpStr1 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_ANGULAR_VEL);
					tmpStr2 = std::to_string(velDist(generator)*maxVel);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = GRAPHIC_SCALE;
					float sc = scale(generator);
					tmpStr2 = std::to_string(sc*maxScale);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = GRAPHIC_GLOW;
					tmpStr2 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_GLOW);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = GRAPHIC_GLOW_SCALE;
					tmpStr2 = std::to_string(1.85 - sc/2.0f);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = GRAPHIC_GREEN_GLOW;
					tmpStr2 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_GREEN_GLOW);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = GRAPHIC_MODEL;
					tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_MODELS[model]);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_YAW);
					tmpStr2 = std::to_string(rotDist(generator));
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = GRAPHIC_EXTRA;
					i == targetPlanet ? tmpStr2 = COMMON_TARGET : tmpStr2 = COMMON_NO_TARGET;
					probability(generator) <= limit ? tmpStr2 += COMMON_INHABITED : tmpStr2 += COMMON_DESERTED;
					int prob(probability(generator)),risk(0);
					if(i == targetPlanet){
						if(prob < 50 - 10 * Application::CGameManager::getInstance()->getObjectivesAquired()) risk=3;
						else risk = 4;
					}else{
						if(prob > 15 - Application::CGameManager::getInstance()->getObjectivesAquired() * 2)risk=1;
						if(prob > 45 - Application::CGameManager::getInstance()->getObjectivesAquired() * 2)risk=2;
						if(prob > 70 - Application::CGameManager::getInstance()->getObjectivesAquired() * 2)risk=3;
						if(prob > 90 - Application::CGameManager::getInstance()->getObjectivesAquired() * 2)risk=4;
					}
					int desc;
					do{
						desc = descDist(generator);
					}while(desc == 3 && risk == 0);
					tmpStr2 += std::to_string(sc)[0];
					tmpStr2 += std::to_string(sc)[2];
					tmpStr2 += "0"+std::to_string(model);
					tmpStr2 += std::to_string(j-1);
					tmpStr2 += std::to_string(risk);
					tmpStr2 += Common::Data::Game::GAME_MOVPLANET_DESC[model][desc];
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_CENTER_NODE_NAME);
					tmpStr2 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_CENTER_NAME);
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					tmpStr1 = Common::Configuration::getDefaultValue(GEN_MOVPLANET_DIRECTION);
					tmpStr2 = neg(generator)?"true":"false";
					entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

					entityList.push_back(entityInProgress);

					break;
				}
			}
		}

		Application::CMissionManager::getInstance()->setPlanets(planetsPos);

		//Camera

		entityInProgress = new CMapEntity(getDefaultValue(GEN_STATIC_CAMERA_ENTIYNAME));
        entityInProgress->setType(getDefaultValue(GEN_STATIC_CAMERA_ENTIYTYPE));

        entityInProgress->setAttribute(STATIC_CAMERA_NAME,        getDefaultValue(GEN_STATIC_CAMERA_NAME));
        entityInProgress->setAttribute(STATIC_CAMERA_NEAR,        getDefaultValue(GEN_STATIC_CAMERA_NEAR));
        entityInProgress->setAttribute(STATIC_CAMERA_FAR,         getDefaultValue(GEN_STATIC_CAMERA_FAR));
        entityInProgress->setAttribute(STATIC_CAMERA_FOV,         getDefaultValue(GEN_STATIC_CAMERA_FOV));
        entityInProgress->setAttribute(STATIC_CAMERA_ORIENTATION, getDefaultValue(GEN_STATIC_CAMERA_ORIENTATION));
        entityInProgress->setAttribute(STATIC_CAMERA_LOOKAT,      getDefaultValue(GEN_STATIC_CAMERA_LOOKAT));

		tmpStr2 = "{"+ std::to_string(distance*1.4f) + ", " + std::to_string(distance*1.4) + ", " + std::to_string(distance/3.5f) + "}";
		entityInProgress->setAttribute(STATIC_CAMERA_POSITION, tmpStr2.c_str());

		entityList.push_back(entityInProgress);

		//Texture Camera
		entityInProgress = new CMapEntity(getDefaultValue(GEN_SYS_TEXTURE_CAMERA_ENTITYNAME));
		entityInProgress->setType(getDefaultValue(GEN_SYS_TEXTURE_CAMERA_ENTITYTYPE));

		entityInProgress->setAttribute(STATIC_CAMERA_NAME,        getDefaultValue(GEN_SYS_TEXTURE_CAMERA_NAME));
		entityInProgress->setAttribute(STATIC_CAMERA_NEAR,        getDefaultValue(GEN_SYS_TEXTURE_CAMERA_NEAR));
		entityInProgress->setAttribute(STATIC_CAMERA_FAR,		  getDefaultValue(GEN_SYS_TEXTURE_CAMERA_FAR));
		entityInProgress->setAttribute(STATIC_CAMERA_FOV,         getDefaultValue(GEN_SYS_TEXTURE_CAMERA_FOV));
		entityInProgress->setAttribute(COMMON_POSITION,			  getDefaultValue(GEN_SYS_TEXTURE_CAMERA_POSITION));
        entityInProgress->setAttribute(STATIC_CAMERA_LOOKAT,      getDefaultValue(GEN_SYS_TEXTURE_CAMERA_LOOKAT));

		entityList.push_back(entityInProgress);

        //Skybox
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_SKYBOX_NAME));
		entityInProgress->setType(getDefaultValue(GEN_SKYBOX_TYPE));
		entityList.push_back(entityInProgress);

		//Light
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_SYS_AMBIENT_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_SYS_AMBIENT_TYPE);
		entityInProgress->setType(tmpStr1.c_str());


		tmpStr1 = COMMON_COLOUR_VALUE;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_SYS_AMBIENT_COLOUR);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		entityList.push_back(entityInProgress);

		return entityList;

	}

	TEntityList CMapGenerator::generatePlanet(const std::string& descriptor)
	{
		log_trace(LOG_CMAPGENERATOR,"Generating a planet...\n");
		
		using namespace Common::Data::Spawn;
        using namespace Common::Configuration;

		TEntityList entityList;
		CMapEntity *entityInProgress;
		std::string tmpStr1,tmpStr2;

		setDefaultFile(CONFIGURE_FILE);
		setDefaultFile(getDefaultValue(CONF_GENERATOR_PATH).c_str());

		std::time_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> numAsteroids(0,Common::Configuration::defaultValue<int>(GEN_ASTEROID_NUM));
		std::uniform_real_distribution<float> posDist(0.0f,Common::Configuration::defaultValue<float>(GEN_ASTEROID_POS));
		std::uniform_real_distribution<float> degDist(0.0f,Common::Configuration::defaultValue<float>(GEN_ASTEROID_ORI));
		std::uniform_int_distribution<int> neg(0,1);
		std::uniform_int_distribution<int> astDist(0,Common::Configuration::defaultValue<int>(GEN_ASTEROID_DIM));

		std::uniform_int_distribution<int> numAsteroids2(0,Common::Configuration::defaultValue<int>(GEN_STATIC_ASTEROID_NUM));
		std::uniform_real_distribution<float> posDist2(0.0f,Common::Configuration::defaultValue<float>(GEN_STATIC_ASTEROID_POS));
		std::uniform_real_distribution<float> degDist2(0.0f,Common::Configuration::defaultValue<float>(GEN_STATIC_ASTEROID_ORI));
		std::uniform_int_distribution<int> astDist2(0,Common::Configuration::defaultValue<int>(GEN_STATIC_ASTEROID_DIM));

		std::uniform_int_distribution<int> enemyDist(0,Common::Configuration::defaultValue<int>(GEN_WANDER_ENEMY_NUM));

		std::string sc(descriptor.substr(descriptor.length()-6,1)+"."+descriptor.substr(descriptor.length()-5,1));
		std::string md(descriptor.substr(descriptor.length()-4,2));
		std::string rs(descriptor.substr(descriptor.length()-1,1));
		float scale(std::atof(sc.c_str()));
		int model(std::atoi(md.c_str()));
		int risk(std::atoi(rs.c_str()));


		std::vector<Ogre::Vector3> tmpPos;

		for(int i = 0 ;i < Common::Configuration::defaultValue<int>(GEN_ASTEROID_MIN_NUM) + numAsteroids(generator); ++i)
		{
			char str[16];
			std::string name = Common::Configuration::getDefaultValue(GEN_ASTEROID_TYPE);
			sprintf(str,"%s%d",name.c_str(),i);

			entityInProgress = new CMapEntity(str);

			tmpStr1 = name;
			entityInProgress->setType(tmpStr1.c_str());

			Ogre::Vector3 pos(0.0f,0.0f,0.0f);

			do{
				pos.x = neg(generator)?posDist(generator):-posDist(generator);
				pos.y = defaultValue<int>(GEN_ASTEROID_Y_AXIS_POS);
				pos.z =	neg(generator)?posDist(generator):-posDist(generator);
			} while(!checkDist(tmpPos,pos,10.0f));

			tmpPos.push_back(pos);

			entityInProgress->setAttribute(GRAPHIC_STATIC, getDefaultValue(GEN_ASTEROID_GRAPHIC_STATIC).c_str());

			tmpStr2 = "{" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "}";
			entityInProgress->setAttribute(COMMON_POSITION, tmpStr2.c_str());

			pos.x = neg(generator)? degDist(generator) : -degDist(generator);
            pos.y = neg(generator)? degDist(generator) : -degDist(generator);
			pos.z =	neg(generator)? degDist(generator) : -degDist(generator);

			tmpStr2 = "{" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "}";
			entityInProgress->setAttribute(COMMON_ORIENTATION,tmpStr2.c_str());
			/*
			tmpStr2 = Common::Configuration::getDefaultValue(GEN_ASTEROID_PHYSX_SHAPE);
			entityInProgress->setAttribute(PHYSIC_ENTITY, tmpStr2.c_str());
			*/
			entityInProgress->setAttribute("physic_entity", "rigid");
			entityInProgress->setAttribute("physic_type", "dynamic");
			entityInProgress->setAttribute("physic_mass", "10");
			entityInProgress->setAttribute("physic_shape", "sphere");

			int mod = astDist(generator);

			std::string aux = GEN_ASTEROID_PHYSX_RADIUS;
			aux += std::to_string(mod);
			tmpStr2 = Common::Configuration::getDefaultValue(aux);
			entityInProgress->setAttribute(PHYSIC_RADIUS, tmpStr2.c_str());

			aux = GEN_ASTEROID_MODEL + std::to_string(mod);
			entityInProgress->setAttribute(GRAPHIC_MODEL, Common::Configuration::getDefaultValue(aux));

			entityInProgress->setAttribute(GRAPHIC_VISIBILITY, getDefaultValue(GEN_ASTEROID_VISIBILITY_MASK).c_str());

			tmpStr2 = std::to_string((float)(defaultValue<float>(GEN_ASTEROID_GRAPHIC_SCALE) * (mod+1)));
			entityInProgress->setAttribute(GRAPHIC_SCALE, tmpStr2.c_str());
            entityInProgress->setAttribute(COMMON_LIFE, getDefaultValue(GEN_ASTEROID_LIFE));

			entityList.push_back(entityInProgress);
        }

		//Static Asteroids

		tmpPos.clear();

		for(int i = 0 ;i < Common::Configuration::defaultValue<int>(GEN_STATIC_ASTEROID_MIN_NUM) + numAsteroids2(generator); ++i)
		{
			char str[32];
			std::string name = Common::Configuration::getDefaultValue(GEN_STATIC_ASTEROID_TYPE);
			sprintf(str,"%s%d",name.c_str(),i);

			entityInProgress = new CMapEntity(str);

			tmpStr1 = name;
			entityInProgress->setType(tmpStr1.c_str());

			entityInProgress->setAttribute(GRAPHIC_STATIC, getDefaultValue(GEN_STATIC_ASTEROID_GRAPHIC_STATIC));

			Ogre::Vector3 pos(0.0f,0.0f,0.0f);

			do{
				pos.x = neg(generator)?posDist2(generator):-posDist2(generator);
				pos.y = defaultValue<int>(GEN_STATIC_ASTEROID_Y_AXIS_POS);
				pos.z =	neg(generator)?posDist2(generator):-posDist2(generator);
			} while(!checkDist(tmpPos,pos,10.0f));

			tmpPos.push_back(pos);

			tmpStr2 = "{" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "}";
			entityInProgress->setAttribute(COMMON_POSITION, tmpStr2.c_str());

			pos.x = neg(generator)? degDist2(generator) : -degDist2(generator);
            pos.y = neg(generator)? degDist2(generator) : -degDist2(generator);
			pos.z =	neg(generator)? degDist2(generator) : -degDist2(generator);

			tmpStr2 = "{" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "}";
			entityInProgress->setAttribute(COMMON_ORIENTATION,tmpStr2.c_str());

			int mod = astDist2(generator);

			std::string aux = GEN_STATIC_ASTEROID_MODEL + std::to_string(mod);
			entityInProgress->setAttribute(GRAPHIC_MODEL, Common::Configuration::getDefaultValue(aux));

			entityInProgress->setAttribute(GRAPHIC_VISIBILITY, getDefaultValue(GEN_STATIC_ASTEROID_VISIBILITY_MASK).c_str());

			tmpStr2 = std::to_string((float)(defaultValue<float>(GEN_STATIC_ASTEROID_GRAPHIC_SCALE) * (mod+1)));
			entityInProgress->setAttribute(GRAPHIC_SCALE, tmpStr2.c_str());

			entityList.push_back(entityInProgress);
		}

		//Player
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLAYER_NAME));

		entityInProgress->setType(getDefaultValue(GEN_PLAYER_TYPE));

		entityInProgress->setAttribute(COMMON_IS_PLAYER, getDefaultValue(GEN_PLAYER_IS_PLAYER));
		entityInProgress->setAttribute(GRAPHIC_STATIC, getDefaultValue(GEN_PLAYER_GRAPHIC_STATIC));
		entityInProgress->setAttribute(MINIMAP_ENTITY_SCALE, getDefaultValue(GEN_PLAYER_MINIMAP_SCALE));
		entityInProgress->setAttribute(COMMON_POSITION, getDefaultValue(GEN_PLAYER_START_POSITION));
		entityInProgress->setAttribute(COMMON_ORIENTATION, getDefaultValue(GEN_PLAYER_START_ORIENTATION));
		entityInProgress->setAttribute(COMMON_SPEED, getDefaultValue(GEN_PLAYER_SPEED));
		entityInProgress->setAttribute(COMMON_ROTATION_SPEED, getDefaultValue(GEN_PLAYER_ROTATION_SPEED));
		entityInProgress->setAttribute(GRAPHIC_MODEL,  getDefaultValue(GEN_PLAYER_MODEL));
		entityInProgress->setAttribute(GRAPHIC_SCALE, getDefaultValue(GEN_PLAYER_SCALE));
		/*
		entityInProgress->setAttribute(PHYSIC_ENTITY,  getDefaultValue(GEN_PLAYER_PHYSX_SHAPE));
		entityInProgress->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_PLAYER_PHYSX_RADIUS));
		entityInProgress->setAttribute(PHYSIC_HEIGHT,  getDefaultValue(GEN_PLAYER_PHYSX_HEIGHT));
		entityInProgress->setAttribute(PHYSIC_CONTACT_FUNCTION, std::to_string(onContactFunction::PLAYER));
		*/
		entityInProgress->setAttribute("physic_entity", "rigid");
		entityInProgress->setAttribute("physic_type", "kinematic");
		entityInProgress->setAttribute("physic_shape", "sphere");
		entityInProgress->setAttribute("physic_mass", "10");
		entityInProgress->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_PLAYER_PHYSX_RADIUS));
        entityInProgress->setAttribute(COMMON_MAXROLL,   getDefaultValue(GEN_PLAYER_MAXROLL));
        entityInProgress->setAttribute(COMMON_ROLLSPEED, getDefaultValue(GEN_PLAYER_ROLL_SPEED));
        entityInProgress->setAttribute(COMMON_FRICTION,  getDefaultValue(GEN_PLAYER_FRICTION));
        entityInProgress->setAttribute(COMMON_DESACELERATION, getDefaultValue(GEN_PLAYER_DESACELERATION));
        entityInProgress->setAttribute(COMMON_MAX_SPEED,      getDefaultValue(GEN_PLAYER_MAX_SPEED));
        entityInProgress->setAttribute(COMMON_LIFE,           getDefaultValue(GEN_PLAYER_LIFE));
		entityInProgress->setAttribute(COMMON_SHIELD,         getDefaultValue(GEN_PLAYER_SHIELD));
        entityInProgress->setAttribute(COMMON_SHIELD_RESIS,   getDefaultValue(GEN_PLAYER_SHIELD_RESISTANCE));
        entityInProgress->setAttribute(COMMON_PRIMARY_WEAPON, getDefaultValue(GEN_PLAYER_PRIM_WEAPON));
        entityInProgress->setAttribute(COMMON_SECONDARY_WEAPON, getDefaultValue(GEN_PLAYER_SEC_WEAPON));

        // Weapons
        //-- laser
        entityInProgress->setAttribute(LASER_DAMAGE, getDefaultValue(GEN_LASER_DAMAGE));
        entityInProgress->setAttribute(LASER_RANGE,  getDefaultValue(GEN_LASER_RANGE));
        entityInProgress->setAttribute(LASER_COST,   getDefaultValue(GEN_LASER_COST));
        //-- laser beam
        entityInProgress->setAttribute(LASERBEAM_DAMAGE,   getDefaultValue(GEN_LASERBEAM_DAMAGE));
        entityInProgress->setAttribute(LASERBEAM_RANGE,    getDefaultValue(GEN_LASERBEAM_RANGE));
        entityInProgress->setAttribute(LASERBEAM_COST,     getDefaultValue(GEN_LASERBEAM_COST));
        entityInProgress->setAttribute(LASERBEAM_INTERVAL, getDefaultValue(GEN_LASERBEAM_INTERVAL));


		entityInProgress->setAttribute("perception_entity_type","player");

		//(Player) Example of multiple soundbank and event loading
		/*tmpStr1 = "property0";
		tmpStr2 = "distance";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "property1";
		tmpStr2 = "distance";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "entities0";
		tmpStr2 = "Asteroid";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "entities1";
		tmpStr2 = "Enemy";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "range00";
		tmpStr2 = "900";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "range01";
		tmpStr2 = "500";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "range02";
		tmpStr2 = "200";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "range10";
		tmpStr2 = "400";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "range11";
		tmpStr2 = "200";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "range12";
		tmpStr2 = "50";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "bank0";
		tmpStr2 = "Master Bank.bank";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "bank1";
		tmpStr2 = "Master Bank.strings.bank";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "bank2";
		tmpStr2 = "Character.bank";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "event0";
		tmpStr2 = "event:/Character/Footsteps/Footsteps";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "event1";
		tmpStr2 = "event:/Character/Footsteps/Footsteps";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "eventName0";
		tmpStr2 = "distEvent";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "eventName1";
		tmpStr2 = "distEventEnem";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "parameter0";
		tmpStr2 = "Surface";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "parameter1";
		tmpStr2 = "Surface";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "parameterName0";
		tmpStr2 = "paramSurf";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "parameterName1";
		tmpStr2 = "paramSurfEnem";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());*/

		entityList.push_back(entityInProgress);

		//Wanderer Enemies
		unsigned enemy_seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine enemy_generator(enemy_seed);
		std::uniform_int_distribution<int> enemy_distribution(
					atof(getDefaultValue(GEN_ENEMY_NEG_BOUNDARY).c_str()),
					atof(getDefaultValue(GEN_ENEMY_POS_BOUNDARY).c_str()));
		/*for (int i=0; i<defaultValue<int>(GEN_WANDER_ENEMY_NUM); ++i) {*/
		int enemiesNum(enemyDist(generator)),div(5 - risk);
		for (int i=0; !risk?0:i< Application::CGameManager::getInstance()->getObjectivesAquired() + (enemiesNum/div); ++i) {
			char str[20];
			std::string name = getDefaultValue(GEN_ENEMY_NAME);
			sprintf(str,"%s%d",name.c_str(),i);
			entityInProgress = new CMapEntity(str);

			entityInProgress->setType(getDefaultValue(GEN_ENEMY_TYPE));

			entityInProgress->setAttribute(GRAPHIC_STATIC, getDefaultValue(GEN_ENEMY_GRAPHIC_STATIC));
			entityInProgress->setAttribute(MINIMAP_ENTITY_SCALE, getDefaultValue(GEN_ENEMY_MINIMAP_SCALE));
			entityInProgress->setAttribute(MINIMAP_ENTITY_ENEMY, getDefaultValue(GEN_ENEMY_MINIMAP_ENEMY));
			std::string enemy_position = "{" + std::to_string(enemy_distribution(enemy_generator)) +
										 ", " + getDefaultValue(GEN_ENEMY_Y_PLANE) + ", " +
										 std::to_string(enemy_distribution(enemy_generator)) +"}";
			entityInProgress->setAttribute(COMMON_POSITION, enemy_position);
			entityInProgress->setAttribute(COMMON_ORIENTATION, getDefaultValue(GEN_ENEMY_START_ORIENTATION));
			entityInProgress->setAttribute(COMMON_MAX_SPEED, getDefaultValue(GEN_ENEMY_MAX_SPEED));
			entityInProgress->setAttribute(COMMON_ROTATION_SPEED, getDefaultValue(GEN_ENEMY_ROTATION_SPEED));
			entityInProgress->setAttribute(GRAPHIC_MODEL, getDefaultValue(GEN_ENEMY_MODEL));
			//entityInProgress->setAttribute(PHYSIC_ENTITY, getDefaultValue(GEN_ENEMY_PHYSX_SHAPE));
			//entityInProgress->setAttribute(PHYSIC_RADIUS, getDefaultValue(GEN_ENEMY_PHYSX_RADIUS));
			//entityInProgress->setAttribute(PHYSIC_HEIGHT, getDefaultValue(GEN_ENEMY_PHYSX_HEIGHT));
			//entityInProgress->setAttribute(PHYSIC_CONTACT_FUNCTION,std::to_string(onContactFunction::ENEMY));
			entityInProgress->setAttribute("physic_entity", "rigid");
			entityInProgress->setAttribute("physic_type", "dynamic");
			entityInProgress->setAttribute("physic_shape", "sphere");
			entityInProgress->setAttribute("physic_mass", "10");
			entityInProgress->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_ENEMY_PHYSX_RADIUS));
			entityInProgress->setAttribute(COMMON_MAXROLL, getDefaultValue(GEN_ENEMY_MAXROLL));
			entityInProgress->setAttribute(COMMON_ROLLSPEED, getDefaultValue(GEN_ENEMY_ROLL_SPEED));
			entityInProgress->setAttribute(COMMON_ACCEL, getDefaultValue(GEN_ENEMY_ACCEL));
			entityInProgress->setAttribute(COMMON_ROTATION_ACCEL, getDefaultValue(GEN_ENEMY_ROTATION_ACCEL));
			entityInProgress->setAttribute(COMMON_LIFE, getDefaultValue(GEN_ENEMY_LIFE));
			entityInProgress->setAttribute(AI_TOLERANCE, getDefaultValue(GEN_ENEMY_MOV_TOLERANCE));
			entityInProgress->setAttribute(AI_BEHAVIOR,getDefaultValue(GEN_ENEMY_BEHAVIOR));

			entityInProgress->setAttribute("perception_entity_type","enemy");

			entityInProgress->setAttribute(COMMON_PRIMARY_WEAPON, "laser");
			entityInProgress->setAttribute(COMMON_SECONDARY_WEAPON, "missile_linear");

			entityList.push_back(entityInProgress);
		}

		//Dummy Enemy
		entityInProgress = new CMapEntity("DummyEnemy"/*+'0'*/);//The commented 0 is for sound events

		entityInProgress->setType(getDefaultValue(GEN_ENEMY_TYPE));

		entityInProgress->setAttribute(GRAPHIC_STATIC, getDefaultValue(GEN_ENEMY_GRAPHIC_STATIC));
		entityInProgress->setAttribute(MINIMAP_ENTITY_SCALE, getDefaultValue(GEN_ENEMY_MINIMAP_SCALE));
		entityInProgress->setAttribute(MINIMAP_ENTITY_ENEMY, getDefaultValue(GEN_ENEMY_MINIMAP_ENEMY));
		entityInProgress->setAttribute(COMMON_POSITION, getDefaultValue(GEN_ENEMY_START_POSITION));
		entityInProgress->setAttribute(COMMON_ORIENTATION, getDefaultValue(GEN_ENEMY_START_ORIENTATION));
		entityInProgress->setAttribute(COMMON_MAX_SPEED, getDefaultValue(GEN_ENEMY_MAX_SPEED));
		entityInProgress->setAttribute(COMMON_ROTATION_SPEED, getDefaultValue(GEN_ENEMY_ROTATION_SPEED));
		entityInProgress->setAttribute(GRAPHIC_MODEL, getDefaultValue(GEN_ENEMY_MODEL));
		//entityInProgress->setAttribute(PHYSIC_ENTITY, getDefaultValue(GEN_ENEMY_PHYSX_SHAPE));
		//entityInProgress->setAttribute(PHYSIC_RADIUS, getDefaultValue(GEN_ENEMY_PHYSX_RADIUS));
		//entityInProgress->setAttribute(PHYSIC_HEIGHT, getDefaultValue(GEN_ENEMY_PHYSX_HEIGHT));
		//entityInProgress->setAttribute(PHYSIC_CONTACT_FUNCTION,std::to_string(onContactFunction::ENEMY));
		entityInProgress->setAttribute("physic_entity", "rigid");
		entityInProgress->setAttribute("physic_type", "dynamic");
		entityInProgress->setAttribute("physic_shape", "sphere");
		entityInProgress->setAttribute("physic_mass", "10");
		entityInProgress->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_ENEMY_PHYSX_RADIUS));
		entityInProgress->setAttribute(COMMON_MAXROLL, getDefaultValue(GEN_ENEMY_MAXROLL));
		entityInProgress->setAttribute(COMMON_ROLLSPEED, getDefaultValue(GEN_ENEMY_ROLL_SPEED));
		entityInProgress->setAttribute(COMMON_ACCEL, getDefaultValue(GEN_ENEMY_ACCEL));
		entityInProgress->setAttribute(COMMON_ROTATION_ACCEL, getDefaultValue(GEN_ENEMY_ROTATION_ACCEL));
        entityInProgress->setAttribute(COMMON_LIFE, getDefaultValue(GEN_ENEMY_LIFE));
		entityInProgress->setAttribute(AI_TOLERANCE, getDefaultValue(GEN_ENEMY_MOV_TOLERANCE));
		entityInProgress->setAttribute(AI_BEHAVIOR,"dummy");

		entityInProgress->setAttribute("perception_entity_type","enemy");
		
		entityInProgress->setAttribute(COMMON_PRIMARY_WEAPON, "laser");
		entityInProgress->setAttribute(COMMON_SECONDARY_WEAPON, "missile_linear");

		entityList.push_back(entityInProgress);

		//Steering Enemy
		entityInProgress = new CMapEntity("SteeringEnemy"/*+'0'*/);//The commented 0 is for sound events

		entityInProgress->setType(getDefaultValue(GEN_ENEMY_TYPE));

		entityInProgress->setAttribute(GRAPHIC_STATIC, getDefaultValue(GEN_ENEMY_GRAPHIC_STATIC));
		entityInProgress->setAttribute(MINIMAP_ENTITY_SCALE, getDefaultValue(GEN_ENEMY_MINIMAP_SCALE));
		entityInProgress->setAttribute(MINIMAP_ENTITY_ENEMY, getDefaultValue(GEN_ENEMY_MINIMAP_ENEMY));
		//entityInProgress->setAttribute(COMMON_POSITION, getDefaultValue(GEN_ENEMY_START_POSITION));
		entityInProgress->setAttribute(COMMON_POSITION, "{1000, -300, 1000}");
		entityInProgress->setAttribute(COMMON_ORIENTATION, getDefaultValue(GEN_ENEMY_START_ORIENTATION));
		entityInProgress->setAttribute(COMMON_MAX_SPEED, getDefaultValue(GEN_ENEMY_MAX_SPEED));
		entityInProgress->setAttribute(COMMON_ROTATION_SPEED, getDefaultValue(GEN_ENEMY_ROTATION_SPEED));
		entityInProgress->setAttribute(GRAPHIC_MODEL, getDefaultValue(GEN_ENEMY_MODEL));
		//entityInProgress->setAttribute(PHYSIC_ENTITY, getDefaultValue(GEN_ENEMY_PHYSX_SHAPE));
		//entityInProgress->setAttribute(PHYSIC_RADIUS, getDefaultValue(GEN_ENEMY_PHYSX_RADIUS));
		//entityInProgress->setAttribute(PHYSIC_HEIGHT, getDefaultValue(GEN_ENEMY_PHYSX_HEIGHT));
		//entityInProgress->setAttribute(PHYSIC_CONTACT_FUNCTION,std::to_string(onContactFunction::ENEMY));
		entityInProgress->setAttribute("physic_entity", "rigid");
		entityInProgress->setAttribute("physic_type", "dynamic");
		entityInProgress->setAttribute("physic_shape", "sphere");
		entityInProgress->setAttribute("physic_mass", "10");
		entityInProgress->setAttribute(PHYSIC_RADIUS,  getDefaultValue(GEN_ENEMY_PHYSX_RADIUS));
		entityInProgress->setAttribute(COMMON_MAXROLL, getDefaultValue(GEN_ENEMY_MAXROLL));
		entityInProgress->setAttribute(COMMON_ROLLSPEED, getDefaultValue(GEN_ENEMY_ROLL_SPEED));
		entityInProgress->setAttribute(COMMON_ACCEL, getDefaultValue(GEN_ENEMY_ACCEL));
		entityInProgress->setAttribute(COMMON_ROTATION_ACCEL, getDefaultValue(GEN_ENEMY_ROTATION_ACCEL));
        entityInProgress->setAttribute(COMMON_LIFE, getDefaultValue(GEN_ENEMY_LIFE));
		entityInProgress->setAttribute(AI_TOLERANCE, getDefaultValue(GEN_ENEMY_MOV_TOLERANCE));

		entityInProgress->setAttribute("perception_entity_type","enemy");

		entityInProgress->setAttribute(COMMON_PRIMARY_WEAPON, "laser");
		entityInProgress->setAttribute(COMMON_SECONDARY_WEAPON, "missile_linear");
        
		//(Enemy) Example of sound loading
		/*tmpStr1 = "loop0";
		tmpStr2 = "true";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "loop1";
		tmpStr2 = "true";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "autoPlay0";
		tmpStr2 = "true";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "autoPlay1";
		tmpStr2 = "true";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "minDist0";
		tmpStr2 = "40.0";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "maxDist0";
		tmpStr2 = "600.0";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "minDist1";
		tmpStr2 = "40.0";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "maxDist1";
		tmpStr2 = "600.0";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "soundFile0";
		tmpStr2 = "SoundIntro1.wav";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "soundFile1";
		tmpStr2 = "SoundIntro2.wav";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "soundName0";
		tmpStr2 = "EnemySound";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = "soundName1";
		tmpStr2 = "EnemySecSound";
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());*/

		entityList.push_back(entityInProgress);

		//Camera
        entityInProgress = new CMapEntity(getDefaultValue(GEN_CAMERA_NAME));
        entityInProgress->setType(CAMERA_TYPE);

        entityInProgress->setAttribute(CAMERA_NAME,        getDefaultValue(GEN_CAMERA_NAME));
        entityInProgress->setAttribute(CAMERA_NEAR,        getDefaultValue(GEN_CAMERA_NEAR));
        entityInProgress->setAttribute(CAMERA_FAR,         getDefaultValue(GEN_CAMERA_FAR));
        entityInProgress->setAttribute(CAMERA_FOV,         getDefaultValue(GEN_CAMERA_FOV));
        entityInProgress->setAttribute(CAMERA_POSITION,    getDefaultValue(GEN_CAMERA_POSITION));
        entityInProgress->setAttribute(CAMERA_ORIENTATION, getDefaultValue(GEN_CAMERA_ORIENTATION));
        entityInProgress->setAttribute(CAMERA_HEIGHT,      getDefaultValue(GEN_CAMERA_HEIGHT));
		entityInProgress->setAttribute(CAMERA_DIST_CONSTANT,      getDefaultValue(GEN_CAMERA_DIST_CONSTANT));
		entityInProgress->setAttribute(CAMERA_MIN_DISTANCE,      getDefaultValue(GEN_CAMERA_MIN_DISTANCE));

		entityList.push_back(entityInProgress);

		//Texture Camera
//#ifndef _DEBUG
		entityInProgress = new CMapEntity(getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_ENTITYNAME));
		entityInProgress->setType(getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_ENTITYTYPE));

		entityInProgress->setAttribute(STATIC_CAMERA_NAME,			getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_NAME));
		entityInProgress->setAttribute(STATIC_CAMERA_NEAR,			getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_NEAR));
		entityInProgress->setAttribute(STATIC_CAMERA_FAR,			getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_FAR));
		entityInProgress->setAttribute(TEXTURE_CAMERA_ORTHO,		getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_ORTHO));
		entityInProgress->setAttribute(TEXTURE_CAMERA_ORTHO_HEIGHT,	getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_ORTHO_WIDTH));
		entityInProgress->setAttribute(TEXTURE_CAMERA_ORTHO_WIDTH,	getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_ORTHO_HEIGHT));
		entityInProgress->setAttribute(COMMON_POSITION,				getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_POSITION));
        entityInProgress->setAttribute(STATIC_CAMERA_LOOKAT,		getDefaultValue(GEN_PLANET_TEXTURE_CAMERA_LOOKAT));
		entityInProgress->setAttribute(CAMERA_ACCEL,				getDefaultValue(GEN_CAMERA_ACCEL_CONST));

		entityList.push_back(entityInProgress);
//#endif
		//Skybox
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_SKYBOX_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_SKYBOX_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		entityList.push_back(entityInProgress);

		//Planet
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_NAME));

		entityInProgress->setType(getDefaultValue(GEN_PLANET_TYPE));
		entityInProgress->setAttribute(GRAPHIC_STATIC, getDefaultValue(GEN_PLANET_STATIC));
		entityInProgress->setAttribute(COMMON_POSITION, getDefaultValue(GEN_PLANET_POSITION));
		entityInProgress->setAttribute(COMMON_ORIENTATION, getDefaultValue(GEN_PLANET_ORIENTATION));
		tmpStr1 = GRAPHIC_SCALE;
		scale *= Common::Configuration::defaultValue<float>(GEN_PLANET_SCALE_MULT);
		scale += Common::Configuration::defaultValue<float>(GEN_PLANET_SCALE_ADD);
		tmpStr2 = std::to_string(scale);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		entityInProgress->setAttribute(COMMON_ROLL, getDefaultValue(GEN_PLANET_ROLL).c_str() );

		tmpStr1 = GRAPHIC_MODEL;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_MODELS[model]);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		entityList.push_back(entityInProgress);

		//Directional Light
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLDIRLIGHT_NAME));
		
		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLDIRLIGHT_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLDIRLIGHT_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLDIRLIGHT_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_DIFFUSE_COLOUR;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLDIRLIGHT_DIFFUSE);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_SPECULAR_COLOUR;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLDIRLIGHT_SPECULAR);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		entityList.push_back(entityInProgress);

		//Ambient Light
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLAMBLIGHT_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLAMBLIGHT_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_COLOUR_VALUE;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLAMBLIGHT_COLOUR);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		entityList.push_back(entityInProgress);

		//Planet Trigger
		
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_TRIGGER_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_TRIGGER_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_TRIGGER_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_TRIGGER_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","sphere");
		entityInProgress->setAttribute(PHYSIC_RADIUS,std::to_string(std::atof(Common::Configuration::getDefaultValue(GEN_PLANET_TRIGGER_RADIUS).c_str()) * 
			Common::Configuration::defaultValue<float>(GEN_PLANET_SCALE_MULT)));
		entityInProgress->setAttribute("physic_trigger","true");
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_TRIGGER_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_RADIUS;
		tmpStr2 = std::to_string(std::atof(Common::Configuration::getDefaultValue(GEN_PLANET_TRIGGER_RADIUS).c_str()) * 
			Common::Configuration::defaultValue<float>(GEN_PLANET_SCALE_MULT));
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_TRIGGER_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/
		entityList.push_back(entityInProgress);

		// Planet Scene Limit Triggers

		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER1_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER1_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER1_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER1_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER1_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_DIMENSION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER1_DIMENSIONS);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER1_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/

		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","box");
		entityInProgress->setAttribute("physic_dimensions",Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER1_DIMENSIONS));
		entityInProgress->setAttribute("physic_trigger","true");

		entityList.push_back(entityInProgress);
		//
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER2_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER2_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER2_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER2_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER2_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_DIMENSION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER2_DIMENSIONS);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER2_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/
		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","box");
		entityInProgress->setAttribute("physic_dimensions",Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER2_DIMENSIONS));
		entityInProgress->setAttribute("physic_trigger","true");
		entityList.push_back(entityInProgress);
		//
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER3_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER3_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER3_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER3_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER3_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_DIMENSION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER3_DIMENSIONS);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER3_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/
		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","box");
		entityInProgress->setAttribute("physic_dimensions",Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER3_DIMENSIONS));
		entityInProgress->setAttribute("physic_trigger","true");
		entityList.push_back(entityInProgress);
		//
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER4_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER4_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER4_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER4_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER4_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_DIMENSION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER4_DIMENSIONS);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER4_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/
		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","box");
		entityInProgress->setAttribute("physic_dimensions",Common::Configuration::getDefaultValue(GEN_PLANET_LIMIT_TRIGGER4_DIMENSIONS));
		entityInProgress->setAttribute("physic_trigger","true");
		entityList.push_back(entityInProgress);
		
		// Scene End Triggers
		
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER1_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER1_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER1_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER1_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER1_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_DIMENSION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER1_DIMENSIONS);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER1_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/
		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","box");
		entityInProgress->setAttribute("physic_dimensions",Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER1_DIMENSIONS));
		entityInProgress->setAttribute("physic_trigger","true");
		entityList.push_back(entityInProgress);
		//
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER2_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER2_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER2_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER2_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER2_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_DIMENSION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER2_DIMENSIONS);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER2_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/
		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","box");
		entityInProgress->setAttribute("physic_dimensions",Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER2_DIMENSIONS));
		entityInProgress->setAttribute("physic_trigger","true");
		entityList.push_back(entityInProgress);
		//
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER3_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER3_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER3_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER3_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER3_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_DIMENSION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER3_DIMENSIONS);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER3_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/
		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","box");
		entityInProgress->setAttribute("physic_dimensions",Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER3_DIMENSIONS));
		entityInProgress->setAttribute("physic_trigger","true");
		entityList.push_back(entityInProgress);
		//
		entityInProgress = new CMapEntity(Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER4_NAME));

		tmpStr1 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER4_TYPE);
		entityInProgress->setType(tmpStr1.c_str());

		tmpStr1 = COMMON_POSITION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER4_POSITION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = COMMON_ORIENTATION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER4_ORIENTATION);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		/*
		tmpStr1 = PHYSIC_ENTITY;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER4_ENTITY);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_DIMENSION;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER4_DIMENSIONS);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());

		tmpStr1 = PHYSIC_TRIGGER;
		tmpStr2 = Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER4_ISTRIGGER);
		entityInProgress->setAttribute(tmpStr1.c_str(),tmpStr2.c_str());
		*/
		entityInProgress->setAttribute(PHYSIC_ENTITY,"rigid");
		entityInProgress->setAttribute("physic_type","static");
		entityInProgress->setAttribute("physic_shape","box");
		entityInProgress->setAttribute("physic_dimensions",Common::Configuration::getDefaultValue(GEN_PLANET_END_TRIGGER4_DIMENSIONS));
		entityInProgress->setAttribute("physic_trigger","true");
		entityList.push_back(entityInProgress);
		
		return entityList;
	}

#pragma warning( pop )

	bool CMapGenerator::checkDist(const std::vector<Ogre::Vector3>& vec,const Ogre::Vector3& pos,float dist, int vecNum)
	{
		if(!vec.empty()){
			for(unsigned int i=0;i<vec.size();++i){
				if(pos.distance(vec[i]) < dist && i != vecNum)
					return false;
			}
		}
		return true;
	}

}