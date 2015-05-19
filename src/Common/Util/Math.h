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

#ifndef __COMMON_UTIL_MATH_H
#define __COMMON_UTIL_MATH_H

#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <OgreMatrix3.h>
#include <OgreMatrix4.h>
#include <OgreRay.h>

typedef Ogre::Matrix4 Matrix4;
typedef Ogre::Matrix3 Matrix3;

typedef Ogre::Vector2 Vector2;
typedef	Ogre::Vector3 Vector3;
typedef	Ogre::Vector4 Vector4;

typedef Ogre::Quaternion Quaternion;

typedef Ogre::Ray Ray;

namespace Common
{
	namespace Util
	{
		namespace Math
		{
			static const float PI = float(4.0 * atan(1.0));
			
			static const float DEG2RAD = PI / 180.0f;
			static const float RAD2DEG = 180.0f / PI;

			static float fromDegreesToRadians(float deg) {return deg * DEG2RAD;}
			static float fromRadiansToDegrees(float rad) {return rad * RAD2DEG;}

			static Vector3 getDirection(float orientation)
			{ 
				return Vector3(-sin(orientation), 0, -cos(orientation));
			}

			static void rotate(const Vector3& turn, Matrix4& transform) 
			{
				Matrix3 rotation;
				transform.extract3x3Matrix(rotation);
                using namespace Ogre;
                Radian yaw, pitch, roll;
				rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
				rotation.FromEulerAnglesYXZ(yaw + Radian(turn.y), pitch + Radian(turn.x), roll + Radian(turn.z));
				transform = rotation;
			}

			static float getYaw(const Matrix4& transform) 
			{
				Matrix3 rotation;
				transform.extract3x3Matrix(rotation);
				Ogre::Radian yaw, pitch, roll;
				rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
				return yaw.valueRadians();
			}

			static float getPitch(const Matrix4& transform) 
			{
				Matrix3 rotation;
				transform.extract3x3Matrix(rotation);
				Ogre::Radian yaw, pitch, roll;
				rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
				return pitch.valueRadians();
			}

			static float getRoll(const Matrix4& transform) 
			{
				Matrix3 rotation;
				transform.extract3x3Matrix(rotation);
				Ogre::Radian yaw, pitch, roll;
				rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
				return roll.valueRadians();
			}

			static void setRotation(const Vector3& turn, Matrix4& transform) 
			{
				transform = Matrix3::IDENTITY;
				Math::rotate(turn,transform);
			}

            static void getRotation(const Matrix4& transform, Ogre::Real& yaw, Ogre::Real& pitch, Ogre::Real& roll)
            {
                Matrix3 rotation;
				transform.extract3x3Matrix(rotation);
                Ogre::Radian x, y, z;
                rotation.ToEulerAnglesXYZ(x, y ,z);
                pitch = x.valueRadians();
                yaw   = y.valueRadians();
                roll  = z.valueRadians();
            }

            static void getRotation(const Matrix4& transform, Vector3& rot)
            {
                Matrix3 rotation;
				transform.extract3x3Matrix(rotation);
                Ogre::Radian x, y, z;
                rotation.ToEulerAnglesXYZ(x, y ,z);
                rot = Vector3(x.valueRadians(), y.valueRadians(), z.valueRadians());
            }

			static Vector3 getDirection(const Matrix4& transform) 
			{
				return getDirection(getYaw(transform));
			}

            //--  Easing --
            static float easeOutElastic(float t, float amplitude, float period)
            {
                if ( t == 0 ) return 0;
                if ( t == 1 ) return 1;
 
                float s;
                if ( amplitude < 1 ) {
                    amplitude = 1;
                    s = period / 4;
                }
                else 
                    s = period / ( 2 * PI) * asin( 1 / amplitude );

                return (float)(amplitude * pow( 2, -10*t ) * sin((t-s) * (2 * PI) / period) + 1);
            }

            static float easeInElastic( float t, float amplitude, float period )
            {
                if ( t == 0 ) return 0;
                if ( t == 1 ) return 1;

                float s = .0f;
                if ( amplitude < 1 ) {
                    amplitude = 1;
                    s = period / 4.0f;
                }
                else
                    s = period / (2 * PI * asin( 1 / amplitude ));
 
                float t_adj = (t / 1) - 1;
                return (float)(-(amplitude * pow(2, 10 * t_adj) * sin((t_adj * 1-s) * (2*PI) / period )));
            }
		}
	}
}

#endif