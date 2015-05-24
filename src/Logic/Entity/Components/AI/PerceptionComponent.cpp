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
#include "Common/Map/MapEntity.h"

#include "AI/Perception/PerceptionSignal.h"
#include "AI/Perception/PerceptionEntity.h"
#include "AI/Perception/Sensor.h"
#include "AI/AI.h"
#include "AI/Perception/PerceptionEntityFactory.h"

#include "Logic/Entity/Components/Movement/Transform.h"
#include "Common/Data/TTG_Types.h"

namespace Logic 
{
	namespace Component
	{
		IMP_FACTORY(CPerceptionComponent);
	
		/*
		Destructor
		*/
		CPerceptionComponent::~CPerceptionComponent(void)
		{
			// Eliminamos la entidad de percepci�n que hemos creado en el spawn
			if (m_pEntity != NULL)
				delete m_pEntity;
		}

		/*
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity).
		En el spawn, se crea una entidad de percepci�n (AI::CPerceptionEntity) y se
		registra en el gestor de percepci�n (AI::CPerceptionManager).
		En este caso utilizaremos los atributos:
		� perception_entity_type: tipo de la entidad de percepci�n. Es una manera
		r�pida y sencilla de definir qu� sensores y se�ales tiene la entidad y cu�les
		son sus propiedades, pero no es muy flexible.
		� physic_radius: utilizamos el radio de la c�psula f�sica como radio de la entidad
		de percepci�n.
		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		bool CPerceptionComponent::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
		{
			if(!IComponent::spawn(entity,scene,entityInfo))
				return false;

			// Crear una nueva entidad de percepci�n y a�adir los sensores y se�ales permanentes
			// En este caso utilizamos una factor�a de entidades de percepci�n, a la que le pasamos el
			// tipo de entidad que leemos del mapa
			if(entityInfo->hasAttribute("perception_entity_type")) {
				std::string pEType = entityInfo->getStringAttribute("perception_entity_type");
				m_pEntity = AI::CPerceptionEntityFactory::getPerceptionEntity(pEType,m_entity,this);
			}

			return true;
		}

		/*
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		En este caso, se registra el componente en el gestor de percepci�n (AI::PerceptionManager)
		@return true si todo ha ido correctamente.
		*/
		bool CPerceptionComponent::activate()
		{
			// Obtenemos la matriz de transformaci�n inicial de la entidad asociada y se la
			// pasamos a la entidad de percepci�n
			CTransform* transf = static_cast<CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP)); 
			m_pEntity->setTransform(transf->getTransform());
			// Y registramos la entidad de percepci�n en el gestor
			AI::CAI::getInstance()->getPerceptionManager()->registerEntity(m_pEntity);

			return true;
		}

		/*
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		Al desactivar el componente la entidad de percepci�n se "desregistra"
		del gestor de percepci�n.
		*/
		void CPerceptionComponent::deactivate()
		{
			// Desregistramos la entidad de percepci�n en el gestor
			AI::CAI::getInstance()->getPerceptionManager()->unregisterEntity(m_pEntity);
		}

		/*
		M�todo llamado en cada frame que actualiza el estado del componente.
		En cada tick se comprueba si la entidad l�gica ha modificado su matriz de transformaci�n
		y, si es as�, se actualiza la matriz de transformaci�n de la entidad de percepci�n.
		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void CPerceptionComponent::tick(unsigned int msecs)
		{
			IComponent::tick(msecs);
		}

		/*
		M�todo invocado por el gestor de percepci�n cuando recibe una notificaci�n de un
		sensor de la entidad de percepci�n.
		En este m�todo se incluir�n las acciones que queremos que el componente realice cuando
		alguno de los sensores asociados percibe una se�al.
		@param notification Notificaci�n recibida
		*/
		void CPerceptionComponent::notificationPerceived(AI::CNotification* notification)
		{
			CEntity* entity = (CEntity*) notification->getPerceivedEntity()->getUserData();
			CTransform* transf = static_cast<CTransform*>(entity->getComponentByName(Common::Data::TRANSFORM_COMP)); 
			std::cout << m_entity->getName() << " percibe a " << entity->getName() << " en " << transf->getPosition() << std::endl;

			// El gestor de percepci�n se desentiende de las notificaciones una vez que las 
			// env�a. Es responsabilidad del receptor eliminarlas.
			delete notification;
		}
	}
}
