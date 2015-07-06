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
	Esta acción espera durante un periodo de tiempo indicado en el constructor.
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

		/*
		Solicita la finalización de la acción en el siguiente tick, estableciendo el estado	a
		SUCCESS o FAIL según el valor del parámetro de entrada.
		*/
		virtual void finish(bool success);

	protected:
		/*
		Tiempo de espera
		*/
		unsigned long m_time;

		/*
		Tiempo en el que se termina la acción
		*/
		unsigned long m_endingTime;
	};

}

#endif
