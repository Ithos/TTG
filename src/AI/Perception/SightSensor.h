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

#ifndef __AI_SIGHTSENSOR_H
#define __AI_SIGHTSENSOR_H

#include "Sensor.h"

namespace AI 
{
	/*
	Clase que implementa un sensor de visión básico. Este sensor comprueba si la señal
	se encuentra dentro de un cono de visión.
	El cono está definido por dos parámetros: la distancia y el ángulo. La distancia indica
	la separación máxima que puede existir entre el sensor y la señal para que ésta sea
	percibida. El ángulo da el sector circular dentro del que tiene que estar la señal
	para poder percibirla. El ángulo se da en relación a la dirección hacia la que apunta
	la entidad de percepción del sensor. Por ejemplo, si la entidad está mirando en la dirección
	dir y el ángulo es PI/3 (60º), el cono abarcaría desde dir - PI/3 hasta dir + PI/3. Los valores
	para el ángulo estarán en el intervalo [0, PI].
	*/
	class CSightSensor : public CSensor
	{
	public:
		/*
		Constructor. Inicializa los parámetros básicos
		@param pEntity Entidad a la que pertenece el sensor
		@param active Indica si el sensor está activo (puede percibir) o no
		@param threshold Intensidad mínima de la señal para que el sensor pueda percibir
		@param maxDistance Distancia máxima a la que puede percibirse
		@param alpha Ángulo del cono.
		*/
		CSightSensor(CPerceptionEntity* pEntity, bool active, float threshold, float maxDistance, float alpha)
			: CSensor(pEntity, active, threshold), m_maxDistance(maxDistance), m_alpha(alpha) {};

		/*
		Destructor
		*/
		~CSightSensor();

		/*
		Realiza todas las comprobaciones necesarias para averiguar si la señal recibida
		se encuentra dentro del cono de visión.
		@param perceptible Señal cuya percepción queremos comprobar
		@param time Instante en el que se realiza la comprobación de percepción
		@return NULL si no se ha percibido la señal. Una instancia de CNotification en caso contrario.
		*/
		virtual CNotification* perceives(const CPerceptionSignal * perceptible, unsigned long time);

		/*
		Devuelve el tipo de sensor. En este caso un sensor de visión.
		*/
		virtual EnmPerceptionType getType() { return PERCEPTION_SIGHT; };

	private:
		/*
		Distancia máxima del cono
		*/
		float m_maxDistance;

		/*
		Amplitud máxima del cono
		*/
		float m_alpha;
	};

}

#endif
