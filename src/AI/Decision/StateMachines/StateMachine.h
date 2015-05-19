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

#ifndef __AI_STATEMACHINE_H
#define __AI_STATEMACHINE_H

#include "Condition.h"

namespace AI 
{
	/*
	Clase padre para las máquinas de estado. Es una clase parametrizada. El parámetro es la
	clase de los elementos en los nodos. En general, este parámetro será una acción
	ejecutable (CLatentAction).
	*/
	template <class TNode>
	class CStateMachine
	{
	public:
		/*
		Constructor
		*/
		CStateMachine() : m_entity(0), m_currentNodeId(-1), m_initialNodeId(-1) { m_edges = new EdgeList(); };

		/*
		Constructor que recibe la entidad que ejecuta la máquina de estado
		*/
		CStateMachine(Logic::CEntity* entity) : m_entity(entity), m_currentNodeId(-1), m_initialNodeId(-1) { m_edges = new EdgeList(); };

		/*
		Destructor
		*/
		~CStateMachine();

		/*
		Este método añade un nodo a la máquina de estado y devuelve un identificador del nodo.
		Este identificador se usa para referirse a los nodos al añadir aristas y establecer el
		nodo inicial. Los nodos serán destruidos cuando se destruya la máquina de estado.
		@param content Contenido del nodo.
		@return Identificador para el nodo.
		*/
		int addNode(TNode *content);

		/*
		Añade una arista a la máquina de estado. El método recibe los identificadores del nodo de
		origen y destino y una condición que indica cuándo se activa la transición de la arista.
		Las condiciones serán destruidas cuando se destruya la máquina de estado.
		@param idOrig Identificador del nodo de origen.
		@param idDest Identificador del nodo de destino.
		@param cond Condición asociada a la arista.
		*/
		void addEdge(int idOrig, int idDest, ICondition<TNode> *cond);

		/*
		Este método comprueba las condiciones de las aristas que salen del  nodo actual y cambia
		de nodo si alguna de ellas es cierta. El método devuelve true si alguna transición es
		cierta (aunque no se cambie el nodo actual) y false en otro caso. Las aristas se comprueban
		en el orden en que se han añadido y la comprobación se detiene en el momento en que una de
		ellas se hace cierta.
		*/
		bool nextState();

		/*
		Devuelve el contenido del nodo actual.
		*/
		TNode* getCurrentNode() { return m_nodes[m_currentNodeId]; };

		/*
		Establece cuál es la entidad que ejecuta la máquina de estado.
		*/
		void setEntity(Logic::CEntity *entity) { m_entity = entity; };

		/*
		Establece el nodo inicial.
		@param idNode Identificador del nodo inicial.
		*/
		void setInitialNode(int idNode) { m_initialNodeId = idNode; };

		/*
		Reinicia la ejecución de la máquina de estado.
		*/
		void resetExecution(){ m_currentNodeId = -1; };

	protected:
		/*
		Tipo que guarda la información de las aristas salientes de un nodo. Para cada arista
		tenemos un vector de pares con la condición y el nodo de destino.
		*/
		typedef std::vector<std::pair<ICondition<TNode>*, int>> PairVector;

		/*
		Tipo que guarda la información de todas las aristas. Está indexado por el identificador
		del nodo de origen.
		*/
		typedef std::map<int, PairVector*> EdgeList;

		/*
		Entidad que ejecuta la máquina de estado.
		*/
		Logic::CEntity* m_entity;

		/*
		Valores del nodo actual e inicial
		*/
		int m_currentNodeId, m_initialNodeId;

		/*
		Lista de nodos. Es un map que relaciona cada identificador de nodo con su contenido.
		*/
		std::map<int, TNode*> m_nodes;

		/*
		Lista de aristas. Es un map que asocia cada nodo de origen de cada arista con una lista
		formada por pares (condición, nodo destino). Por ejemplo, si tenemos una aristas que sale
		del nodo con id 1 y va hasta el 2 y su condición es c1 y otra que va desde 1 hasta 3 y su 
		condición es c2, la estructura que tendremos será:
			{ 1 -> [ (c1, 2), 
					 (c2, 3) ] }
		*/
		EdgeList* m_edges;
	};




	//  ----------------
	// | IMPLEMENTACIÓN |
	//  ----------------

	template <class TNode>
	CStateMachine<TNode>::~CStateMachine() 
	{
		// Borramos las aristas
		for (EdgeList::iterator it = m_edges->begin(); it != m_edges->end(); it++)
		{
			PairVector* v = it->second;
			for (PairVector::iterator it2 = v->begin(); it2 != v->end(); it2++)
				// Borramos la condición
				delete it2->first;
			delete v;
		}
		// Borramos el vector
		delete m_edges;

		// Borramos los nodos
		for (std::map<int, TNode*>::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
			delete it->second;
	}



	template <class TNode>
	int CStateMachine<TNode>::addNode(TNode* content)
	{
		// Añadimos el nuevo nodos a la lista de nodos en la última posición
		int id = m_nodes.size();
		m_nodes[id] = content;

		// Devolver el id (la posición en la que lo hemos añadido
		return id;
	}



	template <class TNode>
	void CStateMachine<TNode>::addEdge(int idOrig, int idDest, ICondition<TNode> *cond)
	{
		// Buscamos en la lista de aristas las que salen de idOrig
		EdgeList::iterator it = m_edges->find(idOrig);
		PairVector* vector = NULL;

		// Si no hay ninguna arista saliente para ese nodo tendremos que crear un nuevo PairVector
		// (que es la estructura donde se almacenan las aristas salientes) y asignarla.
		if (it == m_edges->end()) {
			vector = new PairVector();
			(*m_edges)[idOrig] = vector;
		}
		// Si ya hay una lista la sacamos
		else
			vector = (*it).second;

		// Añadimos la condición y el nodo de destino a la lista
		vector->push_back(std::pair<ICondition<TNode>*, int>(cond, idDest));
	}



	template <class TNode>
	bool CStateMachine<TNode>::nextState()
	{
		// Si la máquina no está inicializada, el nodo actual será -1. En ese caso la inicializamos
		// asignándole al current el nodo inicial e indicando que sí que ha habido cambio de nodo
		if (m_currentNodeId == -1) {
			m_currentNodeId = m_initialNodeId;
			return true;
		}

		// En la lista de aristas (_edges) buscamos las que salen del nodo actual (m_edges está
		// indexada por el nodo de origen de las aristas).
		EdgeList::iterator it = m_edges->find(m_currentNodeId);
		// Si hay alguna arista de salida
		if (it != m_edges->end()) {
			PairVector* vector = (*it).second;
			// Recorremos la lista de aristas de salida y para cada una de ellas
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++) {
				TNode* node = m_nodes[m_currentNodeId];
				// Comprobamos la condición de la arista (método check)
				if (edgeIt->first->check(node, m_entity)) {
					// Si se verifica actualizamos el nodo actual y devolvemos true
					int newNode = edgeIt->second;
					m_currentNodeId = newNode;
					return true;
				}
			}
		}
		// Si no hay aristas o no se cumple ninguna condición devolvemos false
		return false;
	}

}

#endif
