#pragma once
#include "AudioService.h"

namespace Engine
{
	class NullAudioService final: public AudioService
	{
	public:
		virtual void SetMasterVolume(float) override {};

		virtual void SetMusicVolume(float) override {};

		virtual void PlayEffect(const std::string&, float) override {};

		virtual void PlayMusic(const std::string&, float) override {};

		virtual void StopMusic() override {};

		virtual void PauseMusic() override {};

		virtual void ResumeMusic() override {};
	};
}

