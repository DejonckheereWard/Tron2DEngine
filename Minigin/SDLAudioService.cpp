#include "SDLAudioService.h"
#include "ResourceManager.h"
#include <SDL_mixer.h>
#include <cassert>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <map>
#include <iostream>


class SDLAudioInitError {};
class SDLAudioLoadError {};

struct AudioEvent
{
	std::string filePath;
	float volume{ 1.f };
	bool isMusic{ false };
	int loops{ 0 };  // -1 = loop forever
	int channel{ -1 };  // -1 = first free channel
};


// IMPLEMENTATION

class Engine::SDLAudioServiceImpl final: AudioService
{
public:
	SDLAudioServiceImpl():
		m_AudioThreadRunning{ true },
		m_AudioThread{ &SDLAudioServiceImpl::AudioEventHandler, this },
		m_EventQueueMutex{},
		m_AudioMap{}
	{
	}

	~SDLAudioServiceImpl()
	{
		m_AudioThreadRunning = false;
		m_QueueCondition.notify_one();  // Wake up the thread so it can exit

		for(auto& audioMapItem : m_AudioMap)
		{
			Mix_FreeChunk(audioMapItem.second);
		}

		if(m_pMusic)
		{
			Mix_FreeMusic(m_pMusic);
		}
	}

	virtual void SetMasterVolume(float volume) override
	{
		Mix_MasterVolume(static_cast<int>(std::clamp(volume, 0.0f, 1.0f) * MIX_MAX_VOLUME));
	}

	virtual void SetMusicVolume(float volume) override
	{
		Mix_VolumeMusic(static_cast<int>(std::clamp(volume, 0.0f, 1.0f) * MIX_MAX_VOLUME));
	}	

	void PlayEffect(const std::string& filePath, float volume) override
	{

		AudioEvent audioEvent{};
		audioEvent.volume = volume;
		audioEvent.filePath = filePath;

		std::lock_guard<std::mutex> lock(m_EventQueueMutex);
		m_AudioEventQueue.push(audioEvent);
		m_QueueCondition.notify_one();
	}

	void PlayMusic(const std::string& filePath, float volume) override
	{
		AudioEvent audioEvent{};
		audioEvent.filePath = filePath;
		audioEvent.volume = volume;
		audioEvent.isMusic = true;
		audioEvent.loops = -1;

		std::lock_guard<std::mutex> lock(m_EventQueueMutex);
		m_AudioEventQueue.push(audioEvent);
		m_QueueCondition.notify_one();
	}

	virtual void StopMusic() override
	{
		Mix_HaltMusic();
	}

	virtual void PauseMusic() override
	{
		Mix_PauseMusic();
	}

	virtual void ResumeMusic() override
	{
		Mix_ResumeMusic();
	}

	void AudioEventHandler()
	{
		// Initialize SDL_mixer
		if(Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
			// Handle initialization error
			throw new SDLAudioInitError();
		}
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
			// Handle audio initialization error
			throw new SDLAudioInitError();
		}

		while(m_AudioThreadRunning)
		{
			// The condition variable requires a unique lock
			std::unique_lock<std::mutex> lock(m_EventQueueMutex);

			// Wait for events to be added
			m_QueueCondition.wait(lock, [this] { return !m_AudioEventQueue.empty() || !m_AudioThreadRunning; });

			if(!m_AudioThreadRunning)
				break;

			// Get the next event
			AudioEvent audioEvent{ m_AudioEventQueue.front() };
			m_AudioEventQueue.pop();

			// Handle the event
			// Check if the audio is already loaded
			if(audioEvent.isMusic)
			{
				// Handle music seperately (it has its own channel)
				const std::string& filePath{ Engine::ResourceManager::GetInstance().GetSoundPath(audioEvent.filePath) };

				if(m_pMusic)
				{
					// Free the previous music if it exists
					Mix_FreeMusic(m_pMusic);
				}

				m_pMusic = Mix_LoadMUS(filePath.c_str());

				// Check if the audio is loaded
				assert(m_pMusic);
				if(m_pMusic == nullptr) {
					// Handle error
					std::cout << "Incorrect music file path: " << audioEvent.filePath << "\n";
				}
				else
				{
					// Play the audio
					Mix_PlayMusic(m_pMusic, audioEvent.loops);
				}
			}
			else
			{
				auto audioMapping{ m_AudioMap.find(audioEvent.filePath) };
				Mix_Chunk* pChunk{};
				if(audioMapping != m_AudioMap.end())
				{
					pChunk = (*audioMapping).second;
				}
				else
				{
					// Load the audio if it isn't loaded yet
					const std::string& filePath{ Engine::ResourceManager::GetInstance().GetSoundPath(audioEvent.filePath) };
					pChunk = Mix_LoadWAV(filePath.c_str());
					m_AudioMap[audioEvent.filePath] = pChunk;
				}

				// Check if the audio is loaded
				assert(pChunk);
				if(pChunk == nullptr) {
					// Handle error
					std::cout << "Incorrect sound file path: " << audioEvent.filePath << "\n";
				}
				else
				{				
					// Play the audio
					Mix_PlayChannel(-1, pChunk, audioEvent.loops);
				}
			}
		}

		// Clean up
		Mix_CloseAudio();
		Mix_Quit();
	}




private:
	std::queue<AudioEvent> m_AudioEventQueue;

	bool m_AudioThreadRunning;
	std::condition_variable m_QueueCondition;
	std::mutex m_EventQueueMutex;
	std::jthread m_AudioThread;

	std::map<std::string, Mix_Chunk*> m_AudioMap;
	Mix_Music* m_pMusic{};  // Current music that is playing

};


// SDLAudioService

Engine::SDLAudioService::SDLAudioService():
	m_pImpl{ std::make_unique<SDLAudioServiceImpl>() }
{}

// Destructor in CPP file to prevent incomplete type error with the unique_ptr
Engine::SDLAudioService::~SDLAudioService() = default;

void Engine::SDLAudioService::SetMasterVolume(float volume)
{
	m_pImpl->SetMasterVolume(volume);
}

void Engine::SDLAudioService::SetMusicVolume(float volume)
{
	m_pImpl->SetMusicVolume(volume);
}

void Engine::SDLAudioService::PlayMusic(const std::string& filePath, float volume)
{
	m_pImpl->PlayMusic(filePath, volume);
}

void Engine::SDLAudioService::StopMusic()
{
	m_pImpl->StopMusic();
}

void Engine::SDLAudioService::PauseMusic()
{
	m_pImpl->PauseMusic();

}

void Engine::SDLAudioService::ResumeMusic()
{
	m_pImpl->ResumeMusic();
}

void Engine::SDLAudioService::PlayEffect(const std::string& filePath, float volume)
{
	m_pImpl->PlayEffect(filePath, volume);
}