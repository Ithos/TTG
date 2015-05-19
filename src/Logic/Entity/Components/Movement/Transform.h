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

#ifndef __LOGIC_TRANSFORM_H
#define __LOGIC_TRANSFORM_H

#include "../Component.h"
#include <Common/Util/Math.h>

namespace Logic
{
	namespace Component
	{
		class CTransform : public IComponent
		{
			DEC_FACTORY(CTransform);

		public:
			CTransform() : IComponent(), m_transform(Matrix4::IDENTITY) {};
			virtual ~CTransform() {};
			virtual bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

			void setTransform(Matrix4 transform) { m_transform = transform;   }
            void setPosition(const Vector3& pos) { m_transform.setTrans(pos); }

			const Vector3& getPosition() { return m_transform.getTrans();}
            Matrix4& getTransform()      { return m_transform; }
			Quaternion getOrientation()  { return m_transform.extractQuaternion(); }
            float getYaw() const         { return Common::Util::Math::getYaw(m_transform); }

            void rotateDeg(float yaw, float pitch = 0, float roll = 0)
            {
                using namespace Common::Util::Math;
		        Common::Util::Math::rotate(Vector3(fromDegreesToRadians(pitch), fromDegreesToRadians(yaw), fromDegreesToRadians(roll)), m_transform);
            }

            void rotateRad(float yaw, float pitch = .0f, float roll = .0f)
            {
                Common::Util::Math::rotate(Vector3(pitch, yaw, roll), m_transform);
            }

		private:
			Matrix4 m_transform;
		};

	REG_FACTORY(CTransform);
	}
}

#endif
