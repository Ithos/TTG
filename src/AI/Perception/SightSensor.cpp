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
	Realiza todas las comprobaciones necesarias para averiguar si la señal recibida
	se encuentra dentro del cono de visión.
	@param perceptible Señal cuya percepción queremos comprobar
	@param time Instante en el que se realiza la comprobación de percepción
	@return NULL si no se ha percibido la señal. Una instancia de CNotification en caso contrario.
	*/
	CNotification* CSightSensor::perceives(const CPerceptionSignal * signal, unsigned long time) 
	{
		// Realizamos las comprobaciones
		// Si alguna de ellas falla tendremos que devolver NULL para indicar que no ha habido percepción
		// Si todas tienen éxito devolveremos una instancia de CNotification

		// Comprobaciones: 
		// 1. Comprueba si el sensor y la señal están activos 
		if (!this->isActive() || !signal->isActive())
			return NULL;

		// 2. Comprueba si el tipo de la señal se corresponde con el tipo que percibe el sensor
		if (this->getType() != signal->getType())
			return NULL;

		// 3. Comprueba si la intensidad de la señal está por encima del threshold del sensor.
		if (signal->getIntensity() < this->getThreshold())
			return NULL;

		// 4. Comprueba la distancia entre la señal y el sensor. Se realiza en 2 pasos:
		// 4.1. Primero se comprueba si la distancia en cada dimensión es mayor que la distancia máxima.
		Matrix4 transform = this->getPerceptionEntity()->getTransform();
		Vector3 sensorPosition = transform.getTrans();
		Vector3 signalPosition = signal->getPerceptionEntity()->getTransform().getTrans();
		Vector3 distance = signalPosition - sensorPosition;
		if (distance.length() > m_maxDistance)//abs(distance.x) > m_maxDistance || abs(distance.y) > m_maxDistance || abs(distance.z) > m_maxDistance
			return NULL;

		// 4.2. A continuación se comprueba si la distancia euclídea es mayor que la distancia máxima.
//		float distMagnitude = distance.length();
//		if (distMagnitude > m_maxDistance)
//			return NULL;

		// 5. Comprobamos si la señal se encuentra dentro del ángulo de amplitud del cono de visión.
		// Tenemos que calcular el ángulo que se forma entre la dirección hacia la que apunta la entidad (su orientación) y
		// la dirección en la que se encuentra el sensor (con respecto a la entidad)
		// Sacamos la orientación de la entidad (getYaw nos da el ángulo de la entidad con respecto al eje Z)
		// Sacamos la orientación de la señal = orientación del vector distancia
		// Y sacamos la diferencia 
		// Nos aseguramos de que el ángulo es menor que PI (nos tenemos que quedar con la parte pequeña de la diferencia)
		// Sacamos la orientación de la entidad 
//		float yaw = Common::Util::Math::getYaw(transform);					// Esta parte es para calcular el cono de visión,
//		Ogre::Radian angle = Ogre::Math::ATan2(-distance.x, -distance.z);	// quizás para naves no hace falta porque ven en 360º
//		float finalAngle = abs(yaw - angle.valueRadians());
//		if (finalAngle > Ogre::Math::PI)
//			finalAngle = Ogre::Math::TWO_PI - finalAngle;
//		if (finalAngle > m_alpha)
//			return NULL;
	
		// 6. Comprobamos si no existe ningún objeto físico entre el sensor y la señal. Para eso usamos un rayo físico.
		// Necesitamos
		// · la posición de origen del rayo = posición del sensor
		// · la dirección (normalizada) = vector distance normalizado
		// · la distancia máxima = magnitud del vector distance
		// Con estos parámetros creamos una instancia de Ogre::Ray
		// Y lo usamos con raycastClosest. 
		// Si el rayo colisiona con algún objeto físico es que hay algún tipo de pared entre el sensor 
		// y la señal, por lo que la entidad que ha emitido la señal no se puede percibir.
		// Si hay algún obstáculo ==> raycastClosest nos devuelve la referencia ==> return NULL
//		Ray ray = Ray(sensorPosition, distance / distMagnitude); // El segundo parámetro es normalizado, lo hacemos así porque ya tenemos calculada su magnitud pero también se podría hacer como distance.getNormaliceCopy()
//		Logic::CEntity* entity = Physics::CServer::getSingletonPtr()->raycastClosest(ray, distMagnitude); // ¡CUIDADO! Este rayo colisiona también contra los triggers. Habría que comprobar que sólo colisionara con el conjunto de colisión
//		if (entity != NULL)
//			return NULL;

		// Si todos los chequeos han tenido éxito tenemos que devolver una nueva instancia de CNotification
		// Los parámetros que necesita son:
		// · El instante en el que hay que entregarla (ahora mismo ==> time)
		// · El sensor que lo ha percibido (this)
		// · La entidad de percepción que ha generado la señal
		// Devolvemos la notificación para que se envíe a la entidad
		return new CNotification(time,this,signal->getPerceptionEntity());
	}

}
