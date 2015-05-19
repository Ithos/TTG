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

#ifndef __AI_PERCEPTIONLISTENER_H
#define __AI_PERCEPTIONLISTENER_H

namespace AI
{

	class CNotification;

	/*
	Interfaz que implementar�n aquellas clases que deseen ser notificadas cuando se active un
	sensor de una entidad de percepci�n
	*/
	class IPerceptionListener
	{
	public:
		/*
		M�todo invocado por el gestor de percepci�n cuando recibe una notificaci�n de un sensor
		de la entidad de percepci�n.
		@param notification Notificaci�n recibida
		*/
		virtual void notificationPerceived(CNotification* notification) = 0;

		/*
		Destructor
		*/
		virtual ~IPerceptionListener(void) {};
	};

}

#endif
