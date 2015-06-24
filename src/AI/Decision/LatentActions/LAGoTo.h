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

#ifndef __AI_LAGOTO_H
#define __AI_LAGOTO_H

#include "LatentAction.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Weapons/PlayerWeapons.h"
#include "Common/Util/Math.h"

namespace AI 
{

	/*
	Acción latente que lleva la entidad a un destino aleatorio
	*/
	class CLAGoToRandom : public CLatentAction
	{
	public:
		/*
		Constructor.
		*/
		CLAGoToRandom(Logic::CEntity* entity, float tolerance) : CLatentAction(), m_tolerance(tolerance) {this->setEntity(entity);};

		/*
		Destructor.
		*/
		~CLAGoToRandom() {};

		/*
		Establece el destino
		*/
		void setTarget(Vector3 target) { m_target = target; };

		/*
		Devuelve el destino
		*/
		const Vector3& getTarget() { return m_target; };

	protected:
		/*
		Método invocado al principio de la ejecución de la acción, para que se realicen las tareas
		que son únicamente necesarias al principio (y no durante toda la vida de la acción).
		@return Estado al que pasa la acción; si se indica que la acción ha terminado
		(LatentAction::Completed), se invocará al OnStop().
		*/
		virtual LAStatus OnStart();

		/*
		Método invocado al final de la ejecución de la acción, para que se realicen las tareas que
		son únicamente necesarias al final (y no durante toda la vida de la acción). En la mayoría de
		los casos este método no hace nada.
		*/
		virtual void OnStop();

		/*
		Método invocado cíclicamente para que se continúe con la ejecución de la acción.
		@return Estado de la acción tras la ejecución del método; permite indicar si la acción ha
		terminado o se ha suspendido, o si sigue en ejecución.
		*/
		virtual LAStatus OnRun();

		/*
		Método invocado cuando la acción ha sido cancelada (el comportamiento al que pertenece se
		ha abortado por cualquier razón). La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
		ejecución de OnStop().
		@return Estado de la acción tras la ejecución del método.
		*/
		virtual LAStatus OnAbort();

		Vector3 m_target;
		float m_tolerance;
		Logic::Component::CWeapons* m_weapons;

	};



	/*
	Acción latente que lleva la entidad a un destino prefijado
	*/
	class CLAGoTo : public CLAGoToRandom
	{
	public:
		/*
		Constructor.
		*/
		CLAGoTo(Logic::CEntity* entity, float tolerance, Vector3 &target) : CLAGoToRandom(entity,tolerance) {this->setTarget(target);};

		/*
		Destructor.
		*/
		~CLAGoTo() {};

	protected:
		/*
		Método invocado al principio de la ejecución de la acción, para que se realicen las tareas
		que son únicamente necesarias al principio (y no durante toda la vida de la acción).
		@return Estado al que pasa la acción; si se indica que la acción ha terminado
		(LatentAction::Completed), se invocará al OnStop().
		*/
		virtual LAStatus OnStart();

	};



	/*
	Acción latente que lleva la entidad a un destino aleatorio mientras dispara
	*/
	class CLAShoottingGoTo : public CLAGoToRandom
	{
	public:
		/*
		Constructor.
		*/
		CLAShoottingGoTo(Logic::CEntity* entity, float tolerance) : CLAGoToRandom(entity,tolerance), m_frequency(0) {};

		/*
		Destructor.
		*/
		~CLAShoottingGoTo() {};

	protected:
		/*
		Método invocado cíclicamente para que se continúe con la ejecución de la acción.
		@return Estado de la acción tras la ejecución del método; permite indicar si la acción ha
		terminado o se ha suspendido, o si sigue en ejecución.
		*/
		virtual LAStatus OnRun(unsigned int msecs);

		int m_frequency;

	};

}

#endif
