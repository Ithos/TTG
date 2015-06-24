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

#include "LatentAction.h"

namespace AI 
{
	/*
	Método llamado cíclicamente por el responsable de la ejecución de la acción latente.
	@return Devuelve el estado de la función latente: si esta ha terminado su ejecución, si
	necesita que se siga invocando a la función tick() cíclicamente o si, aún sin haber
	terminado, no necesita (de momento) la invocación a tick().
	*/
	CLatentAction::LAStatus CLatentAction::tick(unsigned int msecs) 
	{
		if (m_status == READY)
			m_status = this->OnStart();

		if ((m_status == RUNNING)) {
			m_status = this->OnRun();
			if (m_status == SUCCESS || m_status == FAIL)
				m_stopping = true;
		}
		if ((m_status == SUCCESS || m_status == FAIL) && (m_stopping)) {
			this->OnStop();
			m_stopping = false;
		}
		return m_status;
	}


	/*
	Cancela la tarea que se está ejecutando; se entiende que este método es llamado cuando el
	comportamiento al que pertenece la tarea es anulado.
	*/
	void CLatentAction::abort() {
		m_stopping = false;
		m_status = this->OnAbort();
	}


	/*
	Reinicia la acción, que queda en un estado listo para ser ejecutada de nuevo (READY).
	Dependiendo del estado de la acción en el momento de llamar a reset (si este es RUNNING o
	SUSPENDED) se considera que la acción ha sido abortada y se llama a OnAbort.
	*/
	void CLatentAction::reset() {
		if (m_status != READY) {
			if (m_status == RUNNING || m_status == SUSPENDED)
				this->OnAbort();
			if ((m_status == SUCCESS || m_status == FAIL) && (m_stopping)) 
				this->OnStop();
			m_status = READY;
			m_stopping = false;
		}
	}
	

	/*
	Solicita la finalización de la acción en el siguiente tick, estableciendo el estado	a
	SUCCESS o FAIL según el valor del parámetro de entrada.
	*/
	void CLatentAction::finish(bool success) { 
		m_status = (success ? SUCCESS : FAIL);
		m_stopping = true;
	}


}
