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
	Clase que implementa un sensor de visi�n b�sico. Este sensor comprueba si la se�al
	se encuentra dentro de un cono de visi�n.
	El cono est� definido por dos par�metros: la distancia y el �ngulo. La distancia indica
	la separaci�n m�xima que puede existir entre el sensor y la se�al para que �sta sea
	percibida. El �ngulo da el sector circular dentro del que tiene que estar la se�al
	para poder percibirla. El �ngulo se da en relaci�n a la direcci�n hacia la que apunta
	la entidad de percepci�n del sensor. Por ejemplo, si la entidad est� mirando en la direcci�n
	dir y el �ngulo es PI/3 (60�), el cono abarcar�a desde dir - PI/3 hasta dir + PI/3. Los valores
	para el �ngulo estar�n en el intervalo [0, PI].
	*/
	class CSightSensor : public CSensor
	{
	public:
		/*
		Constructor. Inicializa los par�metros b�sicos
		@param pEntity Entidad a la que pertenece el sensor
		@param active Indica si el sensor est� activo (puede percibir) o no
		@param threshold Intensidad m�nima de la se�al para que el sensor pueda percibir
		@param maxDistance Distancia m�xima a la que puede percibirse
		@param alpha �ngulo del cono.
		*/
		CSightSensor(CPerceptionEntity* pEntity, bool active, float threshold, float maxDistance, float alpha)
			: CSensor(pEntity, active, threshold), m_maxDistance(maxDistance), m_alpha(alpha) {};

		/*
		Destructor
		*/
		~CSightSensor();

		/*
		Realiza todas las comprobaciones necesarias para averiguar si la se�al recibida
		se encuentra dentro del cono de visi�n.
		@param perceptible Se�al cuya percepci�n queremos comprobar
		@param time Instante en el que se realiza la comprobaci�n de percepci�n
		@return NULL si no se ha percibido la se�al. Una instancia de CNotification en caso contrario.
		*/
		virtual CNotification* perceives(const CPerceptionSignal * perceptible, unsigned long time);

		/*
		Devuelve el tipo de sensor. En este caso un sensor de visi�n.
		*/
		virtual EnmPerceptionType getType() { return PERCEPTION_SIGHT; };

	private:
		/*
		Distancia m�xima del cono
		*/
		float m_maxDistance;

		/*
		Amplitud m�xima del cono
		*/
		float m_alpha;
	};

}

#endif
