#include "MainAudioService.h"
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

// Exceptions
class SDLAudioInitError {};
class SDLAudioLoadError {};

// Structs
struct AudioEvent
{
	std::string filePath;
	float volume{ 1.f };
	bool isMusic{ false };
	int loops{ 0 };  // -1 = loop forever
	int channel{ -1 };  // -1 = first free channel
};


// IMPLEMENTATION

class Engine::MainAudioServiceImpl final: AudioService
{
public:
	MainAudioServiceImpl():
		m_AudioThreadRunning{ true },
		m_AudioThread{ &MainAudioServiceImpl::AudioEventHandler, this },
		m_EventQueueMutex{},
		m_AudioMap{},
		m_AudioMapMutex{},
		m_MusicMutex{}
	{
	}

	~MainAudioServiceImpl()
	{
		m_AudioThreadRunning = false;
		m_QueueCondition.notify_one();  // Wake up the thread so it can exit
		m_AudioThread.join();  // Wait for the thread to exit before cleaning up

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

		std::lock_guard lock{ m_EventQueueMutex };
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
			std::unique_lock lock{ m_EventQueueMutex };

			// Wait for events to be added
			m_QueueCondition.wait(lock, [this] { return !m_AudioEventQueue.empty() || !m_AudioThreadRunning; });

			if(!m_AudioThreadRunning)
				break;

			// Get the next event
			AudioEvent audioEvent{ m_AudioEventQueue.front() };
			m_AudioEventQueue.pop();
			lock.unlock();  // Unlock the mutex since we don't need it anymore for the event queue, and we dont want to block the other thread

			// Handle the event
			// Check if the audio is already loaded
			if(audioEvent.isMusic)
			{
				std::lock_guard musicLock{ m_MusicMutex };

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
				m_AudioMapMutex.lock();

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

				m_AudioMapMutex.unlock();

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

	std::mutex m_AudioMapMutex;
	std::map<std::string, Mix_Chunk*> m_AudioMap;


	std::mutex m_MusicMutex;
	Mix_Music* m_pMusic{};  // Current music that is playing

};


// SDLAudioService

Engine::MainAudioService::MainAudioService():
	m_pAudioImpl{ std::make_unique<MainAudioServiceImpl>() }
{}

// Destructor in CPP file to prevent incomplete type error with the unique_ptr
Engine::MainAudioService::~MainAudioService() = default;

void Engine::MainAudioService::SetMasterVolume(float volume)
{
	m_pAudioImpl->SetMasterVolume(volume);
}

void Engine::MainAudioService::SetMusicVolume(float volume)
{
	m_pAudioImpl->SetMusicVolume(volume);
}

void Engine::MainAudioService::PlayMusic(const std::string& filePath, float volume)
{
	m_pAudioImpl->PlayMusic(filePath, volume);
}

void Engine::MainAudioService::StopMusic()
{
	m_pAudioImpl->StopMusic();
}

void Engine::MainAudioService::PauseMusic()
{
	m_pAudioImpl->PauseMusic();

}

void Engine::MainAudioService::ResumeMusic()
{
	m_pAudioImpl->ResumeMusic();
}

void Engine::MainAudioService::PlayEffect(const std::string& filePath, float volume)
{
	m_pAudioImpl->PlayEffect(filePath, volume);
}