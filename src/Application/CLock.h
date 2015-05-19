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

#ifndef __APPLICATION_CLOCK_H
#define __APPLICATION_CLOCK_H

#include <OgreTimer.h>
#include <OgreRoot.h>

namespace Application 
{
	class CClock
	{
	public:
        static CClock& getInstance( Ogre::Root* root = nullptr )
        {
            static CClock obj(root);
            return obj;
        }

		~CClock() { }

		/**
		Hace avanzar la hora del reloj, para que getTime() y
        getLastFrameDuration() devuelvan los nuevos valores.
		*/
		void updateTime();

		/**
		Devuelve la hora (milisegundos) en el momento de la última invocación
		a updateTime(). La hora debe tomarse siempre relativa a otra anterior.
		*/
		const unsigned long& getTime() const { return m_lastTime; }

		/**
		Devuelve la duración del último frame, o lo que es lo mismo,
		el tiempo transcurrido entre las dos últimas invocaciones
		a updateTime(). Dos veces seguidas el resultado es indeterminado.
		*/
		const unsigned int& getLastFrameDuration() const { return m_lastFrameDuration; }

        /**
		 Método que devuelve la "hora física" del sistema
		 en milisegundos.
		 */
        unsigned long getPhysicalTime() { return m_timer->getMilliseconds(); }

    private:
         CClock(Ogre::Root* root) : 
            m_lastTime(0), m_lastFrameDuration(0), m_timer(root->getTimer()), m_newTime(0)
        {  }

		/**
		@brief last time recorded
		*/
		unsigned long m_lastTime;

		/**
		@brief Last frame duration
		*/
		unsigned int m_lastFrameDuration;

        /**
        Ogre timer
        */
        Ogre::Timer* m_timer;
        unsigned long m_newTime;
    };
}

#endif 
