#pragma once
#include "AudioService.h"

namespace Engine
{
	class SDLAudioService: public AudioService
	{
	public:
		virtual void Play(int audioId, float volume) const override;
		virtual void Stop(int audioId) const override;
		virtual void Pause(int audioId) const override;
		virtual void Resume(int audioId) const override;
		virtual void SetVolume(int audioId, float volume) const override;
	};
}

