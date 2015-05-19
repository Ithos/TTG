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

#ifndef __AI_CONDITION_H
#define __AI_CONDITION_H

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Movement/Transform.h"
#include "../LatentActions/LatentAction.h"
#include "Common/Util/Math.h"
#include "Common/Data/TTG_Types.h"

namespace AI 
{
	/*
	Interfaz que deberán implementar las condiciones de  las máquinas de estado.
	*/
	template <class TNode>
	class ICondition
	{
	public:
		/*
		En este método se comprobará la condición. Recibe el  nodo al que pertenece la
		condición (de esta manera  podemos comprobar si el nodo ha terminado y en qué 
		condiciones) y la entidad que ejecuta el comportamiento  (que puede ser NULL).
		@param currentNode Nodo al que pertenece la condición.
		@param entity Entidad que ejecuta el comportamiento
		@return true o false según si se cumple la condición o no.
		*/
		virtual bool check(TNode* currentNode, Logic::CEntity* entity) = 0;
	};


	/*
	Esta clase define una condición que siempre se evalúa a true.
	*/
	template <class TNode>
	class CConditionTrue : public ICondition<TNode>
	{
	public:
		bool check(TNode* currentNode, Logic::CEntity* entity) { return true; }
	};
	

	/*
	Esta clase define una condición que se evalúa a true si la  acción a la que pertenece
	ha terminado.
	*/
	class CConditionFinished : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, Logic::CEntity* entity) 
		{ 
			return (currentNode->getStatus() == CLatentAction::SUCCESS) || 
				(currentNode->getStatus() == CLatentAction::FAIL);
		}
	};


	/*
	Esta clase define una condición que se evalúa a true si la  acción a la que pertenece
	ha terminado con éxito.
	*/
	class CConditionSuccess : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, Logic::CEntity* entity) 
		{ 
			return currentNode->getStatus() == CLatentAction::SUCCESS;
		}
	};


	/*
	Esta clase define una condición que se evalúa a true si la  acción a la que pertenece
	ha terminado con fallo.
	*/
	class CConditionFail : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, Logic::CEntity* entity) 
		{ 
			return currentNode->getStatus() == CLatentAction::FAIL;	
		}
	};


	/*
	Esta clase define una condición que se evalúa a true si la  entidad se encuentra en
	la posición indicada, con una tolerancia
	*/
	template <class TNode>
	class CConditionNear : public ICondition<TNode>
	{
	public:
		CConditionNear(Vector3 position, float tolerance) : m_position(position), m_tolerance(tolerance) {}
		bool check(CLatentAction* currentNode, Logic::CEntity* entity) 
		{ 
			Logic::Component::CTransform* transf =
				static_cast<Logic::Component::CTransform*>(entity->getComponentByName(Common::Data::TRANSFORM_COMP));
			return transf->getPosition().positionEquals(m_position, m_tolerance);
		}

	protected:
		Vector3 m_position;
		float m_tolerance;
	};

}

#endif
