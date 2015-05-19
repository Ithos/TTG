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
	Clase que representa una acción latente del sistema.
	Debe ser extendida con clases hijas que implementan las distintas acciones latentes del sistema.
	Las acciones latentes después serán ejecutadas por una entidad externa (FSM, BT...)
	*/
	class CLatentAction
	{
	public:
		enum LAStatus {
			READY,		// La acción latente aún no se ha empezado a ejecutar
			RUNNING,	// Ejecutándose (necesita ciclos de CPU)
			SUSPENDED,	// Esperando algún evento externo (no necesita ciclos de CPU por el momento)
			SUCCESS,	// La acción latente ha terminado su tarea con éxito
			FAIL		// La acción latente ha terminado su tarea con fallo
		};
		
		/*
		Constructor.
		*/
		CLatentAction() : m_status(READY), m_stopping(false) {};

		/*
		Establece la entidad que ejecuta la acción.
		*/
		void setEntity(Logic::CEntity* entity) { m_entity = entity; };

		/*
		Método llamado cíclicamente por el responsable de la ejecución de la acción latente.
		@return Devuelve el estado de la función latente: si esta ha terminado su ejecución, si
		necesita que se siga invocando a la función tick() cíclicamente o si, aún sin haber
		terminado, no necesita (de momento) la invocación a tick().
		*/
		LAStatus tick();

		/*
		Cancela la tarea que se está ejecutando; se entiende que este método es llamado cuando el
		comportamiento al que pertenece la tarea es anulado.
		*/
		void abort();

		/*
		Reinicia la acción, que queda en un estado listo para ser ejecutada de nuevo (READY).
		Dependiendo del estado de la acción en el momento de llamar a reset (si este es RUNNING o
		SUSPENDED) se considera que la acción ha sido abortada y se llama a OnAbort.
		*/
		void reset();

		/*
		Devuelve el estado actual de la acción.
		*/
		LAStatus getStatus() {return m_status; };

	private:
		/*
		Estado de la acción.
		*/
		LAStatus m_status;

	protected:
		/*
		Entidad que ejecuta la acción
		*/
		Logic::CEntity* m_entity;

		/*
		Se pone a cierto cuando se solicita la finalización de la acción.
		*/
		bool m_stopping;

		/*
		Método invocado al principio de la ejecución de la acción, para que se realicen las tareas
		que son únicamente necesarias al principio (y no durante toda la vida de la acción).
		@return Estado al que pasa la acción; si se indica que la acción ha terminado
		(LatentAction::Completed), se invocará al OnStop().
		*/
		virtual LAStatus OnStart() { return READY;}

		/*
		Método invocado al final de la ejecución de la acción, para que se realicen las tareas que
		son únicamente necesarias al final (y no durante toda la vida de la acción). En la mayoría de
		los casos este método no hace nada.
		*/
		virtual void OnStop() {}

		/*
		Método invocado cíclicamente para que se continúe con la ejecución de la acción.
		@return Estado de la acción tras la ejecución del método; permite indicar si la acción ha
		terminado o se ha suspendido, o si sigue en ejecución.
		*/
		virtual LAStatus OnRun() { return SUCCESS;}

		/*
		Método invocado cuando la acción ha sido cancelada (el comportamiento al que pertenece se
		ha abortado por cualquier razón). La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
		ejecución de OnStop().
		@return Estado de la acción tras la ejecución del método.
		*/
		virtual LAStatus OnAbort() { return FAIL; }

		/*
		Solicita la finalización de la acción en el siguiente tick, estableciendo el estado	a
		SUCCESS o FAIL según el valor del parámetro de entrada.
		*/
		virtual void finish(bool success);

		/*
		Continúa la ejecución de la acción cuando esta se encuentra suspendida. Cuidado: únicamente
		realiza el cambio de estado de SUSPENDED a RUNNING. Si el estado original es otro, no tiene
		efecto.
		*/
		void resume() { if (m_status == SUSPENDED) m_status = RUNNING; };
	};

}

#endif
