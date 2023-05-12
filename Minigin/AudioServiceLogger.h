#pragma once

#include "AudioService.h"
namespace Engine
{
	class AudioServiceLogger final: public AudioService
	{
	public:
		AudioServiceLogger(std::unique_ptr<AudioService> pAudioService):
			m_pAudioService{ std::move(pAudioService)} {}


	private:

		// Inherited via AudioService
		virtual void Play(int audioId, float volume) const override;
		virtual void Stop(int audioId) const override;
		virtual void Pause(int audioId) const override;
		virtual void Resume(int audioId) const override;
		virtual void SetVolume(int audioId, float volume) const override;

		std::unique_ptr<AudioService> m_pAudioService;
	};
}

