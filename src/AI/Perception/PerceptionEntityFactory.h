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
	Clase factoría para crear las entidades de percepción.
	Tiene un método estático, getPerceptionEntity, que recibe una cadena que identifica
	la entidad de percepción y algunos parámetros, y devuelve una nueva entidad de percepción
	del tipo correspondiente.
	*/
	class CPerceptionEntityFactory
	{
	public:
		/*
		Devuelve una entidad de percepción a partir de una cadena de texto que identifica su tipo.
		@param type Tipo de entidad de percepción. Por ahora sólo admite "enemy" y "player"
		@param radius Radio de la entidad de percepción
		@param userData Datos de usuario asociados a la entidad de percepción
		@param listener Listener que será notificado cuando la entidad perciba una señal
		@return CPerceptionEntity del tipo correspondiente al parámetro type
		*/
		static CPerceptionEntity* getPerceptionEntity(std::string type, void* userData, IPerceptionListener* listener);

	};



	/*
	Clase de entidad de percepción que representa a un enemigo
	Esta clase sólo tiene un sensor de visión básico (AI::CSightSensor), inicializado
	con los siguientes parámetros:
	· active = true
	· threshold = 1.0
	· maxDistance = 200.0
	· alpha = PI / 4
	Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
	*/
	class CPerceptionEntityEnemy : public CPerceptionEntity
	{
	public:
		CPerceptionEntityEnemy(void* userData, IPerceptionListener* listener);
		~CPerceptionEntityEnemy() {};
	};



	/*
	Clase de entidad de percepción que representa al jugador
	Esta clase sólo tiene una señal con los siguientes parámetros:
	· type = PERCEPTION_SIGHT (es decir, que la señal es de visibilidad)
	· intensity = 1.0
	· isActive = true
	· keepAlive = true (la señal no se destruye después de un ciclo de percepción, sino que sigue activa)
	Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
	*/
	class CPerceptionEntityPlayer : public CPerceptionEntity
	{
	public:
		CPerceptionEntityPlayer(void* userData, IPerceptionListener* listener);
		~CPerceptionEntityPlayer() {};
	};



	/*
	Clase de entidad de percepción que representa a un asteroide
	Esta clase sólo tiene una señal con los siguientes parámetros:
	· type = PERCEPTION_SIGHT (es decir, que la señal es de visibilidad)
	· intensity = 1.0
	· isActive = true
	· keepAlive = true (la señal no se destruye después de un ciclo de percepción, sino que sigue activa)
	Cuidado: estos parámetros son datos y, por lo tanto, deberían declararse en un archivo de configuración aparte.
	*/
	class CPerceptionEntityAsteroid : public CPerceptionEntity
	{
	public:
		CPerceptionEntityAsteroid(void* userData, IPerceptionListener* listener);
		~CPerceptionEntityAsteroid() {};
	};

}

#endif
