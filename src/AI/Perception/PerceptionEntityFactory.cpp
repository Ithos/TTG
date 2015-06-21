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

#include "PerceptionEntityFactory.h"

#include "SightSensor.h"
#include "PerceptionSignal.h"

namespace AI 
{
	/*
	Devuelve una entidad de percepción a partir de una cadena de texto que identifica su tipo.
	@param type Tipo de entidad de percepción. Por ahora sólo admite "enemy" y "player"
	@param radius Radio de la entidad de percepción
	@param userData Datos de usuario asociados a la entidad de percepción
	@param listener Listener que será notificado cuando la entidad perciba una señal
	@return CPerceptionEntity del tipo correspondiente al parámetro type
	*/
	CPerceptionEntity* CPerceptionEntityFactory::getPerceptionEntity(std::string type, void* userData, IPerceptionListener* listener)
	{
		if		(type == "enemy")		return new CPerceptionEntityEnemy(userData, listener);
		else if (type == "player")		return new CPerceptionEntityPlayer(userData, listener);
		else if (type == "asteroid")	return new CPerceptionEntityAsteroid(userData, listener);
		else							return NULL;
	}

	/*
	Crea una entidad de percepción que representa a un enemigo.
	*/
	CPerceptionEntityEnemy::CPerceptionEntityEnemy(void* userData, IPerceptionListener* listener) : CPerceptionEntity(userData, listener)
	{
		// Esta clase tiene un sensor de visión básico (AI::CSightSensor) y una señal
		// Sensor:
		// · active = true
		// · threshold = 1.0
		// · maxDistance = 400.0
		// · alpha = PI / 4
		// Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración externo.
		this->addSensor(new CSightSensor(this,true,1.0f,400.0f,Common::Util::Math::PI/4.0f));

		// Señal:
		// · type = PERCEPTION_SIGHT (es decir, que la señal es de visibilidad)
		// · intensity = 1.0
		// · delay = 0.0 (instantánea)
		// · isActive = true
		// · keepAlive = true (la señal no se destruye después de un ciclo de percepción, sino que sigue activa)
		// Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
//		this->addSignal(new CPerceptionSignal(this,PERCEPTION_SIGHT,1.0f,0.0f,true,true));
	}

	/*
	Clase de entidad de percepción que representa al jugador
	*/
	CPerceptionEntityPlayer::CPerceptionEntityPlayer(void* userData, IPerceptionListener* listener) :
		CPerceptionEntity(userData, listener)
	{
		// Esta clase sólo tiene una señal con los siguientes parámetros:
		// · type = PERCEPTION_SIGHT (es decir, que la señal es de visibilidad)
		// · intensity = 1.0
		// · delay = 0.0 (instantánea)
		// · isActive = true
		// · keepAlive = true (la señal no se destruye después de un ciclo de percepción, sino que sigue activa)
		// Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
		this->addSignal(new CPerceptionSignal(this,PERCEPTION_SIGHT,1.0f,0.0f,true,true));
	}

	/*
	Clase de entidad de percepción que representa a un asteroide
	*/
	CPerceptionEntityAsteroid::CPerceptionEntityAsteroid(void* userData, IPerceptionListener* listener) :
		CPerceptionEntity(userData, listener)
	{
		// Esta clase sólo tiene una señal con los siguientes parámetros:
		// · type = PERCEPTION_SIGHT (es decir, que la señal es de visibilidad)
		// · intensity = 1.0
		// · delay = 0.0 (instantánea)
		// · isActive = true
		// · keepAlive = true (la señal no se destruye después de un ciclo de percepción, sino que sigue activa)
		// Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
		this->addSignal(new CPerceptionSignal(this,PERCEPTION_SIGHT,1.0f,0.0f,true,true));
	}

}
