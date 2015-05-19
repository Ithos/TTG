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

#include "PerceptionEntity.h"

#include "Sensor.h"
#include "PerceptionSignal.h"

namespace AI 
{
	/*
	El destructor se responsabiliza de eliminar el contenido de las listas de sensores y de señales de la entidad
	*/
	CPerceptionEntity::~CPerceptionEntity()
	{
		// Eliminamos los sensores
		for (std::list<CSensor*>::iterator it = m_sensors.begin(); it != m_sensors.end(); it++)
			delete (*it);

		// Y las señales
		for (std::list<CPerceptionSignal*>::iterator it = m_signals.begin(); it != m_signals.end(); it++)
			delete (*it);
	}

	/*
	Elimina un sensor de la entidad
	*/
	void CPerceptionEntity::removeSensor(CSensor* sensor)
	{
		for (std::list<CSensor*>::iterator it = m_sensors.begin(); it != m_sensors.end(); it++)
		{
			if (sensor == (*it))
				m_sensors.erase(it);
			break;
		}
	}

	/*
	Elimina una señal de la entidad
	*/
	void CPerceptionEntity::removeSignal(CPerceptionSignal* signal)
	{
		for (std::list<CPerceptionSignal*>::iterator it = m_signals.begin(); it != m_signals.end(); it++)
		{
			if (signal== (*it))
				m_signals.erase(it);
			break;
		}
	}

	/*
	Establece el valor del flag active para todos los sensores
	*/
	void CPerceptionEntity::setSensorsActive(bool active)
	{
		for (std::list<CSensor*>::iterator it = m_sensors.begin(); it != m_sensors.end(); it++)
		{
			(*it)->setActive(active);
			break;
		}
	}

	/*
	Establece el valor del flag active para todas las señales
	*/
	void CPerceptionEntity::setSignalsActive(bool active)
	{
		for (std::list<CPerceptionSignal*>::iterator it = m_signals.begin(); it != m_signals.end(); it++)
			(*it)->setActive(active);
	}

}
