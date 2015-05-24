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

#ifndef __LOGIC_PERCEPTIONCOMPONENT_H
#define __LOGIC_PERCEPTIONCOMPONENT_H

#include "Logic/Entity/Components/Component.h"
#include "AI/Perception/PerceptionListener.h"

namespace AI {
	class CPerceptionEntity;
}

namespace Logic
{
	namespace Component
	{
		/*
		Este componente se encarga de las tareas relacionadas con la percepción.
		Al cargar las propiedades del mapa, se ocupa de crear una entidad de percepción
		(AI::CPerceptionEntity), que estará asociada a la entidad lógica. En esta entidad
		de percepción se definen los sensores que posee la entidad y las señales que
		genera en cada tick.
		La entidad de percepción se registra en el gestor de percepción (AI::CPerceptionManager)
		para poder percibir y ser percibida.
		Cuando uno de los sensores de la entidad percibe una señal apropiada, se invoca
		al método notificationPerceived que, en este caso, envía un mensaje con los
		detalles de la percepción realizada.
		*/
		class CPerceptionComponent: public IComponent, public AI::IPerceptionListener
		{
			DEC_FACTORY(CPerceptionComponent);

		public:
			/*
			Constructor
			*/
			CPerceptionComponent(void) : m_pEntity(NULL) {};

			/*
			Destructor
			*/
			~CPerceptionComponent(void);

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
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

			/*
			Método que activa el componente; invocado cuando se activa
			el mapa donde está la entidad a la que pertenece el componente.
			En este caso, se registra el componente en el gestor de percepción (AI::PerceptionManager)
			@return true si todo ha ido correctamente.
			*/
			virtual bool activate();

			/*
			Método que desactiva el componente; invocado cuando se
			desactiva el mapa donde está la entidad a la que pertenece el
			componente. Se invocará siempre, independientemente de si estamos
			activados o no.
			Al desactivar el componente la entidad de percepción se "desregistra"
			del gestor de percepción.
			*/
			virtual void deactivate();

			/*
			Método llamado en cada frame que actualiza el estado del componente.
			En cada tick se comprueba si la entidad lógica ha modificado su matriz de transformación
			y, si es así, se actualiza la matriz de transformación de la entidad de percepción.
			@param msecs Milisegundos transcurridos desde el último tick.
			*/
			virtual void tick(unsigned int msecs);

			/*
			Método invocado por el gestor de percepción cuando recibe una notificación de un
			sensor de la entidad de percepción.
			En este método se incluirán las acciones que queremos que el componente realice cuando
			alguno de los sensores asociados percibe una señal.
			@param notification Notificación recibida
			*/
			virtual void notificationPerceived(AI::CNotification* notification);

		private:
			/*
			Entidad de percepción que se registra en el CPerceptionManager
			*/
			AI::CPerceptionEntity* m_pEntity;

		};

		REG_FACTORY(CPerceptionComponent);
	}
}

#endif
