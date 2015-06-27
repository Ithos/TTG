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

#include "EntityFactory.h"

#include <log.h>

#include <fstream>

#include <Common/Configure/Configure.h>
#include <Common/Util/XMLParser/XmlParser.h>
#include <Common/Map/MapEntity.h>

#include "ComponentFactory.h"
#include "Scene/Scene.h"

#include "Entity/Components/Component.h"
#include "Entity/Components/Cameras/StaticCamera.h"
#include "Entity/Components/Cameras/RTTCamera.h"
#include "Entity/Components/Graphic/Graphics.h"
#include "Entity/Components/Minimap/MinimapHighlight.h"
#include "Entity/Components/Lights/PointLight.h"
#include "Entity/Components/Lights/AmbientLight.h"
#include "Entity/Components/Lights/DirectionalLight.h"
#include "Entity/Components/Movement/Transform.h"
#include "Entity/Components/Graphic/SkyBox.h"
#include "Entity/Components/Cameras/Camera.h"
#include "Entity/Components/Movement/Controller.h"
#include "Entity/Components/System/Rotation.h"
#include "Entity/Components/Movement/PlanetRotation.h"
#include "Entity/Components/AI/SteeringMovement.h"
#include "Entity/Components/AI/StateMachineExecutor.h"
#include "Entity/Components/AI/PerceptionComponent.h"
#include "Entity/Components/System/OrbitMovement.h"
#include "Entity/Components/Gameplay/Shield.h"
#include "Entity/Components/Sound/Listener.h"

//Sound
#include "Entity/Components/Sound/Listener.h"
#include "Entity/Components/Sound/Emitter.h"
#include "Entity/Components/Sound/PropertyManager.h"

//Physic
#include "Entity/Components/Physic/PhysicEntity.h"
#include "Entity/Components/Physic/PhysicContact.h"
#include "Entity/Components/Triggers/PlanetTrigger.h"
#include "Entity/Components/Triggers/SceneLimitTrigger.h"
#include "Entity/Components/Triggers/SceneEndTrigger.h"
#include "Entity/Components/Triggers/ShieldTrigger.h"
#include "Entity/Components/Triggers/MissileTrigger.h"

//Console
//#include "Entity/Components/Graphic/GraphicPrimitive.h"

std::istream& operator>>(std::istream& is, Logic::CEntityFactory::TBluePrint& blueprint) 
{
	is >> blueprint.type;
	std::string aux;
	getline(is,aux,'\n');
	std::istringstream components(aux);
	while(!components.eof())
	{
		aux.clear();
		components >> aux;
		if(!aux.empty())
			blueprint.components.push_back(aux);
	}
	return is;
}

namespace Logic
{
	const char* const LOG_CENTITYFACTORY = "Logic::CEntityFactory";

	CEntityFactory* CEntityFactory::m_instance = 0;

	TEntityID CEntityFactory::m_nextId = FIRST_ID;

	CEntityFactory::CEntityFactory()
	{
		m_instance = this;
	}

	CEntityFactory::~CEntityFactory()
	{
		m_instance = 0;
	}

	bool CEntityFactory::init()
	{
		if(m_instance) return false;

		new CEntityFactory();

		if(!m_instance->open()){
			release();
			return false;
		}
		return true;
	}

	void CEntityFactory::release()
	{
		if(m_instance){
			m_instance->close();
			delete m_instance;
		}
	}

	bool CEntityFactory::loadBluePrints(const std::string &filename)
	{
		using std::ifstream;
		using std::string;
		Common::Configuration::setDefaultFile(CONFIGURE_FILE);
		string path = Common::Configuration::getDefaultValue(BLUEPRINTS_PATH);
		string completePath = path + filename;
		//string completePath = path + "blueprints.xml";

		ifstream in(completePath.c_str());
		if(!in) {
			log_error(LOG_CENTITYFACTORY,"Error blueprints file not found\n");
			return false;
		}

        std::string line("");
        getline(in, line);

        //XML
        if ( strstr(line.c_str(), "xml") ) {
			log_trace(LOG_CENTITYFACTORY, "Parsing xml file...\n");
			in.close();
			m_xmlBlueprints = Common::Util::XMLParser::CXMLParser::loadBluePrints(completePath);
			if (m_xmlBlueprints.empty()) {
                log_error(LOG_CENTITYFACTORY,"Error while parsing %s\n", filename.c_str());
				return false;
			}
		} 
        else { // TXT
            // this part of code shouldn't be executed anymore
			while (in){
				TBluePrint b;
				in >> b;
				if(!b.type.empty()){
					if(m_blueprints.count(b.type)){
						m_blueprints.erase(b.type);
					}
					m_blueprints.insert(std::pair<string,TBluePrint>(b.type,b));
				}
			}
            in.close();
		}
		return true;
	}

	void CEntityFactory::unloadBluePrints()
	{
		m_blueprints.clear();
	}

	CEntity* CEntityFactory::createEntity(const Map::CMapEntity *entityInfo, CScene *scene)
	{
		CEntity* ret = assembleEntity(entityInfo->getType());

		if (!ret) return 0;

        if (scene) {
		    scene->addEntity(ret);

		    if(ret->spawn(scene,entityInfo)) 
                return ret;

            scene->removeEntity(ret);
		    delete ret;
		    return 0;
        }
     
        return ret;
	}

	void CEntityFactory::deleteEntity(CEntity* entity)
	{
		entity->getScene()->removeEntity(entity);
		delete entity;
	}

	void CEntityFactory::deferredDeleteEntity(CEntity* entity)
	{
		m_pendingEntities.push_back(entity);
	}

	void CEntityFactory::deleteDeferredEntities()
	{
		TEntityList::const_iterator it = m_pendingEntities.begin();
		TEntityList::const_iterator end = m_pendingEntities.end();
		while(it != end){
			CEntity* entity = *it;
			it++;
			deleteEntity(entity);
		}

		if(!m_pendingEntities.empty()) m_pendingEntities.clear();
	}

	bool CEntityFactory::open()
	{
		return true;
	}

	void CEntityFactory::close()
	{
		unloadBluePrints();
	}

	CEntity* CEntityFactory::assembleEntity(const std::string &type)
	{
		if(m_xmlBlueprints.empty()){
			TBluePrintMap::const_iterator it (m_blueprints.find(type));

			if(it != m_blueprints.end()){
				CEntity* ent = new CEntity(nextID());

				std::list<std::string>::const_iterator cIt((*it).second.components.begin());

				Component::IComponent* comp;
				for(; cIt != (*it).second.components.end(); cIt++){
				
					if (CComponentFactory::getInstance()->has((*cIt))){
						comp = CComponentFactory::getInstance()->create((*cIt));
					}
                    else {
						// WTF??????????????????
                        std::stringstream str;
                        str << "Line: " << __LINE__ << " || Component '"<< (*cIt) << "' name not found\n";
                        log_fatal(LOG_CENTITYFACTORY, str.str().c_str());
						delete ent;
						return nullptr;
					}
					if(comp){
						comp->setName(*cIt);
						ent->addComponent(comp);
					}
				}
				return ent;
			}
			return nullptr;
		} else {
			log_trace(LOG_CENTITYFACTORY,"AssembleEntity from xml\n");
			
			using Common::Data::Blueprints::TBluePrintsMap;
			using Common::Data::Blueprints::TComponentsVector;
			using Common::Data::Blueprints::TXMLComponent;

			TBluePrintsMap::const_iterator it (m_xmlBlueprints.find(type));
			if(it != m_xmlBlueprints.end()){
				CEntity* ent = new CEntity(nextID());

				std::vector<TXMLComponent>::const_iterator cIt((*it).second.begin());
				Component::IComponent* comp;
				for(; cIt != (*it).second.end(); ++cIt){
					std::string name = (*cIt).name;
					if(CComponentFactory::getInstance()->has((*cIt).name)){
						comp = CComponentFactory::getInstance()->create((*cIt).name);
					} else {
                        log_fatal(LOG_CENTITYFACTORY, "Component %s name not found\n", (*cIt).name.c_str());
						delete ent;
						return nullptr;
					}
					if(comp){
						comp->setName((*cIt).name);
						ent->addComponent(comp,(*cIt).number,(*cIt).priority);
					}
				}

				return ent;
			}
			return nullptr;
		}
	}

	TEntityID CEntityFactory::nextID()
	{
		TEntityID ret = m_nextId;
		if(ret == UNASSIGNED){
			log_fatal(LOG_CENTITYFACTORY,"No Entity ID!\n");
		}
		m_nextId++;
		return m_nextId;
	}

}