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

#include "LAGoTo.h"
#include "AI/Decision/StateMachines/StateMachine.h"
#include "Logic/Entity/Components/AI/SteeringMovement.h"
#include "Application/BaseApplication.h"
#include "Common/Data/TTG_Types.h"
#include "Common/Configure/Configure.h"

#include <random>

namespace AI 
{
	const float NEG_BOUNDARY = -3000.0f;
	const float POS_BOUNDARY = 3000.0f;
	const float Y_PLANE = -300.0f;

	/*
	M�todo invocado al principio de la ejecuci�n de la acci�n, para que se realicen las tareas
	que son �nicamente necesarias al principio (y no durante toda la vida de la acci�n).
	@return Estado al que pasa la acci�n; si se indica que la acci�n ha terminado
	(LatentAction::Completed), se invocar� al OnStop().
	*/
	CLatentAction::LAStatus CLAGoToRandom::OnStart()
	{
		Logic::Component::CSteeringMovement* steering =
			static_cast<Logic::Component::CSteeringMovement*>(m_entity->getComponentByName(Common::Data::STEERING_MOV));
		Logic::Component::CTransform* transform =
			static_cast<Logic::Component::CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));
		m_weapons = static_cast<Logic::Component::CWeapons*>(m_entity->getComponentByName(Common::Data::WEAPONS_COMP));

		unsigned seed = Application::CBaseApplication::getInstance()->getApptime() + transform->getPosition().x;
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution(NEG_BOUNDARY,POS_BOUNDARY);

		m_target.x = distribution(generator);
		m_target.y = Y_PLANE;
		m_target.z = distribution(generator);

		steering->setTarget(m_target);

		return RUNNING;
	}

	/*
	M�todo invocado al final de la ejecuci�n de la acci�n, para que se realicen las tareas que
	son �nicamente necesarias al final (y no durante toda la vida de la acci�n). En la mayor�a de
	los casos este m�todo no hace nada.
	*/
	void CLAGoToRandom::OnStop()
	{
	}

	/*
	M�todo invocado c�clicamente para que se contin�e con la ejecuci�n de la acci�n.
	@return Estado de la acci�n tras la ejecuci�n del m�todo; permite indicar si la acci�n ha
	terminado o se ha suspendido, o si sigue en ejecuci�n.
	*/
	CLatentAction::LAStatus CLAGoToRandom::OnRun() 
	{
		Logic::Component::CTransform* transf =
			static_cast<Logic::Component::CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));
		return (transf->getPosition().positionEquals(m_target, m_tolerance)) ? SUCCESS : RUNNING;
	}

	/*
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento al que pertenece se
	ha abortado por cualquier raz�n). La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada". IMPORTANTE: el Abort NO provoca la
	ejecuci�n de OnStop().
	@return Estado de la acci�n tras la ejecuci�n del m�todo.
	*/
	CLatentAction::LAStatus CLAGoToRandom::OnAbort() 
	{
		return FAIL;
	}

	/*
	M�todo invocado al principio de la ejecuci�n de la acci�n, para que se realicen las tareas
	que son �nicamente necesarias al principio (y no durante toda la vida de la acci�n).
	@return Estado al que pasa la acci�n; si se indica que la acci�n ha terminado
	(LatentAction::Completed), se invocar� al OnStop().
	*/
	CLatentAction::LAStatus CLAGoTo::OnStart()
	{
		Logic::Component::CSteeringMovement* steering =
			static_cast<Logic::Component::CSteeringMovement*>(m_entity->getComponentByName(Common::Data::STEERING_MOV));
		steering->setTarget(this->getTarget());

		return RUNNING;
	}

	/*
	M�todo invocado c�clicamente para que se contin�e con la ejecuci�n de la acci�n.
	@return Estado de la acci�n tras la ejecuci�n del m�todo; permite indicar si la acci�n ha
	terminado o se ha suspendido, o si sigue en ejecuci�n.
	*/
	CLatentAction::LAStatus CLAShoottingGoTo::OnRun()
	{
		Logic::Component::CTransform* transf =
			static_cast<Logic::Component::CTransform*>(m_entity->getComponentByName(Common::Data::TRANSFORM_COMP));

		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution(0,100);
//		if (distribution(generator) < 50)
		if (m_frequency >=1000)
		{
			m_weapons->shootSecondaryWeapon();
			m_frequency = 0;
		}

		m_frequency++;

		return (transf->getPosition().positionEquals(m_target, m_tolerance)) ? SUCCESS : RUNNING;
	}

}
