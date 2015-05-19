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
		Componente que se encarga de ejecutar m�quinas de estado.
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
			Inicializaci�n del componente, utilizando la informaci�n extra�da de la entidad le�da
			del mapa (Maps::CMapEntity). Toma del mapa el atributo behavior, que indica el nombre
			de la m�quina de estado a ejecutar.
			*/
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

			/*
			M�todo llamado en cada frame que actualiza el estado del componente. Este m�todo
			actualiza la m�quina de estado. Si hay un cambio de estado,  se actualiza el valor del
			atributo m_currentAction, que es el que contiene la acci�n latente que se est� ejecutando.
			Por �ltimo, se llama al tick de la acci�n latente para que avance su ejecuci�n.
			@param msecs Milisegundos transcurridos desde el �ltimo tick.
			*/
			virtual void tick(unsigned int msecs);

		private:
			/**
			Almacena la m�quina de estado que se est� ejecutando
			*/
			AI::CStateMachine<AI::CLatentAction>* m_currentStateMachine;

			/**
			Acci�n que se est� ejecutando.
			*/
			AI::CLatentAction* m_currentAction;

		};

		REG_FACTORY(CStateMachineExecutor);
	}
}

#endif
