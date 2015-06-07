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

#include "AI.h"
#include "Common\Util\Math.h"

namespace AI {

	CAI* CAI::m_instance = 0;

	CAI::CAI()
	{
		m_instance = this;
		m_pManager = new CPerceptionManager();
	}


	CAI::~CAI()
	{
		m_instance = 0;
		delete m_pManager;
	}


	bool CAI::init() 
	{
		m_instance = new CAI();
		return true;
	}


	void CAI::release()
	{
		if (m_instance)
			delete m_instance;
	}


	bool CAI::tick(unsigned int msecs)
	{
		m_pManager->update(msecs);
		return true;
	}


	double CAI::correctAngle(double angle)
	{
		while (angle > Common::Util::Math::PI)
			angle -= 2 * Common::Util::Math::PI;
		while (angle < -Common::Util::Math::PI)
			angle += 2 * Common::Util::Math::PI;
		return angle;
	}

}