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

#include "PerceptionManager.h"

#include "Sensor.h"
#include "PerceptionEntity.h"
#include "PerceptionSignal.h"

#include "Application/BaseApplication.h"

namespace AI 
{

	/*
	Elimina una entidad de percepción de la lista de entidades gestionadas. Esto evita que se
	realicen las comprobaciones de percepción sobre sus sensores y señales y que reciba
	notificaciones.
	@param entity Entidad de percepción que se va a desregistrar
	*/
	void CPerceptionManager::unregisterEntity(CPerceptionEntity* entity)
	{
		for (std::list<CPerceptionEntity*>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			if (entity == (*it))
			{
				m_entities.erase(it);
				break;
			}
		}
	}

	/*
	El método update se encarga de actualizar la información sobre los sensores de
	cada entidad de percepción.
	*/
	void CPerceptionManager::update(unsigned int msecs)
	{
		// Sacamos del reloj de la aplicación el instante actual para gestionar los instantes de las notificaciones
		unsigned int time = Application::CBaseApplication::getInstance()->getApptime();

		// Fase de agregación y chequeo:
		// Se buscan los sensores potenciales de las señales actuales y se comprueba si detectan las señales
		// Las variables notIni, notAdded y notRemoved nos indican cuántas notificaciones hay en la cola de
		// pendientes al principio, después de añadir y después de entregar. Podemos usar estos valores para depurar.
		checkPerception(time);
		
		// Fase de notificación:
		// En esta fase se envían las notificaciones cuyo tiempo de entrega haya caducado
		notifyEntities(time);		
	}

	/*
	Comprueba los sensores de las entidades registradas
	@param time Instante de tiempo en que se realiza la comprobación
	*/
	void CPerceptionManager::checkPerception(unsigned int time)
	{
		// Para cada entidad que puede percibir (senser)
		for (CPerceptionEntity* senser : m_entities)
		{
			// Para cada entidad que puede ser percibida (signaler)
			for (CPerceptionEntity* signaler : m_entities)
			{
				// No permitimos que una entidad se perciba a ella misma
				if (senser != signaler)
				{
					// Por cada señal que emita el signaler
					std::list<CPerceptionSignal*> signals = signaler->getSignals();
					auto itSignal = signals.begin();
					while (itSignal != signals.end())
					{
						CPerceptionSignal* signal = (*itSignal);
						// Comprobamos si la señal está activa
						if (signal->isActive())
						{
							// Por cada sensor del senser
							for (CSensor* sensor : senser->getSensors())
							{
								// Comprobamos si el sensor está activo
								if (sensor->isActive())
								{
									// Comprobamos si el sensor puede percibir la señal invocando al método perceives
									// El método perceives devuelve la notificación que hay que enviar si hay percepción o NULL si no la hay
									CNotification* notification = sensor->perceives(signal, time);
									// Si el sensor percibe la señal, la añadimos a la lista de notificaciones pendientes
									// La lista de notificaciones es una cola con prioridad, por lo que 
									// las notificaciones se ordenarán por el tiempo en el que hay que entregarlas
									if (notification != NULL)
										m_notifications.push(notification);
								}
							}
						}
						// Hay 2 tipos de señales: temporales y permanentes.
						// Si la señal es temporal tenemos que borrarla (esto nos lo indica la propiedad keepAlive).
						// Si no es temporal, pasamos directamente a la siguiente incrementando el iterador.
						if (signal->keepAlive())
							itSignal++;
						else {
							delete(*itSignal);
							itSignal = signals.erase(itSignal);
						}
					}
				}
			}
		}
	}

	/*
	Recorre la lista de notificaciones pendientes y envía las que hayan caducado
	@param time Instante de tiempo en que se realiza la comprobación
	*/
	void CPerceptionManager::notifyEntities(unsigned int time)
	{
		// Si no hay notificaciones pendientes no hacemos nada
		// Procesamos las notificaciones de la cola cuyo tiempo de entrega sea anterior al actual
		// La cola está ordenada por el tiempo de entrega
		CNotification* notification;
		while(!m_notifications.empty() && (notification = m_notifications.top())->getTime() <= time ) {
			m_notifications.pop();
			notification->getSensor()->getPerceptionEntity()->sendNotification(notification);
		}
	}

}
