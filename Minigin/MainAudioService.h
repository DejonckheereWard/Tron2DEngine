#pragma once
#include "AudioService.h"

namespace Engine
{
	class MainAudioService: public AudioService
	{
	public:
		MainAudioService();
		~MainAudioService();

		virtual void SetMasterVolume(float volume) override;

		virtual void SetMusicVolume(float volume) override;

		virtual void PlayEffect(const std::string& filePath, float volume) override;

		virtual void PlayMusic(const std::string& filePath, float volume) override;

		virtual void StopMusic() override;

		virtual void PauseMusic() override;

		virtual void ResumeMusic() override;

	private:
		std::unique_ptr<class MainAudioServiceImpl> m_pAudioImpl;

	};
}

