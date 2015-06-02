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

#ifndef __COMMON_SOUND_SOUND_H
#define __COMMON_SOUND_SOUND_H

#include <string>
#include <vector>
#include <map>

#include <OGRE\OgreVector3.h>

namespace FMOD
{
	class Sound;
	class System;

	namespace Studio
	{
		class Bank;
		class System;
		class EventInstance;
		class ParameterInstance;
	}
}

namespace Ogre{
	class Matrix4;
}

namespace Common
{
	namespace Sound
	{
		class CSound
		{
			///TODO --mirar tipos de rolloff--///
		public:
			static bool init();
			static void release();
			void tick(unsigned int msecs);
			static CSound* getSingletonPtr(){return m_instance;};

			/**
			Adds a sound effect to the map
			*/
			bool addSound(const std::string& fileName, const std::string& soundName);

			/**
			Adds a long duration tune to be played as a stream
			*/
			bool addMusic(const std::string& fileName, const std::string& musicName, bool loop = false);

			/**
			Adds a sound which can be located in the space
			*/
			bool add3dSound(const std::string& fileName, const std::string& soundName, bool loop = false);

			/**
			Releases the chosen sound and erases it from the sounds map.
			*/
			void releaseSound(const std::string& soundName);

			/**
			Releases the chosen located sound
			*/
			void release3dSound(const std::string& soundName);

			/**
			Releases the chosen tune and erases it from the music map
			*/
			void releaseMusic(const std::string& musicName);

			/**
			Plays the chosen sound
			*/
			void playSound(const std::string& soundName, float volume = GLOBAL_MENU_SOUNDS_VOLUME, bool release = false);

			/**
			Adds sounds to the delayed list
			*/
			void playDelayedSound(const std::string& soundName, int milis);

			/**
			Updates delayed sounds
			*/

			void updateDelayed(unsigned int msecs);

			/**
			Plays the chosen tune
			*/
			void playMusic(const std::string& musicName, float volume = GLOBAL_MUSIC_VOLUME);

			/**
			Plays the chosen tune
			*/
			void play3dSound(const std::string& soundName,const Ogre::Matrix4& transform, float volume = GLOBAL_EFFECTS_VOLUME);

			/**
			Pauses/Resumes the chosen tune
			*/
			void pauseResumeMusic(const std::string& musicName);

			/**
			Pauses/Resumes the chosen located sound
			*/
			void pauseResume3dSound(const std::string& soundName);

			/**
			Stops the chosen tune
			*/
			void stopMusic(const std::string& musicName);

			/**
			Stops the chosen located sound
			*/
			void stop3dSound(const std::string& soundName);

			/**
			To be called in the listener's tick
			Note: time is expected in seconds
			*/
			void updateListener(const Ogre::Matrix4& transform, float msecs);

			/**
			To be called in the emitter's tick
			*/
			void update3dSound(const std::string& soundName, const Ogre::Matrix4& transform);

			/**
			stop all
			*/
			void stopAllSounds();

			/**
			pause/resume all
			*/
			void pauseResumeAll();

			/**
			Set 3D channel parameters
			Note: soundName channel has to be active when this method is called
			*/
			void set3DMinMaxDistance(const std::string& soundName, float minDistance, float maxDistance);

			/**
			Loads a bank and adds it to the map
			*/
			bool addBank(const std::string& fileName, const std::string& bankName);

			/**
			Loads and instances an event
			*/
			bool addEvent(const std::string& eventString, const std::string& eventName);

			/**
			Gets a parameter from an event instance and adds it to the map
			*/
			bool addParameter(const std::string& eventName, const std::string& paramString, const std::string& paramName);

			/**
			Starts an event instance
			*/
			void startEvent(const std::string& eventName);

			/**
			Stops an event instance
			*/
			void stopEvent(const std::string& eventName);

			/**
			Releases an event instance
			*/
			void releaseEvent(const std::string& eventName);

			/**
			Sets the value of a parameter
			*/
			void setParamValue(const std::string& paramName, float value);

			/**
			Gets the value of a parameter
			*/
			float getParamValue(const std::string& paramName);

			/**
			Erases a parameter from the map
			*/
			void releaseParam(const std::string& paramName);

			/**
			Releases the chosen bank and erases it from the map
			*/
			void releaseBank(const std::string& bankName);

			/**
			Returns true if the chosen sound is being played
			*/
			bool isPlaying(const std::string& soundName){return (m_channels.find(soundName) != m_channels.end());}

			/**
			Returns true if the chosen music is in the map
			*/
			bool hasMusic(const std::string& soundName){return (m_music.find(soundName) != m_music.end());}

		protected:
			CSound();
			~CSound();
			static CSound* m_instance;

			//This is here because we might want to change the volume of the effects and the music
			static float GLOBAL_MENU_SOUNDS_VOLUME;
			static float GLOBAL_MUSIC_VOLUME;
			static float GLOBAL_EFFECTS_VOLUME;

			/**
			Map that contains all sounds
			*/
			std::map<std::string, FMOD::Sound*> m_sounds;

			/**
			Map that contains all 3d sounds
			*/
			std::map<std::string, FMOD::Sound*> m_3dsounds;

			/**
			Map that contains the loaded banks
			*/
			std::map<std::string, FMOD::Studio::Bank*> m_banks;

			/**
			Map that contains the loaded events
			*/
			std::map<std::string, FMOD::Studio::EventInstance*> m_events;

			/**
			Map that contains the loaded prameters
			*/
			std::map<std::string, FMOD::Studio::ParameterInstance*> m_params;

			/**
			Map that contains the positions of the located sounds
			*/
			std::map<std::string,std::vector<Ogre::Vector3> > m_3dpos;

			/**
			Map that contains the long duration tunes
			*/
			std::map<std::string, FMOD::Sound*> m_music;

			/**
			Map that contains the delayed sounds
			*/
			std::map<std::string, int> m_delayed;

			/**
			Map that contains the used channels ids
			*/
			std::map<std::string, int> m_channels;

			/**
			FMOD instance
			*/
			FMOD::System *m_systemFMOD;
			FMOD::Studio::System *m_studioFMOD;

			/**
			Previous listener's position, this is used to calculate the velocity of the listener
			*/
			Ogre::Vector3 m_prevListenerPos;
			
			/**
			Data referring to the listeners position
			Note: this data is supposed to not change once setted to a value
			*/
			Ogre::Vector3 m_listenerForward, m_listenerUp;

			bool open();

			void close();

			bool ERRCHECK(int result);
		};
	}
}

#endif
