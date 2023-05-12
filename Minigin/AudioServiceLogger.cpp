#include "AudioServiceLogger.h"
#include <iostream>

void Engine::AudioServiceLogger::Play(int audioId, float volume) const
{
	std::cout << "AudioService::Play: " << audioId << " " << volume << std::endl;
	m_pAudioService->Play(audioId, volume);

}

void Engine::AudioServiceLogger::Stop(int audioId) const
{
	std::cout << "AudioService::Stop: " << audioId << std::endl;
	m_pAudioService->Stop(audioId);
}

void Engine::AudioServiceLogger::Pause(int audioId) const
{
	std::cout << "AudioService::Pause: " << audioId << std::endl;
	m_pAudioService->Pause(audioId);
}

void Engine::AudioServiceLogger::Resume(int audioId) const
{
	std::cout << "AudioService::Resume: " << audioId << std::endl;
	m_pAudioService->Resume(audioId);
}

void Engine::AudioServiceLogger::SetVolume(int audioId, float volume) const
{
	std::cout << "AudioService::SetVolume: " << audioId << " " << volume << std::endl;
	m_pAudioService->SetVolume(audioId, volume);
}
