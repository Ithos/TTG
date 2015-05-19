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

#ifndef __AI_SIMPLELATENTACTIONS_H
#define __AI_SIMPLELATENTACTIONS_H

#include "LatentAction.h"

#include "Logic/Entity/Entity.h"

using namespace Logic;

namespace AI 
{
	/*
	Esta acci�n espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLAWait : public CLatentAction
	{
	public:
		/*
		Constructor.
		@param time Tiempo de espera
		*/
		CLAWait(unsigned long time) : CLatentAction(), m_time(time) {};

		/*
		Destructor
		*/
		~CLAWait() {};

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

		/*
		Solicita la finalizaci�n de la acci�n en el siguiente tick, estableciendo el estado	a
		SUCCESS o FAIL seg�n el valor del par�metro de entrada.
		*/
		virtual void finish(bool success);

	protected:
		/*
		Tiempo de espera
		*/
		unsigned long m_time;

		/*
		Tiempo en el que se termina la acci�n
		*/
		unsigned long m_endingTime;
	};

}

#endif
