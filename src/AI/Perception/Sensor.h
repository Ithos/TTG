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

#ifndef __AI_SENSOR_H
#define __AI_SENSOR_H

#include "PerceptionManager.h"

namespace AI 
{

	class CPerceptionSignal;

	/*
	Clase abstracta que representa un sensor. Los sensores representan la capacidad de percibir
	determinados tipos de estímulo por parte de las entidades. Los estímulos se representan
	mediante señales (AI::CPerceptionSignal).
	Toda entidad que tiene que percibir un tipo de señal debe tener al menos un sensor asociado.
	Cada sensor que herede de esta clase tendrá que implementar al menos el método getType (identifica
	el tipo de señal que puede percibir) y perceives. El método perceives será invocado por el gestor
	de percepción para cada señal. En este método es donde se incluye todo el código necesario para
	comprobar si un sensor detecta una señal determinada.
	*/
	class CSensor
	{
	public:
		CSensor(CPerceptionEntity* pEntity, bool active, float threshold) :
			m_pEntity(pEntity), m_active(active), m_threshold(threshold) {}
		virtual ~CSensor() {}

		virtual CNotification* perceives(const CPerceptionSignal* perceptible, unsigned long time) = 0;

		virtual EnmPerceptionType getType() = 0;
		bool isActive() { return m_active; }
		void setActive(bool active) { m_active = active; }
		CPerceptionEntity* getPerceptionEntity() { return m_pEntity; }
		float getThreshold() { return m_threshold; }

	protected:
		bool m_active;
		CPerceptionEntity* m_pEntity;
		float m_threshold;
	};

}

#endif
