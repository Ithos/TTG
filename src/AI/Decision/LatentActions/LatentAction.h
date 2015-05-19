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

#ifndef __AI_LATENTACTION_H
#define __AI_LATENTACTION_H

#include "Logic/Entity/Entity.h"

namespace AI 
{
	/*
	Clase que representa una acci�n latente del sistema.
	Debe ser extendida con clases hijas que implementan las distintas acciones latentes del sistema.
	Las acciones latentes despu�s ser�n ejecutadas por una entidad externa (FSM, BT...)
	*/
	class CLatentAction
	{
	public:
		enum LAStatus {
			READY,		// La acci�n latente a�n no se ha empezado a ejecutar
			RUNNING,	// Ejecut�ndose (necesita ciclos de CPU)
			SUSPENDED,	// Esperando alg�n evento externo (no necesita ciclos de CPU por el momento)
			SUCCESS,	// La acci�n latente ha terminado su tarea con �xito
			FAIL		// La acci�n latente ha terminado su tarea con fallo
		};
		
		/*
		Constructor.
		*/
		CLatentAction() : m_status(READY), m_stopping(false) {};

		/*
		Establece la entidad que ejecuta la acci�n.
		*/
		void setEntity(Logic::CEntity* entity) { m_entity = entity; };

		/*
		M�todo llamado c�clicamente por el responsable de la ejecuci�n de la acci�n latente.
		@return Devuelve el estado de la funci�n latente: si esta ha terminado su ejecuci�n, si
		necesita que se siga invocando a la funci�n tick() c�clicamente o si, a�n sin haber
		terminado, no necesita (de momento) la invocaci�n a tick().
		*/
		LAStatus tick();

		/*
		Cancela la tarea que se est� ejecutando; se entiende que este m�todo es llamado cuando el
		comportamiento al que pertenece la tarea es anulado.
		*/
		void abort();

		/*
		Reinicia la acci�n, que queda en un estado listo para ser ejecutada de nuevo (READY).
		Dependiendo del estado de la acci�n en el momento de llamar a reset (si este es RUNNING o
		SUSPENDED) se considera que la acci�n ha sido abortada y se llama a OnAbort.
		*/
		void reset();

		/*
		Devuelve el estado actual de la acci�n.
		*/
		LAStatus getStatus() {return m_status; };

	private:
		/*
		Estado de la acci�n.
		*/
		LAStatus m_status;

	protected:
		/*
		Entidad que ejecuta la acci�n
		*/
		Logic::CEntity* m_entity;

		/*
		Se pone a cierto cuando se solicita la finalizaci�n de la acci�n.
		*/
		bool m_stopping;

		/*
		M�todo invocado al principio de la ejecuci�n de la acci�n, para que se realicen las tareas
		que son �nicamente necesarias al principio (y no durante toda la vida de la acci�n).
		@return Estado al que pasa la acci�n; si se indica que la acci�n ha terminado
		(LatentAction::Completed), se invocar� al OnStop().
		*/
		virtual LAStatus OnStart() { return READY;}

		/*
		M�todo invocado al final de la ejecuci�n de la acci�n, para que se realicen las tareas que
		son �nicamente necesarias al final (y no durante toda la vida de la acci�n). En la mayor�a de
		los casos este m�todo no hace nada.
		*/
		virtual void OnStop() {}

		/*
		M�todo invocado c�clicamente para que se contin�e con la ejecuci�n de la acci�n.
		@return Estado de la acci�n tras la ejecuci�n del m�todo; permite indicar si la acci�n ha
		terminado o se ha suspendido, o si sigue en ejecuci�n.
		*/
		virtual LAStatus OnRun() { return SUCCESS;}

		/*
		M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento al que pertenece se
		ha abortado por cualquier raz�n). La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
		ejecuci�n de OnStop().
		@return Estado de la acci�n tras la ejecuci�n del m�todo.
		*/
		virtual LAStatus OnAbort() { return FAIL; }

		/*
		Solicita la finalizaci�n de la acci�n en el siguiente tick, estableciendo el estado	a
		SUCCESS o FAIL seg�n el valor del par�metro de entrada.
		*/
		virtual void finish(bool success);

		/*
		Contin�a la ejecuci�n de la acci�n cuando esta se encuentra suspendida. Cuidado: �nicamente
		realiza el cambio de estado de SUSPENDED a RUNNING. Si el estado original es otro, no tiene
		efecto.
		*/
		void resume() { if (m_status == SUSPENDED) m_status = RUNNING; };
	};

}

#endif
