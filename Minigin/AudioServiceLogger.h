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
		virtual void SetMasterVolume(float volume) override;
		virtual void SetMusicVolume(float volume) override;


		virtual void PlayEffect(const std::string& filePath, float volume) override;

		virtual void PlayMusic(const std::string& filePath, float volume) override;
		virtual void StopMusic() override;
		virtual void PauseMusic() override;
		virtual void ResumeMusic() override;

	private:
		std::unique_ptr<AudioService> m_pAudioService;

	};
}

