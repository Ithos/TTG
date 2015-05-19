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

#ifndef __AI_PERCEPTIONENTITY_H
#define __AI_PERCEPTIONENTITY_H

#include "PerceptionListener.h"
#include "Common/Util/Math.h"

namespace AI
{

	class CSensor;
	class CPerceptionSignal;

	/*
	La clase CPerceptionEntity representa a las entidades que pueden percibir o
	ser percibidas (o ambas cosas) dentro del gestor de percepción.
	Cada entidad de percepción tiene una lista de sensores, que le da la capacidad
	de percibir, y de señales, que permiten a otras entidades percibirla.
	
	En cada tick de IA el gestor recorrerá los sensores de la entidad y comprobará si
	alguno de ellos se activa. Por otra parte, recorrerá las señales asociadas a la
	entidad comprobando también si activan alguno de los sensores de las demás entidades.
	
	El campo userData permite asociar a la entidad de percepción con cualquier dato
	que pueda ser útil durante el proceso de percepción (en general nos interesará
	asociarla a la entidad lógica).
	
	La entidad de percepción tiene una matriz de transformación que será utilizada
	por los sensores que así lo requieran para obtener datos como la posición o la
	orientación. Esta matriz tiene que ser actualizada de manera explícita (en
	general, por la entidad lógica a la que esté asociada la entidad de percepción).
	*/
	class CPerceptionEntity
	{
	public:
		/*
		Constructor
		@param userData Datos de usuario asociados a la entidad de percepción
		@param listener Clase que será notificada cuando la entidad de percepción reciba una notificación
		@param radius Radio de la entidad de percepción
		*/
		CPerceptionEntity(void* userData, IPerceptionListener* listener) : m_userData(userData), m_listener(listener) {};

		/*
		El destructor se responsabiliza de eliminar el contenido de las listas de sensores y de señales de la entidad
		*/
		~CPerceptionEntity();

		/*
		Devuelve los datos de usuario asociados a la entidad de percepción
		*/
		void* getUserData() {return m_userData;};

		/*
		Devuelve la lista de sensores de la entidad
		*/
		const std::list<CSensor*>& getSensors() { return m_sensors; };

		/*
		Devuelve la lista de señales de la entidad
		*/
		const std::list<CPerceptionSignal*>& getSignals() { return m_signals; };

		/*
		Añade un sensor a la entidad
		*/
		void addSensor(CSensor* sensor) { m_sensors.push_back(sensor); };

		/*
		Añade una señal a la entidad
		*/
		void addSignal(CPerceptionSignal* signal) { m_signals.push_back(signal); };

		/*
		Elimina un sensor de la entidad
		*/
		void removeSensor(CSensor* sensor);

		/*
		Elimina una señal de la entidad
		*/
		void removeSignal(CPerceptionSignal* signal);

		/*
		Establece el valor del flag active para todos los sensores
		*/
		void setSensorsActive(bool active);

		/*
		Establece el valor del flag active para todas las señales
		*/
		void setSignalsActive(bool active);

		/*
		Envía una notificación a su listener
		*/
		void sendNotification(CNotification* notification) { m_listener->notificationPerceived(notification); };

		/*
		Actualiza la matriz de transformación de la entidad de percepción
		*/
		void setTransform(const Matrix4& transform) { m_transform = transform; };

		/*
		Devuelve la matriz de transformación de la entidad de percepción
		*/
		Matrix4& getTransform() { return m_transform; };

	private:
		/*
		Datos de usuario
		*/
		void* m_userData;

		/*
		Lista de sensores
		*/
		std::list<CSensor*> m_sensors;

		/*
		Lista de señales
		*/
		std::list<CPerceptionSignal*> m_signals;

		/*
		Clase que será notificada cuando alguno de los sensores se active
		*/
		IPerceptionListener* m_listener;

		/*
		Matriz de transformación
		*/
		Matrix4 m_transform;
	};

}

#endif
