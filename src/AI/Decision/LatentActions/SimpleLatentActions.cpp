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
	M�todo invocado al principio de la ejecuci�n de la acci�n, para que se realicen las tareas
	que son �nicamente necesarias al principio (y no durante toda la vida de la acci�n).
	@return Estado al que pasa la acci�n; si se indica que la acci�n ha terminado
	(LatentAction::Completed), se invocar� al OnStop().
	*/
	CLatentAction::LAStatus CLAWait::OnStart()
	{
		// Tiempo actual
		unsigned int currentTime = Application::CBaseApplication::getInstance()->getApptime();
		// Instante de tiempo en el que tiene que finalizar la acci�n
		m_endingTime = currentTime + m_time;

		return RUNNING; 
	}

	/*
	M�todo invocado al final de la ejecuci�n de la acci�n, para que se realicen las tareas que
	son �nicamente necesarias al final (y no durante toda la vida de la acci�n). En la mayor�a de
	los casos este m�todo no hace nada.
	*/
	void CLAWait::OnStop()
	{
	}

	/*
	M�todo invocado c�clicamente para que se contin�e con la ejecuci�n de la acci�n.
	@return Estado de la acci�n tras la ejecuci�n del m�todo; permite indicar si la acci�n ha
	terminado o se ha suspendido, o si sigue en ejecuci�n.
	*/
	CLatentAction::LAStatus CLAWait::OnRun() 
	{
		// Si hemos superado el tiempo de espera, terminamos con �xito
		// Si no, seguimos en ejecuci�n
		unsigned int currentTime = Application::CBaseApplication::getInstance()->getApptime();
		return (currentTime > m_endingTime) ? SUCCESS : RUNNING;
	}

	/*
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento al que pertenece se
	ha abortado por cualquier raz�n). La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
	ejecuci�n de OnStop().
	@return Estado de la acci�n tras la ejecuci�n del m�todo.
	*/
	CLatentAction::LAStatus CLAWait::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	/*
	Solicita la finalizaci�n de la acci�n en el siguiente tick, estableciendo el estado	a
	SUCCESS o FAIL seg�n el valor del par�metro de entrada.
	*/
	void CLAWait::finish(bool success)
	{
	}



	/*
	M�todo invocado al principio de la ejecuci�n de la acci�n, para que se realicen las tareas
	que son �nicamente necesarias al principio (y no durante toda la vida de la acci�n).
	@return Estado al que pasa la acci�n; si se indica que la acci�n ha terminado
	(LatentAction::Completed), se invocar� al OnStop().
	*/
	CLatentAction::LAStatus CLAShoot::OnStart()
	{
		return RUNNING; 
	}

	/*
	M�todo invocado al final de la ejecuci�n de la acci�n, para que se realicen las tareas que
	son �nicamente necesarias al final (y no durante toda la vida de la acci�n). En la mayor�a de
	los casos este m�todo no hace nada.
	*/
	void CLAShoot::OnStop()
	{
	}

	/*
	M�todo invocado c�clicamente para que se contin�e con la ejecuci�n de la acci�n.
	@return Estado de la acci�n tras la ejecuci�n del m�todo; permite indicar si la acci�n ha
	terminado o se ha suspendido, o si sigue en ejecuci�n.
	*/
	CLatentAction::LAStatus CLAShoot::OnRun() 
	{
		return RUNNING;
	}

	/*
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento al que pertenece se
	ha abortado por cualquier raz�n). La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
	ejecuci�n de OnStop().
	@return Estado de la acci�n tras la ejecuci�n del m�todo.
	*/
	CLatentAction::LAStatus CLAShoot::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	/*
	Solicita la finalizaci�n de la acci�n en el siguiente tick, estableciendo el estado	a
	SUCCESS o FAIL seg�n el valor del par�metro de entrada.
	*/
	void CLAShoot::finish(bool success)
	{
	}

}
