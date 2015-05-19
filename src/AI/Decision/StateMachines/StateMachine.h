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
	Clase padre para las m�quinas de estado. Es una clase parametrizada. El par�metro es la
	clase de los elementos en los nodos. En general, este par�metro ser� una acci�n
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
		Constructor que recibe la entidad que ejecuta la m�quina de estado
		*/
		CStateMachine(Logic::CEntity* entity) : m_entity(entity), m_currentNodeId(-1), m_initialNodeId(-1) { m_edges = new EdgeList(); };

		/*
		Destructor
		*/
		~CStateMachine();

		/*
		Este m�todo a�ade un nodo a la m�quina de estado y devuelve un identificador del nodo.
		Este identificador se usa para referirse a los nodos al a�adir aristas y establecer el
		nodo inicial. Los nodos ser�n destruidos cuando se destruya la m�quina de estado.
		@param content Contenido del nodo.
		@return Identificador para el nodo.
		*/
		int addNode(TNode *content);

		/*
		A�ade una arista a la m�quina de estado. El m�todo recibe los identificadores del nodo de
		origen y destino y una condici�n que indica cu�ndo se activa la transici�n de la arista.
		Las condiciones ser�n destruidas cuando se destruya la m�quina de estado.
		@param idOrig Identificador del nodo de origen.
		@param idDest Identificador del nodo de destino.
		@param cond Condici�n asociada a la arista.
		*/
		void addEdge(int idOrig, int idDest, ICondition<TNode> *cond);

		/*
		Este m�todo comprueba las condiciones de las aristas que salen del  nodo actual y cambia
		de nodo si alguna de ellas es cierta. El m�todo devuelve true si alguna transici�n es
		cierta (aunque no se cambie el nodo actual) y false en otro caso. Las aristas se comprueban
		en el orden en que se han a�adido y la comprobaci�n se detiene en el momento en que una de
		ellas se hace cierta.
		*/
		bool nextState();

		/*
		Devuelve el contenido del nodo actual.
		*/
		TNode* getCurrentNode() { return m_nodes[m_currentNodeId]; };

		/*
		Establece cu�l es la entidad que ejecuta la m�quina de estado.
		*/
		void setEntity(Logic::CEntity *entity) { m_entity = entity; };

		/*
		Establece el nodo inicial.
		@param idNode Identificador del nodo inicial.
		*/
		void setInitialNode(int idNode) { m_initialNodeId = idNode; };

		/*
		Reinicia la ejecuci�n de la m�quina de estado.
		*/
		void resetExecution(){ m_currentNodeId = -1; };

	protected:
		/*
		Tipo que guarda la informaci�n de las aristas salientes de un nodo. Para cada arista
		tenemos un vector de pares con la condici�n y el nodo de destino.
		*/
		typedef std::vector<std::pair<ICondition<TNode>*, int>> PairVector;

		/*
		Tipo que guarda la informaci�n de todas las aristas. Est� indexado por el identificador
		del nodo de origen.
		*/
		typedef std::map<int, PairVector*> EdgeList;

		/*
		Entidad que ejecuta la m�quina de estado.
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
		formada por pares (condici�n, nodo destino). Por ejemplo, si tenemos una aristas que sale
		del nodo con id 1 y va hasta el 2 y su condici�n es c1 y otra que va desde 1 hasta 3 y su 
		condici�n es c2, la estructura que tendremos ser�:
			{ 1 -> [ (c1, 2), 
					 (c2, 3) ] }
		*/
		EdgeList* m_edges;
	};




	//  ----------------
	// | IMPLEMENTACI�N |
	//  ----------------

	template <class TNode>
	CStateMachine<TNode>::~CStateMachine() 
	{
		// Borramos las aristas
		for (EdgeList::iterator it = m_edges->begin(); it != m_edges->end(); it++)
		{
			PairVector* v = it->second;
			for (PairVector::iterator it2 = v->begin(); it2 != v->end(); it2++)
				// Borramos la condici�n
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
		// A�adimos el nuevo nodos a la lista de nodos en la �ltima posici�n
		int id = m_nodes.size();
		m_nodes[id] = content;

		// Devolver el id (la posici�n en la que lo hemos a�adido
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

		// A�adimos la condici�n y el nodo de destino a la lista
		vector->push_back(std::pair<ICondition<TNode>*, int>(cond, idDest));
	}



	template <class TNode>
	bool CStateMachine<TNode>::nextState()
	{
		// Si la m�quina no est� inicializada, el nodo actual ser� -1. En ese caso la inicializamos
		// asign�ndole al current el nodo inicial e indicando que s� que ha habido cambio de nodo
		if (m_currentNodeId == -1) {
			m_currentNodeId = m_initialNodeId;
			return true;
		}

		// En la lista de aristas (_edges) buscamos las que salen del nodo actual (m_edges est�
		// indexada por el nodo de origen de las aristas).
		EdgeList::iterator it = m_edges->find(m_currentNodeId);
		// Si hay alguna arista de salida
		if (it != m_edges->end()) {
			PairVector* vector = (*it).second;
			// Recorremos la lista de aristas de salida y para cada una de ellas
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++) {
				TNode* node = m_nodes[m_currentNodeId];
				// Comprobamos la condici�n de la arista (m�todo check)
				if (edgeIt->first->check(node, m_entity)) {
					// Si se verifica actualizamos el nodo actual y devolvemos true
					int newNode = edgeIt->second;
					m_currentNodeId = newNode;
					return true;
				}
			}
		}
		// Si no hay aristas o no se cumple ninguna condici�n devolvemos false
		return false;
	}

}

#endif
