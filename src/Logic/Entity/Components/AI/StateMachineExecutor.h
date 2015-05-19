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

#ifndef __LOGIC_STATEMACHINEEXECUTOR_H
#define __LOGIC_STATEMACHINEEXECUTOR_H

#include "../Component.h"
#include <AI/Decision/StateMachines/StateMachine.h>

namespace Logic
{
	namespace Component
	{
		/*
		Componente que se encarga de ejecutar máquinas de estado.
		*/
		class CStateMachineExecutor : public IComponent
		{
			DEC_FACTORY(CStateMachineExecutor);
	
		public:
			/*
			Constructor
			*/
			CStateMachineExecutor(void) : m_currentStateMachine(0), m_currentAction(0) {};

			/*
			Destructor
			*/
			~CStateMachineExecutor(void) { 
				if (m_currentStateMachine != 0) delete m_currentStateMachine;
			};

			/*
			Inicialización del componente, utilizando la información extraída de la entidad leída
			del mapa (Maps::CMapEntity). Toma del mapa el atributo behavior, que indica el nombre
			de la máquina de estado a ejecutar.
			*/
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

			/*
			Método llamado en cada frame que actualiza el estado del componente. Este método
			actualiza la máquina de estado. Si hay un cambio de estado,  se actualiza el valor del
			atributo m_currentAction, que es el que contiene la acción latente que se está ejecutando.
			Por último, se llama al tick de la acción latente para que avance su ejecución.
			@param msecs Milisegundos transcurridos desde el último tick.
			*/
			virtual void tick(unsigned int msecs);

		private:
			/**
			Almacena la máquina de estado que se está ejecutando
			*/
			AI::CStateMachine<AI::CLatentAction>* m_currentStateMachine;

			/**
			Acción que se está ejecutando.
			*/
			AI::CLatentAction* m_currentAction;

		};

		REG_FACTORY(CStateMachineExecutor);
	}
}

#endif
