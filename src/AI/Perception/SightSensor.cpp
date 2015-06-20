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

#include "SightSensor.h"

#include "PerceptionEntity.h"
#include "PerceptionSignal.h"

//#include "Physics/Server.h"

namespace AI 
{
	/*
	Destructor
	*/
	CSightSensor::~CSightSensor()
	{
	}

	/*
	Realiza todas las comprobaciones necesarias para averiguar si la se�al recibida
	se encuentra dentro del cono de visi�n.
	@param perceptible Se�al cuya percepci�n queremos comprobar
	@param time Instante en el que se realiza la comprobaci�n de percepci�n
	@return NULL si no se ha percibido la se�al. Una instancia de CNotification en caso contrario.
	*/
	CNotification* CSightSensor::perceives(const CPerceptionSignal * signal, unsigned long time) 
	{
		// Realizamos las comprobaciones
		// Si alguna de ellas falla tendremos que devolver NULL para indicar que no ha habido percepci�n
		// Si todas tienen �xito devolveremos una instancia de CNotification

		// Comprobaciones: 
		// 1. Comprueba si el sensor y la se�al est�n activos 
		if (!this->isActive() || !signal->isActive())
			return NULL;

		// 2. Comprueba si el tipo de la se�al se corresponde con el tipo que percibe el sensor
		if (this->getType() != signal->getType())
			return NULL;

		// 3. Comprueba si la intensidad de la se�al est� por encima del threshold del sensor.
		if (signal->getIntensity() < this->getThreshold())
			return NULL;

		// 4. Comprueba la distancia entre la se�al y el sensor. Se realiza en 2 pasos:
		// 4.1. Primero se comprueba si la distancia en cada dimensi�n es mayor que la distancia m�xima.
		Matrix4 transform = this->getPerceptionEntity()->getTransform();
		Vector3 sensorPosition = transform.getTrans();
		Vector3 signalPosition = signal->getPerceptionEntity()->getTransform().getTrans();
		Vector3 distance = signalPosition - sensorPosition;
		if (distance.length() > m_maxDistance)//abs(distance.x) > m_maxDistance || abs(distance.y) > m_maxDistance || abs(distance.z) > m_maxDistance
			return NULL;

		// 4.2. A continuaci�n se comprueba si la distancia eucl�dea es mayor que la distancia m�xima.
//		float distMagnitude = distance.length();
//		if (distMagnitude > m_maxDistance)
//			return NULL;

		// 5. Comprobamos si la se�al se encuentra dentro del �ngulo de amplitud del cono de visi�n.
		// Tenemos que calcular el �ngulo que se forma entre la direcci�n hacia la que apunta la entidad (su orientaci�n) y
		// la direcci�n en la que se encuentra el sensor (con respecto a la entidad)
		// Sacamos la orientaci�n de la entidad (getYaw nos da el �ngulo de la entidad con respecto al eje Z)
		// Sacamos la orientaci�n de la se�al = orientaci�n del vector distancia
		// Y sacamos la diferencia 
		// Nos aseguramos de que el �ngulo es menor que PI (nos tenemos que quedar con la parte peque�a de la diferencia)
		// Sacamos la orientaci�n de la entidad 
//		float yaw = Common::Util::Math::getYaw(transform);					// Esta parte es para calcular el cono de visi�n,
//		Ogre::Radian angle = Ogre::Math::ATan2(-distance.x, -distance.z);	// quiz�s para naves no hace falta porque ven en 360�
//		float finalAngle = abs(yaw - angle.valueRadians());
//		if (finalAngle > Ogre::Math::PI)
//			finalAngle = Ogre::Math::TWO_PI - finalAngle;
//		if (finalAngle > m_alpha)
//			return NULL;
	
		// 6. Comprobamos si no existe ning�n objeto f�sico entre el sensor y la se�al. Para eso usamos un rayo f�sico.
		// Necesitamos
		// � la posici�n de origen del rayo = posici�n del sensor
		// � la direcci�n (normalizada) = vector distance normalizado
		// � la distancia m�xima = magnitud del vector distance
		// Con estos par�metros creamos una instancia de Ogre::Ray
		// Y lo usamos con raycastClosest. 
		// Si el rayo colisiona con alg�n objeto f�sico es que hay alg�n tipo de pared entre el sensor 
		// y la se�al, por lo que la entidad que ha emitido la se�al no se puede percibir.
		// Si hay alg�n obst�culo ==> raycastClosest nos devuelve la referencia ==> return NULL
//		Ray ray = Ray(sensorPosition, distance / distMagnitude); // El segundo par�metro es normalizado, lo hacemos as� porque ya tenemos calculada su magnitud pero tambi�n se podr�a hacer como distance.getNormaliceCopy()
//		Logic::CEntity* entity = Physics::CServer::getSingletonPtr()->raycastClosest(ray, distMagnitude); // �CUIDADO! Este rayo colisiona tambi�n contra los triggers. Habr�a que comprobar que s�lo colisionara con el conjunto de colisi�n
//		if (entity != NULL)
//			return NULL;

		// Si todos los chequeos han tenido �xito tenemos que devolver una nueva instancia de CNotification
		// Los par�metros que necesita son:
		// � El instante en el que hay que entregarla (ahora mismo ==> time)
		// � El sensor que lo ha percibido (this)
		// � La entidad de percepci�n que ha generado la se�al
		// Devolvemos la notificaci�n para que se env�e a la entidad
		return new CNotification(time,this,signal->getPerceptionEntity());
	}

}
