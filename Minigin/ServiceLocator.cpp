#include "ServiceLocator.h"
#include "NullAudioService.h"

// Initialize static variables
std::unique_ptr<Engine::AudioService> Engine::ServiceLocator::m_pAudioService{};

// Register services
void Engine::ServiceLocator::RegisterAudioService(std::unique_ptr<Engine::AudioService> pAudioService = std::make_unique<NullAudioService>())
{
	m_pAudioService = std::move(pAudioService);
}