#pragma once

#include <memory>
#include "Singleton.h"

// Services included here to stop having to include them in every file
#include "AudioService.h"

namespace Engine
{
	class ServiceLocator final
	{

	public:
		// Is a full static class, so no need for a constructor or destructor
		ServiceLocator() = delete;
		~ServiceLocator() = delete;

		static void RegisterAudioService(std::unique_ptr<Engine::AudioService> pAudioService);
		static AudioService& GetAudioService() { return *m_pAudioService; };

	private:

		static std::unique_ptr<AudioService> m_pAudioService;
	};
}