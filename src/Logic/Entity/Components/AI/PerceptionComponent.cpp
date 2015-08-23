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

#include "PerceptionComponent.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Logic.h"
#include "Common/Map/MapEntity.h"

#include <Common/Configure/Configure.h>
#include <Common/Map/MapEntity.h>

#include "AI/Perception/PerceptionSignal.h"
#include "AI/Perception/PerceptionEntity.h"
#include "AI/Perception/Sensor.h"
#include "AI/AI.h"
#include "AI/Perception/PerceptionEntityFactory.h"

#include "Logic/Entity/Components/Movement/Transform.h"
#include "Logic/Entity/Components/AI/SteeringMovement.h"
#include "Common/Data/TTG_Types.h"

#include "GUI\GUI.h"
#include "GUI\Controllers\PlayerController.h"

#include <Application/States/State GUI/PlanetGUI.h>

#include <log.h>
#include <OgreStringConverter.h>

namespace Logic 
{
	namespace Component
	{
		const float MARGIN = 100.0f;
		const float MAX_TIME = 5.00f;

		IMP_FACTORY(CPerceptionComponent);
	
		/*
		Destructor
		*/
		CPerceptionComponent::~CPerceptionComponent(void)
		{
			// Eliminamos la entidad de percepción que hemos creado en el spawn
			if (m_pEntity)
				delete m_pEntity;
		}

		/*
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity).
		En el spawn, se crea una entidad de percepción (AI::CPerceptionEntity) y se
		registra en el gestor de percepción (AI::CPerceptionManager).
		En este caso utilizaremos los atributos:
		· perception_entity_type: tipo de la entidad de percepción. Es una manera
		rápida y sencilla de definir qué sensores y señales tiene la entidad y cuáles
		son sus propiedades, pero no es muy flexible.
		· physic_radius: utilizamos el radio de la cápsula física como radio de la entidad
		de percepción.
		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		bool CPerceptionComponent::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
		{
			if(!IComponent::spawn(entity,scene,entityInfo))
				return false;

			// Crear una nueva entidad de percepción y añadir los sensores y señales permanentes
			// En este caso utilizamos una factoría de entidades de percepción, a la que le pasamos el
			// tipo de entidad que leemos del mapa
			if(entityInfo->hasAttribute("perception_entity_type")) {
				std::string pEType = entityInfo->getStringAttribute("perception_entity_type");
				m_pEntity = AI::CPerceptionEntityFactory::getPerceptionEntity(pEType,m_entity,this);
			}

			return true;
		}

		/*
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		En este caso, se registra el componente en el gestor de percepción (AI::PerceptionManager)
		@return true si todo ha ido correctamente.
		*/
		bool CPerceptionComponent::activate()
		{
			if(!IComponent::activate())
				return false;
			// Obtenemos la matriz de transformación inicial de la entidad asociada y se la
			// pasamos a la entidad de percepción
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP)); 
			m_pEntity->setTransform(transf->getTransform());
			// Y registramos la entidad de percepción en el gestor
			AI::CAI::getInstance()->getPerceptionManager()->registerEntity(m_pEntity);

			return true;
		}

		/*
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		Al desactivar el componente la entidad de percepción se "desregistra"
		del gestor de percepción.
		*/
		void CPerceptionComponent::deactivate()
		{
			IComponent::deactivate();

			if(playerSeen)
				Application::CPlanetGUI::getInstance()->releaseLock();

			// Desregistramos la entidad de percepción en el gestor
			AI::CAI::getInstance()->getPerceptionManager()->unregisterEntity(m_pEntity);
		}

		/*
		Método llamado en cada frame que actualiza el estado del componente.
		En cada tick se comprueba si la entidad lógica ha modificado su matriz de transformación
		y, si es así, se actualiza la matriz de transformación de la entidad de percepción.
		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void CPerceptionComponent::tick(unsigned int msecs)
		{
		
			if(m_Time < MAX_TIME)
			{
				m_Time += msecs / 1000.0f;
			}

			IComponent::tick(msecs);

			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP)); 
			m_pEntity->setTransform(transf->getTransform());
		}

		/*
		Método invocado por el gestor de percepción cuando recibe una notificación de un
		sensor de la entidad de percepción.
		En este método se incluirán las acciones que queremos que el componente realice cuando
		alguno de los sensores asociados percibe una señal.
		@param notification Notificación recibida
		*/
		void CPerceptionComponent::notificationPerceived(AI::CNotification* notification)
		{
			CEntity* entity = (CEntity*) notification->getPerceivedEntity()->getUserData();
			CTransform* transf = static_cast<CTransform*>(entity->getComponentByName(Common::Data::TRANSFORM_COMP)); 
			CSteeringMovement* steering = static_cast<CSteeringMovement*>(m_entity->getComponentByName(Common::Data::STEERING_MOV));

			if (entity->isPlayer()) {

				//This is here beacause of an awfull bug
				if(m_Time < MAX_TIME){
					Common::Configuration::setDefaultFile(CONFIGURE_FILE);
					Common::Configuration::setDefaultFile(Common::Configuration::getDefaultValue(CONF_GENERATOR_PATH).c_str());

					Map::CMapEntity* entityInfo(new Map::CMapEntity(""));
					entityInfo->setAttribute("pos", Common::Configuration::getDefaultValue(GEN_PLAYER_START_POSITION));
					Vector3 playerInitPos(entityInfo->getVector3Attribute("pos"));
					if(transf->getPosition().x <= playerInitPos.x + MARGIN && transf->getPosition().x >= playerInitPos.x - MARGIN &&
						transf->getPosition().z <= playerInitPos.z + MARGIN && transf->getPosition().z >= playerInitPos.z - MARGIN){
						delete notification;
						notification = nullptr;
						AI::CAI::getInstance()->clearNotifications();
						Application::CPlanetGUI::getInstance()->deactivate();
						GUI::CGUI::getInstance()->getPlayerController()->deactivate();
						entity->deactivate();
						Logic::CLogic::getInstance()->reloadLevel();
						GUI::CGUI::getInstance()->getPlayerController()->activate();
						Application::CPlanetGUI::getInstance()->activate();
						return;
					}
				}

				if (!playerSeen) {
					steering->setPlayerAsTarget();
					playerSeen = true;
					Application::CPlanetGUI::getInstance()->addLock();	
				}
				else
					steering->setEvadePlayer(transf->getPosition());
			}
			else
				steering->setEvadeObstacle(transf->getPosition());

			// El gestor de percepción se desentiende de las notificaciones una vez que las 
			// envía. Es responsabilidad del receptor eliminarlas.
			delete notification;
			notification = nullptr;
		}
	}
}
