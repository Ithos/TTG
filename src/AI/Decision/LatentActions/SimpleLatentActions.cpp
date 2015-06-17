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

#include "SimpleLatentActions.h"
#include "Application/BaseApplication.h"
#include "AI/Decision/StateMachines/StateMachine.h"

namespace AI 
{

	/*
	Método invocado al principio de la ejecución de la acción, para que se realicen las tareas
	que son únicamente necesarias al principio (y no durante toda la vida de la acción).
	@return Estado al que pasa la acción; si se indica que la acción ha terminado
	(LatentAction::Completed), se invocará al OnStop().
	*/
	CLatentAction::LAStatus CLAWait::OnStart()
	{
		// Tiempo actual
		unsigned int currentTime = Application::CBaseApplication::getInstance()->getApptime();
		// Instante de tiempo en el que tiene que finalizar la acción
		m_endingTime = currentTime + m_time;

		return RUNNING; 
	}

	/*
	Método invocado al final de la ejecución de la acción, para que se realicen las tareas que
	son únicamente necesarias al final (y no durante toda la vida de la acción). En la mayoría de
	los casos este método no hace nada.
	*/
	void CLAWait::OnStop()
	{
	}

	/*
	Método invocado cíclicamente para que se continúe con la ejecución de la acción.
	@return Estado de la acción tras la ejecución del método; permite indicar si la acción ha
	terminado o se ha suspendido, o si sigue en ejecución.
	*/
	CLatentAction::LAStatus CLAWait::OnRun() 
	{
		// Si hemos superado el tiempo de espera, terminamos con éxito
		// Si no, seguimos en ejecución
		unsigned int currentTime = Application::CBaseApplication::getInstance()->getApptime();
		return (currentTime > m_endingTime) ? SUCCESS : RUNNING;
	}

	/*
	Método invocado cuando la acción ha sido cancelada (el comportamiento al que pertenece se
	ha abortado por cualquier razón). La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
	ejecución de OnStop().
	@return Estado de la acción tras la ejecución del método.
	*/
	CLatentAction::LAStatus CLAWait::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	/*
	Solicita la finalización de la acción en el siguiente tick, estableciendo el estado	a
	SUCCESS o FAIL según el valor del parámetro de entrada.
	*/
	void CLAWait::finish(bool success)
	{
	}



	/*
	Método invocado al principio de la ejecución de la acción, para que se realicen las tareas
	que son únicamente necesarias al principio (y no durante toda la vida de la acción).
	@return Estado al que pasa la acción; si se indica que la acción ha terminado
	(LatentAction::Completed), se invocará al OnStop().
	*/
	CLatentAction::LAStatus CLAShoot::OnStart()
	{
		return RUNNING; 
	}

	/*
	Método invocado al final de la ejecución de la acción, para que se realicen las tareas que
	son únicamente necesarias al final (y no durante toda la vida de la acción). En la mayoría de
	los casos este método no hace nada.
	*/
	void CLAShoot::OnStop()
	{
	}

	/*
	Método invocado cíclicamente para que se continúe con la ejecución de la acción.
	@return Estado de la acción tras la ejecución del método; permite indicar si la acción ha
	terminado o se ha suspendido, o si sigue en ejecución.
	*/
	CLatentAction::LAStatus CLAShoot::OnRun() 
	{
		return RUNNING;
	}

	/*
	Método invocado cuando la acción ha sido cancelada (el comportamiento al que pertenece se
	ha abortado por cualquier razón). La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
	ejecución de OnStop().
	@return Estado de la acción tras la ejecución del método.
	*/
	CLatentAction::LAStatus CLAShoot::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	/*
	Solicita la finalización de la acción en el siguiente tick, estableciendo el estado	a
	SUCCESS o FAIL según el valor del parámetro de entrada.
	*/
	void CLAShoot::finish(bool success)
	{
	}

}
