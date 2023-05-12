#pragma once
#include <string>
#include <thread>

namespace Engine
{
	class AudioService
	{
	public:
		virtual ~AudioService() = default;

		virtual void Play(int audioId, float volume = 1.f) const = 0;

		virtual void Stop(int audioId) const = 0;

		virtual void Pause(int audioId) const = 0;

		virtual void Resume(int audioId) const = 0;

		virtual void SetVolume(int audioId, float volume) const = 0;


	protected:
		std::jthread m_AudioThread;

	};
}

