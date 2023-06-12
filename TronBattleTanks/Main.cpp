// TronBattleTanks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

// Engine / Main
#include "Main.h"
#include "Minigin.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "Renderer.h"

// Services
#include "ServiceLocator.h"
#include "AudioServiceLogger.h"
#include "MainAudioService.h"
#include "NullAudioService.h"

// State
#include "EnemyStates.h"
#include "GameStates.h"

// Other
#include "GameMode.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "StateComponent.h"


void PrintManual()
{
	std::cout << std::endl;
	std::cout << "Controls:" << std::endl;
	std::cout << "Player 1:" << std::endl;
	std::cout << "WASD - Movement" << std::endl;
	std::cout << "SPACE - Shoot" << std::endl;
	std::cout << "ARROWS - Aiming" << std::endl;
}


void InitGame()
{
	using namespace Engine;
	PrintManual();

	// Set up services
	ServiceLocator::RegisterAudioService(std::make_unique<AudioServiceLogger>(std::make_unique<MainAudioService>()));

	///*Scene* pScene = */Engine::SceneManager::GetInstance().CreateMainScene();
	Scene* pPermanentScene = Engine::SceneManager::GetInstance().CreateScene("PermanentScene");
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	// Add fps counter permanently
	{
		auto fpsFont = Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);
		GameObject* pFpsCounter{ pPermanentScene->AddChild(new GameObject()) };
		pFpsCounter->AddComponent<Engine::RenderComponent>();
		pFpsCounter->AddComponent<Engine::TextComponent>()->SetFont(fpsFont);
		pFpsCounter->AddComponent<Engine::FPSComponent>();
		pFpsCounter->GetTransform()->SetLocalPosition(10.0f, 10.0f);
	}

	// Add game mode
	GameObject* pGameModeObj{ pPermanentScene->AddChild(new GameObject()) };
	GameMode* pGameMode{ pGameModeObj->AddComponent<GameMode>() };

	pGameMode->AddLevelPath(resourceManager.GetResourcePath("Level/Level_0.csv"));
	pGameMode->AddLevelPath(resourceManager.GetResourcePath("Level/Level_1.csv"));
	pGameMode->AddLevelPath(resourceManager.GetResourcePath("Level/Level_2.csv"));
	pGameMode->SetCurrentLevel(0);

	// Add game state manager
	{
		GameObject* pGameStateManager{ pPermanentScene->AddChild(new GameObject()) };
		auto* mainMenuState{ new States::MainMenuState() };
		mainMenuState->SetGameMode(pGameMode);
		pGameStateManager->AddComponent<StateComponent>(mainMenuState);
	}
}

void load()
{
	InitGame();
}

int main(int, char* [])
{
	Engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}