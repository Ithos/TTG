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

#include "Sound.h"

#include "../../Common/Configure/Configure.h"

#include <assert.h>
#include <vector>

#include <log.h>

#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

#include <OGRE\OgreMatrix4.h>

namespace Common
{
	namespace Sound
	{
		CSound *CSound::m_instance = nullptr;
		float CSound::GLOBAL_MENU_SOUNDS_VOLUME = 0.7f;
		float CSound::GLOBAL_MUSIC_VOLUME = 0.7f;
		float CSound::GLOBAL_EFFECTS_VOLUME = 0.7f;

		const char* const LOG_CSOUND = "Common::Sound::CSound";

		bool CSound::init()
		{
			assert(m_instance == nullptr);
			m_instance = new CSound();
			if (!m_instance->open()) {
				release();
				return false;
			}

			return true;
		}

		void CSound::release()
		{
			assert(m_instance);
			delete m_instance;
			m_instance = nullptr;
		}

		void CSound::tick(unsigned int msec)
		{
			m_studioFMOD->update();
			m_systemFMOD->update();
			if(msec>100)
				msec=100;
			updateDelayed(msec);

		}

		void CSound::updateListener(const Ogre::Matrix4& transform, float msecs)
		{
			FMOD_VECTOR    listenerPosition;
			FMOD_VECTOR    listenerForward;
			FMOD_VECTOR    listenerUp;
			FMOD_VECTOR    listenerVelocity;
			Ogre::Vector3  vectorVelocity(0.0,0.0,0.0);

			if (msecs > 0)
				vectorVelocity = (transform.getTrans() - m_prevListenerPos) / msecs;
			
			listenerPosition.x = transform.getTrans().x;
			listenerPosition.y = transform.getTrans().y;
			listenerPosition.z = transform.getTrans().z;

			listenerVelocity.x = vectorVelocity.x;//vectorVelocity.x
			listenerVelocity.y = vectorVelocity.y;//vectorVelocity.y
			listenerVelocity.z = vectorVelocity.z;//vectorVelocity.z

			listenerForward.x = m_listenerForward.x;
			listenerForward.y = m_listenerForward.y;
			listenerForward.z = m_listenerForward.z;

			listenerUp.x = m_listenerUp.x;
			listenerUp.y = m_listenerUp.y;
			listenerUp.z = m_listenerUp.z;

			m_systemFMOD->set3DListenerAttributes(0, &listenerPosition, &listenerVelocity, &listenerForward, &listenerUp);
			m_systemFMOD->update();

			m_prevListenerPos = transform.getTrans();

			//Update 3dsounds channels
			for(auto it=m_3dsounds.begin(); it!=m_3dsounds.end(); ++it){
				FMOD::Channel* canal = NULL;

				if(m_channels.find(it->first) == m_channels.end())
					continue;

				FMOD_RESULT result = m_systemFMOD->getChannel(m_channels[it->first],&canal);

				assert(m_3dpos.find(it->first) != m_3dpos.end() && "Sound position not found.");

				if (msecs > 0)
					vectorVelocity = (m_3dpos[it->first][0] - m_3dpos[it->first][1])/msecs;
				else
					vectorVelocity = Ogre::Vector3(0.0,0.0,0.0);

				listenerPosition.x = m_3dpos[it->first][0].x;
				listenerPosition.y = m_3dpos[it->first][0].y;
				listenerPosition.z = m_3dpos[it->first][0].z;

				listenerVelocity.x = vectorVelocity.x;
				listenerVelocity.y = vectorVelocity.y;
				listenerVelocity.z = vectorVelocity.z;

				 canal->set3DAttributes(&listenerPosition, &listenerVelocity);
				 m_3dpos[it->first][1] = m_3dpos[it->first][0];
			}

		}

		void CSound::update3dSound(const std::string& soundName, const Ogre::Matrix4& transform)
		{
			if(m_channels.find(soundName) != m_channels.end()){
				assert("3dsound not found." && m_3dpos.find(soundName) != m_3dpos.end());
				std::vector<Ogre::Vector3> tmpVec(m_3dpos[soundName]);
				tmpVec[1]=tmpVec[0];//Save previous location
				tmpVec[0]=transform.getTrans();//Get new location
				m_3dpos[soundName]=tmpVec;
			}
		}

		void CSound::updateDelayed(unsigned int msecs)
		{
			std::vector<std::string> vec;
			for(auto it=m_delayed.begin(); it!=m_delayed.end(); ++it){
				it->second -= msecs;
				if(it->second <= 0)
				{
					playSound(it->first);
					vec.push_back(it->first);
				}
			}

			for(unsigned int i=0; i< vec.size();++i)
			{
				if(m_delayed[vec[i]])
					m_delayed.erase(vec[i]);
			}
		}

		bool CSound::addSound(const std::string& fileName, const std::string& soundName)
		{
			assert(!m_sounds[soundName] && "Sound name repeated.");

			Common::Configuration::setDefaultFile(CONFIGURE_FILE);
			std::string soundPath(Common::Configuration::getDefaultValue(SOUNDS_PATH));
			soundPath = soundPath + fileName;

			FMOD::Sound* sound;
			FMOD_RESULT result = m_systemFMOD->createSound(soundPath.c_str(), FMOD_DEFAULT, 0, &sound);
			if(ERRCHECK(result))
				return false;

			m_sounds[soundName] = sound;

			return true;
		}

		bool CSound::addMusic(const std::string& fileName, const std::string& musicName, bool loop) 
		{
			assert(!m_music[musicName] && "Music name repeated.");

			Common::Configuration::setDefaultFile(CONFIGURE_FILE);
			std::string soundPath(Common::Configuration::getDefaultValue(SOUNDS_PATH));
			soundPath = soundPath + fileName;

			FMOD::Sound* music;
			FMOD_RESULT result;
			if(loop)
				result = m_systemFMOD->createStream(soundPath.c_str(), FMOD_LOOP_NORMAL | FMOD_2D , 0, &music);
			else
				result = m_systemFMOD->createStream(soundPath.c_str(), FMOD_DEFAULT, 0, &music);

			if(ERRCHECK(result))
				return false;

			m_music[musicName] = music;

			return true;
		}

		bool CSound::add3dSound(const std::string& fileName, const std::string& soundName, bool loop)
		{
			assert(!m_3dsounds[soundName] && "Music name repeated.");

			Common::Configuration::setDefaultFile(CONFIGURE_FILE);
			std::string soundPath(Common::Configuration::getDefaultValue(SOUNDS_PATH));
			soundPath = soundPath + fileName;

			FMOD::Sound* sound;
			FMOD_RESULT result;

			if(loop)
				result = m_systemFMOD->createSound(soundPath.c_str(), FMOD_LOOP_NORMAL | FMOD_3D |  FMOD_3D_LINEARSQUAREROLLOFF, 0, &sound);
			else
				result = m_systemFMOD->createSound(soundPath.c_str(), FMOD_3D |  FMOD_3D_LINEARSQUAREROLLOFF, 0, &sound);

			if(ERRCHECK(result))
				return false;

			sound->set3DMinMaxDistance(0.0f, 2000.0f);

			m_3dsounds[soundName] = sound;

			return true;
		}

		bool CSound::addBank(const std::string& fileName, const std::string& bankName)
		{
			assert(!m_banks[bankName] && "Bank name repeated.");

			Common::Configuration::setDefaultFile(CONFIGURE_FILE);
			std::string soundPath(Common::Configuration::getDefaultValue(SOUNDS_PATH));
			soundPath = soundPath + fileName;

			FMOD::Studio::Bank* bank(nullptr);
			FMOD_RESULT result = m_studioFMOD->loadBankFile(soundPath.c_str(),FMOD_STUDIO_LOAD_BANK_NORMAL,&bank);
			if(ERRCHECK(result))
				return false;

			m_banks[bankName] = bank;

			return true;
		}

		bool CSound::addEvent(const std::string& eventString, const std::string& eventName)
		{
			assert(!m_events[eventName] && "Event name repeated.");

			FMOD::Studio::EventDescription* eventDescription = NULL;
			FMOD_RESULT result = m_studioFMOD->getEvent(eventString.c_str(), &eventDescription);
			if(ERRCHECK(result))
				return false;
			 FMOD::Studio::EventInstance* eventInstance = NULL;
			 result = eventDescription->createInstance(&eventInstance);
			 if(ERRCHECK(result))
				return false;

			 m_events[eventName] = eventInstance;

			return true;
		}

		bool CSound::addParameter(const std::string& eventName, const std::string& paramString, const std::string& paramName)
		{
			assert(!m_params[paramName] && "Parameter name repeated.");
			assert(m_events[eventName] && "Event name not found.");

			FMOD::Studio::ParameterInstance* parameter = NULL;
			FMOD_RESULT result = m_events[eventName]->getParameter(paramString.c_str(), &parameter);
			if(ERRCHECK(result))
				return false;

			m_params[paramName] = parameter;

			return true;
		}

		void CSound::releaseSound(const std::string& soundName)
		{
			m_sounds[soundName]->release();
			m_sounds.erase(soundName);
			if(m_channels.find(soundName) != m_channels.end())
				m_channels.erase(soundName);
		}

		void CSound::releaseMusic(const std::string& musicName)
		{
			m_music[musicName]->release();
			m_music.erase(musicName);
			if(m_channels.find(musicName) != m_channels.end())
				m_channels.erase(musicName);
		}

		void CSound::release3dSound(const std::string& soundName)
		{
			if(m_3dsounds.find(soundName) != m_3dsounds.end())
			{
				m_3dsounds[soundName]->release();
				if(m_channels.find(soundName) != m_channels.end())
					m_channels.erase(m_channels.find(soundName));
				if(m_3dpos.find(soundName) != m_3dpos.end())
					m_3dpos.erase(m_3dpos.find(soundName));
				m_3dsounds.erase(m_3dsounds.find(soundName));
			}
		}

		void CSound::free3dSound(const std::string& str)
		{
			m_3dsounds[str]->release();
			if(m_channels.find(str) != m_channels.end())
				m_channels.erase(str);
			if(m_3dpos.find(str) != m_3dpos.end())
				m_3dpos.erase(str);
		}

		void CSound::releaseEvent(const std::string& eventName)
		{
			assert(m_events[eventName] && "Event name not found.");

			FMOD_RESULT result = m_events[eventName]->release();
			ERRCHECK(result);

			m_events.erase(eventName);

		}

		void CSound::releaseParam(const std::string& paramName)
		{
			assert(m_params[paramName] && "Event name not found.");

			m_params.erase(paramName);
		}

		void CSound::releaseBank(const std::string& bankName)
		{
			assert(m_banks[bankName] && "Event name not found.");

			FMOD_RESULT result = m_banks[bankName]->unload();
			ERRCHECK(result);

			m_banks.erase(bankName);
		}

		void CSound::playSound(const std::string& soundName, float volume, bool release)
		{
			FMOD::Channel* canal = NULL;

			FMOD::Sound* sound = m_sounds[soundName];
			if(!sound){
				log_error(LOG_CSOUND,"Sound: %s not found.\n",soundName); 
				return;
			}

			FMOD_RESULT result = m_systemFMOD->playSound(sound, 0, false, &canal);
			ERRCHECK(result);

			result = canal->setVolume(volume);
			ERRCHECK(result);
			if(release){
				m_sounds.erase(soundName);
				return;
			}

			int id;
			result =canal->getIndex(&id);
			ERRCHECK(result);

			m_channels[soundName] = id;

		}

		void CSound::playDelayedSound(const std::string& soundName, int milis)
		{
			if(!m_sounds[soundName]){
				log_error(LOG_CSOUND,"Sound: %s not found.\n",soundName); 
				assert(!"Sound not found.");
			}

			m_delayed[soundName] = milis;
		}

		void CSound::playMusic(const std::string& musicName, float volume)
		{
			FMOD::Channel* canal = NULL;

			FMOD::Sound* music = m_music[musicName];
			if(!music){
				log_error(LOG_CSOUND,"Sound: %s not found.\n",musicName); 
				assert(!"Sound not found.");
			}

			FMOD_RESULT result = m_systemFMOD->playSound(music, 0, false, &canal);
			ERRCHECK(result);

			result = canal->setVolume(volume);
			ERRCHECK(result);

			int id;
			result =canal->getIndex(&id);
			ERRCHECK(result);

			m_channels[musicName] = id;
		}

		void CSound::play3dSound(const std::string& soundName, const Ogre::Matrix4& transform, float volume)
		{
			FMOD::Channel* canal = NULL;
			FMOD_VECTOR    initialPosition;

			FMOD::Sound* sound = m_3dsounds[soundName];
			if(!sound){
				log_error(LOG_CSOUND,"Sound: %s not found.\n",soundName); 
				assert(!"Sound not found.");
			}
		
			FMOD_RESULT result = m_systemFMOD->playSound(sound, 0, false, &canal);
			ERRCHECK(result);

			result = canal->setVolume(volume);
			ERRCHECK(result);

			std::vector<Ogre::Vector3> tmpVec;
			tmpVec.push_back(transform.getTrans());//Previous location
			tmpVec.push_back(transform.getTrans());//New Location
			m_3dpos[soundName]=tmpVec;

			initialPosition.x = transform.getTrans().x;
			initialPosition.y = transform.getTrans().y;
			initialPosition.z = transform.getTrans().z;

			result = canal->set3DAttributes(&initialPosition, NULL);
			ERRCHECK(result);


			int id;
			result =canal->getIndex(&id);
			ERRCHECK(result);

			m_channels[soundName] = id;

			canal->setPaused(false);
		}

		void CSound::startEvent(const std::string& eventName)
		{
			assert(m_events[eventName] && "Event not found." );

			FMOD_RESULT result = m_events[eventName]->start();
			ERRCHECK(result);
		}

		void CSound::pauseResumeMusic(const std::string& musicName)
		{
			FMOD::Channel* canal = NULL;

			FMOD::Sound* music = m_music[musicName];
			if(!music){
				log_error(LOG_CSOUND,"Music: %s not found.\n",musicName); 
				assert(!"Sound not found.");
			}

			if(m_channels.find(musicName) == m_channels.end())
			{
				log_error(LOG_CSOUND,"Channel: %s not found.\n",musicName); 
				assert(!"Channel not found.");
			}

			m_systemFMOD->getChannel(m_channels[musicName], &canal); 

			bool pause;

			FMOD_RESULT result = canal->getPaused(&pause);
			ERRCHECK(result);

			result = canal->setPaused(!pause);
			ERRCHECK(result);

		}

		void CSound::pauseResume3dSound(const std::string& soundName)
		{
			FMOD::Channel* canal = NULL;

			FMOD::Sound* sound = m_3dsounds[soundName];
			if(!sound){
				log_error(LOG_CSOUND,"Sound: %s not found.\n",soundName); 
				assert(!"Sound not found.");
			}

			if(m_channels.find(soundName) == m_channels.end())
			{
				log_error(LOG_CSOUND,"Channel: %s not found.\n",soundName); 
				assert(!"Channel not found.");
			}

			m_systemFMOD->getChannel(m_channels[soundName], &canal); 

			bool pause;

			FMOD_RESULT result = canal->getPaused(&pause);
			ERRCHECK(result);

			result = canal->setPaused(!pause);
			ERRCHECK(result);
		}

		void CSound::stopMusic(const std::string& musicName)
		{
			FMOD::Channel* canal = NULL;

			FMOD::Sound* music = m_music[musicName];
			if(!music){
				log_error(LOG_CSOUND,"Sound: %s not found.\n",musicName); 
				assert(!"Sound not found.");
			}

			if(m_channels.find(musicName) == m_channels.end())
			{
				log_error(LOG_CSOUND,"Channel: %s not found.\n",musicName); 
				assert(!"Channel not found.");
			}

			m_systemFMOD->getChannel(m_channels[musicName], &canal); 

			FMOD_RESULT result = canal->stop();
			ERRCHECK(result);

			m_channels.erase(musicName);
		}

		void CSound::stopEvent(const std::string& eventName)
		{
			assert(m_events[eventName] && "Event not found." );

			FMOD_RESULT result = m_events[eventName]->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
			ERRCHECK(result);

		}

		void CSound::setParamValue(const std::string& paramName, float value)
		{
			assert(m_params[paramName] && "Parameter not found." );

			FMOD_RESULT result = m_params[paramName]->setValue(value);
			ERRCHECK(result);
		}

		float CSound::getParamValue(const std::string& paramName)
		{
			assert(m_params[paramName] && "Parameter not found." );

			float value;

			FMOD_RESULT result = m_params[paramName]->getValue(&value);
			ERRCHECK(result);

			return value;
		}

		void CSound::set3DMinMaxDistance(const std::string& soundName, float minDistance, float maxDistance)
		{
			FMOD_RESULT    result;
			FMOD::Channel *channel;

			if(m_channels.find(soundName) == m_channels.end())
				return;

			result = m_systemFMOD->getChannel(m_channels[soundName], &channel);
			ERRCHECK(result);

			channel->set3DMinMaxDistance(minDistance, maxDistance);
		}

		void CSound::stop3dSound(const std::string& soundName)
		{
			FMOD::Channel* canal = NULL;

			FMOD::Sound* sound = m_3dsounds[soundName];
			if(!sound){
				log_error(LOG_CSOUND,"Sound: %s not found.\n",soundName); 
				assert(!"Sound not found.");
			}

			if(m_channels.find(soundName) == m_channels.end())
			{
				return;
			}

			m_systemFMOD->getChannel(m_channels[soundName], &canal); 

			FMOD_RESULT result = canal->setPaused(true);
			ERRCHECK(result);

			result = canal->stop();
			ERRCHECK(result);

			m_channels.erase(soundName);

			if(m_3dpos.find(soundName) != m_3dpos.end())
				m_3dpos.erase(soundName);
		}

		void CSound::stopAllSounds()
		{
			FMOD::Channel* canal = NULL;

			for(auto it = m_channels.begin();it != m_channels.end();++it){
				m_systemFMOD->getChannel(it->second, &canal); 
				FMOD_RESULT result = canal->stop();
				ERRCHECK(result);

				m_channels.erase(it->first);

				if(m_3dpos.find(it->first) != m_3dpos.end())
					m_3dpos.erase(it->first);
			}
		}

		void CSound::pauseResumeAll()
		{
			FMOD::Channel* canal = NULL;

			for(auto it = m_channels.begin();it != m_channels.end();++it){
				m_systemFMOD->getChannel(it->second, &canal); 
				bool pause;

				FMOD_RESULT result = canal->getPaused(&pause);
				ERRCHECK(result);

				result = canal->setPaused(!pause);
				ERRCHECK(result);

				m_channels.erase(it->first);

				if(m_3dpos.find(it->first) != m_3dpos.end())
					m_3dpos.erase(it->first);
			}
		}

		CSound::CSound():m_systemFMOD(nullptr), m_prevListenerPos(Ogre::Vector3(0.0,0.0,0.0))
		{
			// i'm supposing that this vectors are fixed
			//another option if this vectors are not known beforehand is to calculate them in the listenerUpdate
			m_listenerForward.x = 0.0;
			m_listenerForward.y = 0.0;
			m_listenerForward.z = 1.0;

			m_listenerUp.x = 0.0;
			m_listenerUp.y = 1.0;
			m_listenerUp.z = 0.0;
		}

		CSound::~CSound()
		{
			close();
		}

		bool CSound::open()
		{
			// Here the system is created
			FMOD_RESULT result = FMOD::Studio::System::create(&m_studioFMOD);
			if(ERRCHECK(result))
				return false;

			// Init channels and some flags
			result = m_studioFMOD->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
			if(ERRCHECK(result))
				return false;

			result = m_studioFMOD->getLowLevelSystem(&m_systemFMOD);
			if(ERRCHECK(result))
				return false;

			const float DOPPLER_SCALE(0.01f);
			const float DISTANCE_FACTOR(1.0f);
			const float ROLLOFF_SCALE(1.0f);

			m_systemFMOD->set3DSettings(DOPPLER_SCALE, DISTANCE_FACTOR, ROLLOFF_SCALE);

			return true;
		}

		void CSound::close()
		{
			FMOD_RESULT result;
		
			for(auto it=m_sounds.begin(); it!=m_sounds.end(); ++it){
				releaseSound(it->first);
			}

			for(auto it=m_music.begin(); it!=m_music.end(); ++it){
				releaseMusic(it->first);
			}

			for(auto it=m_3dsounds.begin(); it!=m_3dsounds.end(); ++it){
				free3dSound(it->first);
			}

			m_3dsounds.clear();

			result = m_systemFMOD->close();
		}

		bool CSound::ERRCHECK(int result)
		{
			FMOD_RESULT res = (FMOD_RESULT) result;
			if (res != FMOD_OK){
				printf("FMOD error %d - %s", res,
				FMOD_ErrorString(res));
				return true;
			}

			return false;
		}
	}
}
