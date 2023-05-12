#pragma once

#include <memory>
#include "Singleton.h"
#include "AudioService.h"

namespace Engine
{
	class ServiceLocator final
	{

	public:
		static void RegisterAudioService(std::unique_ptr<Engine::AudioService> pAudioService);
		static AudioService& GetAudioService() { return *m_pAudioService; };


	private:
		static std::unique_ptr<AudioService> m_pAudioService;

		// Destructor AND Constructor need to be defined in the CPP file for smart pointers to accept forward declarations
		ServiceLocator();
		~ServiceLocator();

	};
}