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

#include "StateMachineExecutor.h"

#include "Common/Map/MapEntity.h"
#include "AI/AI.h"
#include "AI/Decision/StateMachines/StateMachineFactory.h"

namespace Logic 
{
	namespace Component
	{
		IMP_FACTORY(CStateMachineExecutor);

		/*
		Inicialización del componente, utilizando la información extraída de la entidad leída
		del mapa (Maps::CMapEntity). Toma del mapa el atributo behavior, que indica el nombre
		de la máquina de estado a ejecutar.
		*/
		bool CStateMachineExecutor::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
		{
			if(!IComponent::spawn(entity,scene,entityInfo))
				return false;

			// Obtiene el nombre de la máquina de estado 
			if(entityInfo->hasAttribute("behavior")) {
				std::string smName = entityInfo->getStringAttribute("behavior");
				if (m_currentStateMachine != 0)
					delete m_currentStateMachine;
				// Saca una instancia de la máquina de estado de la factoría
				m_currentStateMachine = AI::CStateMachineFactory::getStateMachine(smName, entity);
			}

			return true;
		}

		/*
		Método llamado en cada frame que actualiza el estado del componente. Este método
		actualiza la máquina de estado. Si hay un cambio de estado,  se actualiza el valor del
		atributo m_currentAction, que es el que contiene la acción latente que se está ejecutando.
		Por último, se llama al tick de la acción latente para que avance su ejecución.
		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void CStateMachineExecutor::tick(unsigned int msecs)
		{
			IComponent::tick(msecs);

			// Actualizamos la máquina de estado
			if (m_currentStateMachine != NULL) {
				// Si la máquina cambia de estado
				if (m_currentStateMachine->nextState()) {
					// Dejamos la acción anterior lista (la resetamos) para que pueda volver
					// a ser ejecutada
					if (m_currentAction != NULL) 
						m_currentAction->reset();
					// Cambiamos la acción latente actual por la del nuevo estado
					m_currentAction = m_currentStateMachine->getCurrentNode();
				}
			}
			// Ejecutamos la acción latente correspondiente al estado actual
			if (m_currentAction != NULL)
				m_currentAction->tick();
		}
	}
}