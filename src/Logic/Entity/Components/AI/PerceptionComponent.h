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

#ifndef __LOGIC_PERCEPTIONCOMPONENT_H
#define __LOGIC_PERCEPTIONCOMPONENT_H

#include "Logic/Entity/Components/Component.h"
#include "AI/Perception/PerceptionListener.h"

namespace AI {
	class CPerceptionEntity;
}

namespace Logic
{
	namespace Component
	{
		/*
		Este componente se encarga de las tareas relacionadas con la percepci�n.
		Al cargar las propiedades del mapa, se ocupa de crear una entidad de percepci�n
		(AI::CPerceptionEntity), que estar� asociada a la entidad l�gica. En esta entidad
		de percepci�n se definen los sensores que posee la entidad y las se�ales que
		genera en cada tick.
		La entidad de percepci�n se registra en el gestor de percepci�n (AI::CPerceptionManager)
		para poder percibir y ser percibida.
		Cuando uno de los sensores de la entidad percibe una se�al apropiada, se invoca
		al m�todo notificationPerceived que, en este caso, env�a un mensaje con los
		detalles de la percepci�n realizada.
		*/
		class CPerceptionComponent: public IComponent, public AI::IPerceptionListener
		{
			DEC_FACTORY(CPerceptionComponent);

		public:
			/*
			Constructor
			*/
			CPerceptionComponent(void) : m_pEntity(NULL) {};

			/*
			Destructor
			*/
			~CPerceptionComponent(void);

			/*
			Inicializaci�n del componente, utilizando la informaci�n extra�da de
			la entidad le�da del mapa (Maps::CEntity).
			En el spawn, se crea una entidad de percepci�n (AI::CPerceptionEntity) y se
			registra en el gestor de percepci�n (AI::CPerceptionManager).
			En este caso utilizaremos los atributos:
			� perception_entity_type: tipo de la entidad de percepci�n. Es una manera
			r�pida y sencilla de definir qu� sensores y se�ales tiene la entidad y cu�les
			son sus propiedades, pero no es muy flexible.
			� physic_radius: utilizamos el radio de la c�psula f�sica como radio de la entidad
			de percepci�n.
			@param entity Entidad a la que pertenece el componente.
			@param map Mapa L�gico en el que se registrar� el objeto.
			@param entityInfo Informaci�n de construcci�n del objeto le�do del fichero de disco.
			@return Cierto si la inicializaci�n ha sido satisfactoria.
			*/
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

			/*
			M�todo que activa el componente; invocado cuando se activa
			el mapa donde est� la entidad a la que pertenece el componente.
			En este caso, se registra el componente en el gestor de percepci�n (AI::PerceptionManager)
			@return true si todo ha ido correctamente.
			*/
			virtual bool activate();

			/*
			M�todo que desactiva el componente; invocado cuando se
			desactiva el mapa donde est� la entidad a la que pertenece el
			componente. Se invocar� siempre, independientemente de si estamos
			activados o no.
			Al desactivar el componente la entidad de percepci�n se "desregistra"
			del gestor de percepci�n.
			*/
			virtual void deactivate();

			/*
			M�todo llamado en cada frame que actualiza el estado del componente.
			En cada tick se comprueba si la entidad l�gica ha modificado su matriz de transformaci�n
			y, si es as�, se actualiza la matriz de transformaci�n de la entidad de percepci�n.
			@param msecs Milisegundos transcurridos desde el �ltimo tick.
			*/
			virtual void tick(unsigned int msecs);

			/*
			M�todo invocado por el gestor de percepci�n cuando recibe una notificaci�n de un
			sensor de la entidad de percepci�n.
			En este m�todo se incluir�n las acciones que queremos que el componente realice cuando
			alguno de los sensores asociados percibe una se�al.
			@param notification Notificaci�n recibida
			*/
			virtual void notificationPerceived(AI::CNotification* notification);

		private:
			/*
			Entidad de percepci�n que se registra en el CPerceptionManager
			*/
			AI::CPerceptionEntity* m_pEntity;

		};

		REG_FACTORY(CPerceptionComponent);
	}
}

#endif
