#include "AudioServiceLogger.h"
#include <iostream>

void Engine::AudioServiceLogger::SetMasterVolume(float volume)
{
	std::cout << "AudioService::SetMasterVolume: " << volume << std::endl;
	m_pAudioService->SetMasterVolume(volume);
}

void Engine::AudioServiceLogger::SetMusicVolume(float volume)
{
	std::cout << "AudioService::SetMasterVolume: " << volume << std::endl;
	m_pAudioService->SetMasterVolume(volume);
}

void Engine::AudioServiceLogger::PlayEffect(const std::string& filePath, float volume)
{
	std::cout << "AudioService::PlayEffect: " << filePath << " Volume:" << volume << std::endl;
	m_pAudioService->PlayEffect(filePath, volume);

}
void Engine::AudioServiceLogger::PlayMusic(const std::string& filePath, float volume)
{
	std::cout << "AudioService::PlayMusic: " << filePath << " Volume:" << volume << std::endl;
	m_pAudioService->PlayMusic(filePath, volume);

}
void Engine::AudioServiceLogger::StopMusic()
{
	std::cout << "AudioService::StopMusic" << std::endl;
	m_pAudioService->StopMusic();

}
void Engine::AudioServiceLogger::PauseMusic()
{
	std::cout << "AudioService::PauseMusic" << std::endl;
	m_pAudioService->PauseMusic();
}
void Engine::AudioServiceLogger::ResumeMusic()
{
	std::cout << "AudioService::ResumeMusic" << std::endl;
	m_pAudioService->ResumeMusic();
}