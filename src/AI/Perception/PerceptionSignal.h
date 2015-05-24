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

#ifndef __AI_PERCEPTIONSIGNAL_H
#define __AI_PERCEPTIONSIGNAL_H

#include "PerceptionManager.h"

namespace AI
{
	/*
	Clase que representa las señales de percepción. Una señal representa de manera abstracta a
	todo aquello que puede ser percibido por un sensor. Las propiedades de la señal son las que
	marcan las diferencias entre los tipos de señales.
	*/
	class CPerceptionSignal
	{
	public:
		CPerceptionSignal(CPerceptionEntity* pEntity, EnmPerceptionType type, float intensity,
			float delay, bool isActive, bool keepAlive) : m_pEntity(pEntity), m_type(type),
			m_intensity(intensity), m_delay(delay), m_active(isActive), m_keepAlive(keepAlive) {}
		~CPerceptionSignal() {}

		bool isActive() const { return m_active; }
		bool keepAlive() { return m_keepAlive; }
		float getIntensity() const { return m_intensity; }
		float getDelay() { return m_delay; }
		EnmPerceptionType getType() const { return m_type; }
		CPerceptionEntity* getPerceptionEntity() const { return m_pEntity; }
		void setActive(bool active) { m_active = active; }

	private:
		CPerceptionEntity* m_pEntity;
		EnmPerceptionType m_type;
		bool m_active;
		bool m_keepAlive;
		float m_intensity;
		float m_delay;
	};

}

#endif
