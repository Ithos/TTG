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
	ser percibidas (o ambas cosas) dentro del gestor de percepci�n.
	Cada entidad de percepci�n tiene una lista de sensores, que le da la capacidad
	de percibir, y de se�ales, que permiten a otras entidades percibirla.
	
	En cada tick de IA el gestor recorrer� los sensores de la entidad y comprobar� si
	alguno de ellos se activa. Por otra parte, recorrer� las se�ales asociadas a la
	entidad comprobando tambi�n si activan alguno de los sensores de las dem�s entidades.
	
	El campo userData permite asociar a la entidad de percepci�n con cualquier dato
	que pueda ser �til durante el proceso de percepci�n (en general nos interesar�
	asociarla a la entidad l�gica).
	
	La entidad de percepci�n tiene una matriz de transformaci�n que ser� utilizada
	por los sensores que as� lo requieran para obtener datos como la posici�n o la
	orientaci�n. Esta matriz tiene que ser actualizada de manera expl�cita (en
	general, por la entidad l�gica a la que est� asociada la entidad de percepci�n).
	*/
	class CPerceptionEntity
	{
	public:
		/*
		Constructor
		@param userData Datos de usuario asociados a la entidad de percepci�n
		@param listener Clase que ser� notificada cuando la entidad de percepci�n reciba una notificaci�n
		@param radius Radio de la entidad de percepci�n
		*/
		CPerceptionEntity(void* userData, IPerceptionListener* listener) : m_userData(userData), m_listener(listener) {};

		/*
		El destructor se responsabiliza de eliminar el contenido de las listas de sensores y de se�ales de la entidad
		*/
		~CPerceptionEntity();

		/*
		Devuelve los datos de usuario asociados a la entidad de percepci�n
		*/
		void* getUserData() {return m_userData;};

		/*
		Devuelve la lista de sensores de la entidad
		*/
		const std::list<CSensor*>& getSensors() { return m_sensors; };

		/*
		Devuelve la lista de se�ales de la entidad
		*/
		const std::list<CPerceptionSignal*>& getSignals() { return m_signals; };

		/*
		A�ade un sensor a la entidad
		*/
		void addSensor(CSensor* sensor) { m_sensors.push_back(sensor); };

		/*
		A�ade una se�al a la entidad
		*/
		void addSignal(CPerceptionSignal* signal) { m_signals.push_back(signal); };

		/*
		Elimina un sensor de la entidad
		*/
		void removeSensor(CSensor* sensor);

		/*
		Elimina una se�al de la entidad
		*/
		void removeSignal(CPerceptionSignal* signal);

		/*
		Establece el valor del flag active para todos los sensores
		*/
		void setSensorsActive(bool active);

		/*
		Establece el valor del flag active para todas las se�ales
		*/
		void setSignalsActive(bool active);

		/*
		Env�a una notificaci�n a su listener
		*/
		void sendNotification(CNotification* notification) { m_listener->notificationPerceived(notification); };

		/*
		Actualiza la matriz de transformaci�n de la entidad de percepci�n
		*/
		void setTransform(const Matrix4& transform) { m_transform = transform; };

		/*
		Devuelve la matriz de transformaci�n de la entidad de percepci�n
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
		Lista de se�ales
		*/
		std::list<CPerceptionSignal*> m_signals;

		/*
		Clase que ser� notificada cuando alguno de los sensores se active
		*/
		IPerceptionListener* m_listener;

		/*
		Matriz de transformaci�n
		*/
		Matrix4 m_transform;
	};

}

#endif
