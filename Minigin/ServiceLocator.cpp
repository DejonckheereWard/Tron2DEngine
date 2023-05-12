#include "ServiceLocator.h"

#include "NullAudioService.h"

std::unique_ptr<Engine::AudioService> Engine::ServiceLocator::m_pAudioService{};

void Engine::ServiceLocator::RegisterAudioService(std::unique_ptr<Engine::AudioService> pAudioService = std::make_unique<NullAudioService>())
{
	m_pAudioService = std::move(pAudioService);
}

Engine::ServiceLocator::ServiceLocator()
{
}


Engine::ServiceLocator::~ServiceLocator()
{
}
