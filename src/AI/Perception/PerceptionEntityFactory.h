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

#ifndef __AI_PERCEPTIONENTITYFACTORY_H
#define __AI_PERCEPTIONENTITYFACTORY_H

#include "PerceptionEntity.h"

namespace AI
{
	/*
	Clase factor�a para crear las entidades de percepci�n.
	Tiene un m�todo est�tico, getPerceptionEntity, que recibe una cadena que identifica
	la entidad de percepci�n y algunos par�metros, y devuelve una nueva entidad de percepci�n
	del tipo correspondiente.
	*/
	class CPerceptionEntityFactory
	{
	public:
		/*
		Devuelve una entidad de percepci�n a partir de una cadena de texto que identifica su tipo.
		@param type Tipo de entidad de percepci�n. Por ahora s�lo admite "enemy" y "player"
		@param radius Radio de la entidad de percepci�n
		@param userData Datos de usuario asociados a la entidad de percepci�n
		@param listener Listener que ser� notificado cuando la entidad perciba una se�al
		@return CPerceptionEntity del tipo correspondiente al par�metro type
		*/
		static CPerceptionEntity* getPerceptionEntity(std::string type, void* userData, IPerceptionListener* listener);

	};



	/*
	Clase de entidad de percepci�n que representa a un enemigo
	Esta clase s�lo tiene un sensor de visi�n b�sico (AI::CSightSensor), inicializado
	con los siguientes par�metros:
	� active = true
	� threshold = 1.0
	� maxDistance = 200.0
	� alpha = PI / 4
	Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
	*/
	class CPerceptionEntityEnemy : public CPerceptionEntity
	{
	public:
		CPerceptionEntityEnemy(void* userData, IPerceptionListener* listener);
		~CPerceptionEntityEnemy() {};
	};



	/*
	Clase de entidad de percepci�n que representa al jugador
	Esta clase s�lo tiene una se�al con los siguientes par�metros:
	� type = PERCEPTION_SIGHT (es decir, que la se�al es de visibilidad)
	� intensity = 1.0
	� isActive = true
	� keepAlive = true (la se�al no se destruye despu�s de un ciclo de percepci�n, sino que sigue activa)
	Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
	*/
	class CPerceptionEntityPlayer : public CPerceptionEntity
	{
	public:
		CPerceptionEntityPlayer(void* userData, IPerceptionListener* listener);
		~CPerceptionEntityPlayer() {};
	};



	/*
	Clase de entidad de percepci�n que representa a un asteroide
	Esta clase s�lo tiene una se�al con los siguientes par�metros:
	� type = PERCEPTION_SIGHT (es decir, que la se�al es de visibilidad)
	� intensity = 1.0
	� isActive = true
	� keepAlive = true (la se�al no se destruye despu�s de un ciclo de percepci�n, sino que sigue activa)
	Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
	*/
	class CPerceptionEntityAsteroid : public CPerceptionEntity
	{
	public:
		CPerceptionEntityAsteroid(void* userData, IPerceptionListener* listener);
		~CPerceptionEntityAsteroid() {};
	};

}

#endif
