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

#ifndef __AI_PERCEPTIONMANAGER_H
#define __AI_PERCEPTIONMANAGER_H

#include "Common/Util/Math.h"

namespace AI 
{

	class CPerceptionEntity;
	class CSensor;

	/*
	Tipos de percepción
	*/
	enum EnmPerceptionType 
	{
		PERCEPTION_UNKNOWN,	// Tipo desconocido
		PERCEPTION_SIGHT,	// Vista
		PERCEPTION_HEAR		// Oído
	};



	/*
	Clase que representa una notificación que hace el gestor de percepción a una
	entidad de percepción cuyo sensor ha percibido una señal.
	*/
	class CNotification
	{
	public:
		CNotification (unsigned long time, CSensor* sensor, CPerceptionEntity* entity) :
			m_time(time), m_sensor(sensor), m_perceivedEntity(entity) {}

		unsigned long getTime() { return m_time; }
		CSensor* getSensor() { return m_sensor; }
		CPerceptionEntity* getPerceivedEntity() { return m_perceivedEntity; }

	private:
		long m_time;
		CSensor* m_sensor;
		CPerceptionEntity* m_perceivedEntity;

	};



	/*
	Clase auxiliar que compara dos notificaciones usando su tiempo de entrega.
	Se utiliza para mantener ordenada la lista de notificaciones pendientes
	*/
	class CNotificationComparator 
	{
	public:
		bool operator() (CNotification* a, CNotification* b) { return a->getTime() > b->getTime(); };
	};



	/*
	Clase gestora de la percepción. En esta clase se registran las entidades interesadas
	en percibir/ser percibidas. La clase se encarga de ordenar las comprobaciones necesarias
	cada tick para que se lleve a cabo la percepción y de enviar las notificaciones a los
	sensores correspondientes cuando han percibido algo.
	*/
	class CPerceptionManager
	{
	public:
		/*
		Constructor por defecto
		*/
		CPerceptionManager(): m_Time(0.0f) {};
		
		/*
		Destructor por defecto
		*/
		~CPerceptionManager() {};

		/*
		El método update se encarga de actualizar la información sobre los sensores de
		cada entidad de percepción.
		*/
		void update(unsigned int msecs);

		/*
		Registra una entidad de percepción en el gestor. Una vez que la entidad está registrada, se
		pueden realizar comprobaciones sobre sus sensores y señales y puede recibir notificaciones.
		@param entity Entidad de percepción que se va a registrar.
		*/
		void registerEntity(CPerceptionEntity* entity) { m_entities.push_back(entity); };

		/*
		Elimina una entidad de percepción de la lista de entidades gestionadas. Esto evita que se
		realicen las comprobaciones de percepción sobre sus sensores y señales y que reciba
		notificaciones.
		@param entity Entidad de percepción que se va a desregistrar
		*/
		void unregisterEntity(CPerceptionEntity* entity);

	private:
		/*
		Lista de entidades de percepción gestionadas
		*/
		std::list<CPerceptionEntity*> m_entities;
		float m_Time;

		/*
		Lista de notificaciones de percepción pendientes de enviar
		*/
		std::priority_queue<CNotification*, std::vector<CNotification*>, CNotificationComparator> m_notifications;

		/*
		Comprueba los sensores de las entidades registradas
		@param time Instante de tiempo en que se realiza la comprobación
		*/
		void checkPerception(unsigned int time);

		/*
		Recorre la lista de notificaciones pendientes y envía las que hayan caducado
		@param time Instante de tiempo en que se realiza la comprobación
		*/
		void notifyEntities(unsigned int time);
	};

}

#endif
