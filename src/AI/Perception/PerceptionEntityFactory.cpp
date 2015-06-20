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
	Devuelve una entidad de percepci�n a partir de una cadena de texto que identifica su tipo.
	@param type Tipo de entidad de percepci�n. Por ahora s�lo admite "enemy" y "player"
	@param radius Radio de la entidad de percepci�n
	@param userData Datos de usuario asociados a la entidad de percepci�n
	@param listener Listener que ser� notificado cuando la entidad perciba una se�al
	@return CPerceptionEntity del tipo correspondiente al par�metro type
	*/
	CPerceptionEntity* CPerceptionEntityFactory::getPerceptionEntity(std::string type, void* userData, IPerceptionListener* listener)
	{
		if		(type == "enemy")		return new CPerceptionEntityEnemy(userData, listener);
		else if (type == "player")		return new CPerceptionEntityPlayer(userData, listener);
		else if (type == "asteroid")	return new CPerceptionEntityAsteroid(userData, listener);
		else							return NULL;
	}

	/*
	Crea una entidad de percepci�n que representa a un enemigo.
	*/
	CPerceptionEntityEnemy::CPerceptionEntityEnemy(void* userData, IPerceptionListener* listener) : CPerceptionEntity(userData, listener)
	{
		// Esta clase tiene un sensor de visi�n b�sico (AI::CSightSensor) y una se�al
		// Sensor:
		// � active = true
		// � threshold = 1.0
		// � maxDistance = 400.0
		// � alpha = PI / 4
		// Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n externo.
		this->addSensor(new CSightSensor(this,true,1.0f,400.0f,Common::Util::Math::PI/4.0f));

		// Se�al:
		// � type = PERCEPTION_SIGHT (es decir, que la se�al es de visibilidad)
		// � intensity = 1.0
		// � delay = 0.0 (instant�nea)
		// � isActive = true
		// � keepAlive = true (la se�al no se destruye despu�s de un ciclo de percepci�n, sino que sigue activa)
		// Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
//		this->addSignal(new CPerceptionSignal(this,PERCEPTION_SIGHT,1.0f,0.0f,true,true));
	}

	/*
	Clase de entidad de percepci�n que representa al jugador
	*/
	CPerceptionEntityPlayer::CPerceptionEntityPlayer(void* userData, IPerceptionListener* listener) :
		CPerceptionEntity(userData, listener)
	{
		// Esta clase s�lo tiene una se�al con los siguientes par�metros:
		// � type = PERCEPTION_SIGHT (es decir, que la se�al es de visibilidad)
		// � intensity = 1.0
		// � delay = 0.0 (instant�nea)
		// � isActive = true
		// � keepAlive = true (la se�al no se destruye despu�s de un ciclo de percepci�n, sino que sigue activa)
		// Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
		this->addSignal(new CPerceptionSignal(this,PERCEPTION_SIGHT,1.0f,0.0f,true,true));
	}

	/*
	Clase de entidad de percepci�n que representa a un asteroide
	*/
	CPerceptionEntityAsteroid::CPerceptionEntityAsteroid(void* userData, IPerceptionListener* listener) :
		CPerceptionEntity(userData, listener)
	{
		// Esta clase s�lo tiene una se�al con los siguientes par�metros:
		// � type = PERCEPTION_SIGHT (es decir, que la se�al es de visibilidad)
		// � intensity = 1.0
		// � delay = 0.0 (instant�nea)
		// � isActive = true
		// � keepAlive = true (la se�al no se destruye despu�s de un ciclo de percepci�n, sino que sigue activa)
		// Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
		this->addSignal(new CPerceptionSignal(this,PERCEPTION_SIGHT,1.0f,0.0f,true,true));
	}

}
