#pragma once
#include <string>
#include <thread>

namespace Engine
{
	class AudioService
	{
	public:
		virtual ~AudioService() = default;

		virtual void SetMasterVolume(float volume) = 0;

		virtual void SetMusicVolume(float volume) = 0;

		virtual void PlayEffect(const std::string& filePath, float volume = 1.0f) = 0;  // Multiple effects can be played at the same time

		virtual void PlayMusic(const std::string& filePath, float volume = 1.0f) = 0;  // Only one music file can be played at a time but it can be paused/resumed

		virtual void StopMusic() = 0;

		virtual void PauseMusic() = 0;

		virtual void ResumeMusic() = 0;

	protected:
		std::jthread m_AudioThread;

	};
}

