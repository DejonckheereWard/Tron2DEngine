// TronBattleTanks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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

// Components
#include "FPSComponent.h" 
#include "TextComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

// Custom Components
#include "HealthDisplay.h"
#include "LivesDisplay.h"
#include "HealthComponent.h"
#include "ScoreDisplay.h"
#include "ScoreComponent.h"
#include "NPCControlComponent.h"
#include "MoveComponent.h"
#include "WallRenderer.h"
#include "LevelComponent.h"

#include "GameCommands.h"
#include "TankGunComponent.h"
#include "TankTurretComponent.h"
#include "Main.h"


void PrintManual()
{
	std::cout << std::endl;
	std::cout << "Controls:" << std::endl;
	std::cout << "Player 1:" << std::endl;
	std::cout << "WASD - Movement" << std::endl;
	std::cout << "SPACE - Shoot" << std::endl;
	std::cout << "ARROWS - Aiming" << std::endl;
}

// Tron main scene
void MainScene()
{
	// TODO:
	// Load the menu for selection
	// Load the level from file (3 levels required)
	//	Load the HUD
	//  Load the players
	//  Load the enemies
	//  Load the pickups
	//  Load the level boundaries
	//  Load the level obstacles
	//  Reload the level when a player dies (if lives > 0)
	// Load the next level if the player reaches the end of the level, repeat some of the above steps


	// Requirements
	// 1 or 2 players
	// 1 -> Keyboard or controller
	// 2 -> Keyboard and controller (OR both on controllers) (can be 2PVE or PVP (1v1))
	// On game over, display game over score and high score list
	// High score list should be saved to file (and loaded from file)
	// High score list should be able to be displayed in the menu
	// Make levels be read from json, or from a binary file (make it easy to convert)
	// (OPTIONAL) Level creator option could be nice to make the levels easier to create
	// Player has 3 game lives, not health.
	// On completion of all levels, it should loop back to the first level (endless play)
	//   Make sure the number of levels is automatic based on the number of levels in the folder (adding a file adds it automatically to the game)


	// Player movement feel / controls
	// WASD goes in the direction you want, if you cant go in that direction (wall) you continue in the direction you were last going
	// This way, you turn the corner from the moment you can
	// Controller is the same, but with the left stick (make sure deadzone input is ingnored)
	// Right stick is used to shoot in the direction you want to shoot, for keyboard this is the arrow keys (up, down, left, right)
	// but also combined for diagonal shots (up + left = upleft shot)



	// Enemies 
	// Blue tanks: 3 shots to kill, insta kills player
	// Red Recognizer: 2x speed blue tank, doesnt shoot but tries to ram player

	using namespace Engine;

	// Create main scene
	Scene* pScene = Engine::SceneManager::GetInstance().CreateMainScene();

	// Set up services
	ServiceLocator::RegisterAudioService(std::make_unique<AudioServiceLogger>(std::make_unique<MainAudioService>()));

	//InputManager::GetInstance().AddAction(SDL_SCANCODE_UP, Engine::InputState::OnPress, std::make_unique<OnPressCommand>());

	PrintManual();

	//// Spawn in background
	//auto backGround = new GameObject();
	//std::shared_ptr<Texture2D> bgTexture{ ResourceManager::GetInstance().LoadTexture("background.tga") };
	//backGround->AddComponent<RenderComponent>()->SetTexture(bgTexture);
	//pScene->AddChild(backGround);

	// Create the level
	CreateLevel(pScene);

	// Spawn in player
	GameObject* pPlayer{ SpawnPlayer(pScene) };
	SpawnEnemy(pScene, pPlayer);
	{
		//std::shared_ptr<Texture2D> circuitBoardTexture{ ResourceManager::GetInstance().LoadTexture("Sprites/Level/CircuitBoard.png") };
		//GameObject* pWall{ new GameObject() };
		//pWall->AddComponent<WallRenderer>()->SetBackgroundTexture(circuitBoardTexture);
		//pWall->GetTransform()->SetLocalPosition(0, 0);

		//pScene->AddChild(pWall);

		//InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_KP_6, std::make_unique<DebugMoveCommand>(pWall), glm::vec2{ 1.0f, 0.0f });
		//InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_KP_4, std::make_unique<DebugMoveCommand>(pWall), glm::vec2{ -1.0f, 0.0f });
		//InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_KP_8, std::make_unique<DebugMoveCommand>(pWall), glm::vec2{ 0.0f, 1.0f });
		//InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_KP_5, std::make_unique<DebugMoveCommand>(pWall), glm::vec2{ 0.0f, -1.0f });
	}

}

Engine::GameObject* SpawnPlayer(Engine::Scene* pScene)
{
	using namespace Engine;
	GameObject* pPlayerTank{ new GameObject() };
	pPlayerTank->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/RedTank.png"));
	pPlayerTank->AddComponent<HealthComponent>()->SetHealth(1);
	pPlayerTank->AddComponent<ScoreComponent>();
	pPlayerTank->AddComponent<MoveComponent>();
	pPlayerTank->GetTransform()->SetLocalPosition(0, 0);
	pScene->AddChild(pPlayerTank);


	GameObject* pPlayerTankTurret{ new GameObject };
	{
		pPlayerTank->AddChild(pPlayerTankTurret);
		pPlayerTankTurret->AddComponent<TankTurretComponent>();
		pPlayerTankTurret->GetTransform()->SetLocalPosition(16.0f, 16.0f); // Set to center of tank
		auto* renderComponent = pPlayerTankTurret->AddComponent<RenderComponent>();
		renderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletPlayer.png"));
		renderComponent->SetTextureOffset({ 0.5f, 0.5f });

	}
	GameObject* pPlayerTankGun{ new GameObject() };
	{
		pPlayerTankTurret->AddChild(pPlayerTankGun);
		auto* renderComponent = pPlayerTankGun->AddComponent<RenderComponent>();
		renderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/RedTankGun.png"));
		renderComponent->SetTextureOffset({ 0.5f, 0.5f });
		pPlayerTankGun->AddComponent<TankGunComponent>();
		pPlayerTankGun->GetTransform()->SetLocalPosition(16.0f, 16.0f);
	}


	// Create controller
	unsigned int controllerIdx = InputManager::GetInstance().AddController();

	// MOVEMENT
	InputManager::GetInstance().AddAxisMapping(controllerIdx, Engine::XController::ControllerAxis::LeftThumbXY, std::make_unique<MoveCommand>(pPlayerTank));
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_D, std::make_unique<MoveCommand>(pPlayerTank), glm::vec2{ 1.0f, 0.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_A, std::make_unique<MoveCommand>(pPlayerTank), glm::vec2{ -1.0f, 0.0f });

	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_W, std::make_unique<MoveCommand>(pPlayerTank), glm::vec2{ 0.0f, 1.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_S, std::make_unique<MoveCommand>(pPlayerTank), glm::vec2{ 0.0f, -1.0f });

	// AIMING
	InputManager::GetInstance().AddAxisMapping(controllerIdx, Engine::XController::ControllerAxis::RightThumbXY, std::make_unique<AimTurretCommand>(pPlayerTankTurret));
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_RIGHT, std::make_unique<AimTurretCommand>(pPlayerTankTurret), glm::vec2{ 1.0f, 0.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_LEFT, std::make_unique<AimTurretCommand>(pPlayerTankTurret), glm::vec2{ -1.0f, 0.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_UP, std::make_unique<AimTurretCommand>(pPlayerTankTurret), glm::vec2{ 0.0f, 1.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_DOWN, std::make_unique<AimTurretCommand>(pPlayerTankTurret), glm::vec2{ 0.0f, -1.0f });

	// Shooting
	InputManager::GetInstance().AddAction(controllerIdx, Engine::XController::ControllerButton::RightShoulder, InputState::OnPress, std::make_unique<ShootCommand>(pPlayerTankGun));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, Engine::InputState::OnPress, std::make_unique<ShootCommand>(pPlayerTankGun));

	return pPlayerTank;
}

Engine::GameObject* SpawnEnemy(Engine::Scene* pScene, Engine::GameObject* pTarget)
{
	using namespace Engine;
	GameObject* pTank{ new GameObject() };
	pTank->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BlueTank.png"));
	pTank->AddComponent<HealthComponent>()->SetHealth(1);
	pTank->AddComponent<ScoreComponent>();
	pTank->AddComponent<MoveComponent>();
	NPCControlComponent* pNPC{ pTank->AddComponent<NPCControlComponent>() };
	pNPC->SetTarget(pTarget);
	pTank->GetTransform()->SetLocalPosition(500.0f, 500.0f);
	pScene->AddChild(pTank);


	GameObject* pTankTurret{ new GameObject };
	{
		pTank->AddChild(pTankTurret);
		TankTurretComponent* pTurretComp{ pTankTurret->AddComponent<TankTurretComponent>() };
		pTankTurret->GetTransform()->SetLocalPosition(16.0f, 16.0f); // Set to center of tank
		RenderComponent* pRenderComponent{ pTankTurret->AddComponent<RenderComponent>() };
		pRenderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletNPC.png"));
		pRenderComponent->SetTextureOffset({ 0.5f, 0.5f });

		pNPC->SetTurretComponent(pTurretComp);

	}
	GameObject* pTankGun{ new GameObject() };
	{
		pTankTurret->AddChild(pTankGun);
		RenderComponent* pRenderComponent{ pTankGun->AddComponent<RenderComponent>() };
		pRenderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BlueTankGun.png"));
		pRenderComponent->SetTextureOffset({ 0.5f, 0.5f });
		TankGunComponent* pGunComp{ pTankGun->AddComponent<TankGunComponent>() };
		pTankGun->GetTransform()->SetLocalPosition(16.0f, 16.0f);

		pNPC->SetGunComponent(pGunComp);
	}


	return pTank;
}

void CreateLevel(Engine::Scene* pScene)
{
	using namespace Engine;
	Renderer& pRenderer{ Renderer::GetInstance() };
	const int levelSize{ static_cast<int>(pRenderer.GetWindowSize().y) };  // Size in pixels


	std::shared_ptr<Texture2D> circuitBoardTexture{ ResourceManager::GetInstance().LoadTexture("Sprites/Level/CircuitBoard.png") };
	Engine::GameObject* pLevel{ pScene->AddChild(new Engine::GameObject()) };
	pLevel->AddComponent<WallRenderer>()->SetBackgroundTexture(circuitBoardTexture);
	LevelComponent* pLevelComp{ pLevel->AddComponent<LevelComponent>(levelSize) };
	pLevelComp->SetLevelFile("Level/Level_0.csv");

	// Center level
	const glm::vec2 windowSize{ pRenderer.GetWindowSize() };
	pLevel->GetTransform()->SetLocalPosition((windowSize.x - levelSize) / 2.0f, (windowSize.y - levelSize) / 2.0f);
}


void load()
{
	using namespace Engine;
	//ControllerInputTest();
	//TestScene();
	MainScene();
}

int main(int, char* [])
{
	Engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}