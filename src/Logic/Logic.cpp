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

#include "Logic.h"

#include <log.h>

#include "EntityFactory.h"
#include "Scene/Scene.h"

#include <Common/Map/Map.h>

namespace Logic
{
	const char* const LOG_CLOGIC = "Logic::CLogic";
	CLogic* CLogic::m_instance = 0;

	CLogic::CLogic(): m_scene(nullptr), m_lastFilename(""), m_root(nullptr), m_window(nullptr), m_lastSceneType(Common::Data::DUMMY)
	{
		m_instance = this;
	}

	CLogic::~CLogic()
	{
		m_instance = 0;
	}

	bool CLogic::init()
	{
		if(m_instance == 0){
			new CLogic();
			if(!m_instance->open()){
				release();
				return false;
			}
			return true;
		}
		return false;
	}

	void CLogic::release()
	{
		if(m_instance){
			m_instance->close();
			delete m_instance;
		}
	}

	void CLogic::tick(unsigned int msecs)
	{
		CEntityFactory::getInstance()->deleteDeferredEntities();
		m_scene->tick(msecs);
	}

	bool CLogic::activateScene()
	{
		if(m_scene)
			return m_scene->activate();
		else return false;
	}

	void CLogic::deactivateScene()
	{
		if(m_scene)
			m_scene->deactivate();
	}

	bool CLogic::loadLevel(const std::string &filename,
		Ogre::Root* root,
		Ogre::RenderWindow* render,
		Common::Data::SceneType type)
	{
		m_lastFilename = filename;
		m_root = root;
		m_window = render;
		m_lastSceneType = type;

		unloadLevel();
		log_trace(LOG_CLOGIC,"Loading level...\n");
		if(m_scene = CScene::createSceneFromFile(filename,root,render,type)){
			if(type == Common::Data::SceneType::DUMMY)
				m_scene->activate();
			return true;
		}
		return false;
	}

	void CLogic::unloadLevel()
	{
		if(m_scene){
			m_scene->deactivate();
			delete m_scene;
			m_scene = 0;
		}
	}

	void CLogic::reloadLevel()
	{
		loadLevel(m_lastFilename, m_root, m_window, m_lastSceneType);
		m_scene->activate();
	}

	bool CLogic::open()
	{
		if(!Map::CMap::init()){
			return false;
		}
		if(!CEntityFactory::init()){
			return false;
		}
		return true;
	}

	void CLogic::close()
	{
		unloadLevel();
		CEntityFactory::release();
		Map::CMap::release();
	}
}