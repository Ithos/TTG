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
	Acci�n latente que lleva la entidad a un destino aleatorio
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
		M�todo invocado al principio de la ejecuci�n de la acci�n, para que se realicen las tareas
		que son �nicamente necesarias al principio (y no durante toda la vida de la acci�n).
		@return Estado al que pasa la acci�n; si se indica que la acci�n ha terminado
		(LatentAction::Completed), se invocar� al OnStop().
		*/
		virtual LAStatus OnStart();

		/*
		M�todo invocado al final de la ejecuci�n de la acci�n, para que se realicen las tareas que
		son �nicamente necesarias al final (y no durante toda la vida de la acci�n). En la mayor�a de
		los casos este m�todo no hace nada.
		*/
		virtual void OnStop();

		/*
		M�todo invocado c�clicamente para que se contin�e con la ejecuci�n de la acci�n.
		@return Estado de la acci�n tras la ejecuci�n del m�todo; permite indicar si la acci�n ha
		terminado o se ha suspendido, o si sigue en ejecuci�n.
		*/
		virtual LAStatus OnRun();

		/*
		M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento al que pertenece se
		ha abortado por cualquier raz�n). La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
		ejecuci�n de OnStop().
		@return Estado de la acci�n tras la ejecuci�n del m�todo.
		*/
		virtual LAStatus OnAbort();

		Vector3 m_target;
		float m_tolerance;
		Logic::Component::CWeapons* m_weapons;

	};



	/*
	Acci�n latente que lleva la entidad a un destino prefijado
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
		M�todo invocado al principio de la ejecuci�n de la acci�n, para que se realicen las tareas
		que son �nicamente necesarias al principio (y no durante toda la vida de la acci�n).
		@return Estado al que pasa la acci�n; si se indica que la acci�n ha terminado
		(LatentAction::Completed), se invocar� al OnStop().
		*/
		virtual LAStatus OnStart();

	};



	/*
	Acci�n latente que lleva la entidad a un destino aleatorio mientras dispara
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
		M�todo invocado c�clicamente para que se contin�e con la ejecuci�n de la acci�n.
		@return Estado de la acci�n tras la ejecuci�n del m�todo; permite indicar si la acci�n ha
		terminado o se ha suspendido, o si sigue en ejecuci�n.
		*/
		virtual LAStatus OnRun(unsigned int msecs);

		int m_frequency;

	};

}

#endif
