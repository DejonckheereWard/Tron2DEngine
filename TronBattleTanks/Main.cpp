// TronBattleTanks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

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

// Components
#include "FPSComponent.h" 
#include "TextComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"

// Custom Components
#include "HealthDisplay.h"
#include "LivesDisplay.h"
#include "HealthComponent.h"
#include "ScoreDisplay.h"
#include "ScoreComponent.h"
#include "NPCControlComponent.h"
#include "MoveComponent.h"
#include "WallRenderer.h"
#include "TankGunComponent.h"
#include "TankTurretComponent.h"
#include "NavmeshComponent.h"
#include "StateComponent.h"
#include "RotateWithMovementComponent.h"

// State
#include "EnemyStates.h"

// Other
#include "GameCommands.h"
#include "LevelLoader.h"
#include "GameMode.h"
#include "NavmeshManager.h"	


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
	Scene* pPermanentScene = Engine::SceneManager::GetInstance().CreateScene("PermanentScene");

	// Create the gamemode
	GameObject* pGameObject{ pPermanentScene->AddChild(new GameObject()) };
	pGameObject->SetTag("GameMode");
	GameMode* pGameMode{ pGameObject->AddComponent<GameMode>() };





	// Set up services
	//ServiceLocator::RegisterAudioService(std::make_unique<AudioServiceLogger>(std::make_unique<MainAudioService>()));
	ServiceLocator::RegisterAudioService(std::make_unique<AudioServiceLogger>(std::make_unique<NullAudioService>()));

	//InputManager::GetInstance().AddAction(SDL_SCANCODE_UP, Engine::InputState::OnPress, std::make_unique<OnPressCommand>());

	PrintManual();

	//// Spawn in background
	//auto backGround = new GameObject();
	//std::shared_ptr<Texture2D> bgTexture{ ResourceManager::GetInstance().LoadTexture("background.tga") };
	//backGround->AddComponent<RenderComponent>()->SetTexture(bgTexture);
	//pScene->AddChild(backGround);

	// Create the level
	CreateLevel(pScene, pGameMode);

	// Add FPS counter
	{
		// Add fps counter
		auto fpsFont = Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);
		GameObject* pFpsCounter{ pScene->AddChild(new GameObject()) };
		pFpsCounter->AddComponent<Engine::FPSComponent>();
		pFpsCounter->AddComponent<Engine::RenderComponent>();
		pFpsCounter->AddComponent<Engine::TextComponent>()->SetFont(fpsFont);
		pFpsCounter->GetTransform()->SetLocalPosition(10.0f, 10.0f);
	}

	// Spawn in player
	GameObject* pPlayer{ SpawnPlayer(pScene, pGameMode) };
	SpawnEnemy(pScene, pPlayer, pGameMode);
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

Engine::GameObject* SpawnPlayer(Engine::Scene* pScene, GameMode* pGameMode)
{
	using namespace Engine;

	const glm::vec2& windowSize{ Renderer::GetInstance().GetWindowSize() };
	std::shared_ptr<Font> defaultFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };

	// Create HUD for the player
	GameObject* pHUD{ pScene->AddChild(new GameObject()) };
	pHUD->GetTransform()->SetLocalPosition(0, 0);

	GameObject* pPlayerTextObj{ pHUD->AddChild(new GameObject()) };
	pPlayerTextObj->GetTransform()->SetLocalPosition(10.0f, windowSize.y - 25.0f);
	pPlayerTextObj->AddComponent<RenderComponent>();
	pPlayerTextObj->AddComponent<TextComponent>()->SetFont(defaultFont);
	pPlayerTextObj->GetComponent<TextComponent>()->SetText("PLAYER 1");

	GameObject* pLivesDisplayObj{ pHUD->AddChild(new GameObject()) };
	LivesDisplay* pLivesDisplay{ pLivesDisplayObj->AddComponent<LivesDisplay>() };
	pLivesDisplay->SetPrefix("LIVES: ");
	pLivesDisplayObj->AddComponent<RenderComponent>();
	pLivesDisplayObj->AddComponent<TextComponent>()->SetFont(defaultFont);
	pLivesDisplayObj->GetTransform()->SetLocalPosition(10.0f, windowSize.y - 50.0f);

	GameObject* pScoreDisplayObj{ pHUD->AddChild(new GameObject()) };
	ScoreDisplay* pScoreDisplay{ pScoreDisplayObj->AddComponent<ScoreDisplay>() };
	pScoreDisplay->SetPrefix("SCORE: ");
	pScoreDisplayObj->AddComponent<RenderComponent>();
	pScoreDisplayObj->AddComponent<TextComponent>()->SetFont(defaultFont);
	pScoreDisplayObj->GetTransform()->SetLocalPosition(10.0f, windowSize.y - 75.0f);

	GameObject* pPlayerTank{ new GameObject() };

	HealthComponent* pHealthComponent{ pPlayerTank->AddComponent<HealthComponent>() };

	pHealthComponent->SetHealth(1);
	pHealthComponent->SetMaxHealth(1);
	pHealthComponent->SetExtraLives(3);
	pHealthComponent->GetSubject()->AddObserver(pLivesDisplay); // Add the Display as an observer to the health component
	pHealthComponent->GetSubject()->AddObserver(pGameMode);

	ScoreComponent* pScoreComponent{ pPlayerTank->AddComponent<ScoreComponent>() };
	pScoreComponent->SetScore(0);
	pScoreComponent->GetSubject()->AddObserver(pScoreDisplay);


	MoveComponent* pMoveComp{ pPlayerTank->AddComponent<MoveComponent>() };
	pMoveComp->SetSpeed(50.0f);

	// Make texture subobject for easy rotation around the center;
	GameObject* pTankTextureObj{ pPlayerTank->AddChild(new GameObject()) };
	pTankTextureObj->GetTransform()->SetLocalPosition(16.0f, 16.0f);
	RenderComponent* pTankRenderComp{ pTankTextureObj->AddComponent<RenderComponent>() };
	pTankRenderComp->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/RedTank.png"));
	pTankRenderComp->SetTextureOffset({ 0.5f, 0.5f });
	pTankTextureObj->AddComponent< RotateWithMovementComponent>()->SetMovementComponent(pMoveComp);

	pPlayerTank->GetTransform()->SetLocalPosition(110, 100);
	pPlayerTank->SetTag("Player");
	CollisionComponent* pCollider{ pPlayerTank->AddComponent<CollisionComponent>() };
	pCollider->SetColliderSize({ 24.0f, 24.0f });
	pCollider->SetColliderOffset({ 4.0f, 4.0f });
	pCollider->SetLayer(CollisionLayer::Player);
	pCollider->SetCollisionMask(CollisionLayer::World);  // Collide with everything except player
	pCollider->SetCollisionCallback([pPlayerTank](GameObject* pOther) {
		if (pOther->GetTag() == "Bullet")
		{
			pPlayerTank->GetComponent<HealthComponent>()->TakeDamage(1);
		}
	});
	
	pScene->AddChild(pPlayerTank);


	GameObject* pPlayerTankTurret{ new GameObject };
	{
		pPlayerTank->AddChild(pPlayerTankTurret);
		pPlayerTankTurret->AddComponent<TankTurretComponent>();
		pPlayerTankTurret->GetTransform()->SetLocalPosition(16.0f, 16.0f); // Set to center of tank

	}
	GameObject* pPlayerTankGun{ new GameObject() };
	{
		pPlayerTankTurret->AddChild(pPlayerTankGun);
		auto* renderComponent = pPlayerTankGun->AddComponent<RenderComponent>();
		renderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/RedTankGun.png"));
		renderComponent->SetTextureOffset({ 0.5f, 0.5f });
		pPlayerTankGun->GetTransform()->SetLocalPosition(16.0f, 16.0f);
		TankGunComponent* pTankGunComp{ pPlayerTankGun->AddComponent<TankGunComponent>() };
		pTankGunComp->SetBulletCollisionLayer(CollisionLayer::Player);
		pTankGunComp->SetBulletTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletPlayer.png"));
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
	InputManager::GetInstance().AddAction(controllerIdx, Engine::XController::ControllerButton::RightShoulder, InputState::Pressed, std::make_unique<ShootCommand>(pPlayerTankGun));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, Engine::InputState::Pressed, std::make_unique<ShootCommand>(pPlayerTankGun));

	return pPlayerTank;
}

Engine::GameObject* SpawnEnemy(Engine::Scene* pScene, Engine::GameObject* pTarget, GameMode* pGameMode)
{
	using namespace Engine;
	GameObject* pTank{ new GameObject() };
	pTank->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BlueTank.png"));
	HealthComponent* pHealthComp{ pTank->AddComponent<HealthComponent>() };
	pHealthComp->SetHealth(1);
	pHealthComp->SetExtraLives(0);
	pHealthComp->GetSubject()->AddObserver(pGameMode);
	pTank->AddComponent<ScoreComponent>();
	pTank->AddComponent<StateComponent>(new States::RoamingState());
	pTank->SetTag("EnemyTank");
	
	MoveComponent* pMoveComp{ pTank->AddComponent<MoveComponent>() };
	pMoveComp->SetSpeed(50.0f);

	// Make texture subobject for easy rotation around the center;
	GameObject* pTankTextureObj{ pTank->AddChild(new GameObject()) };
	pTankTextureObj->GetTransform()->SetLocalPosition(16.0f, 16.0f);
	RenderComponent* pTankRenderComp{ pTankTextureObj->AddComponent<RenderComponent>() };
	pTankRenderComp->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BlueTank.png"));
	pTankRenderComp->SetTextureOffset({ 0.5f, 0.5f });
	pTankTextureObj->AddComponent< RotateWithMovementComponent>()->SetMovementComponent(pMoveComp);


	CollisionComponent* pCollider{ pTank->AddComponent<CollisionComponent>() };
	pCollider->SetColliderSize({ 24.0f, 24.0f });
	pCollider->SetColliderOffset({ 4.0f, 4.0f });
	pCollider->SetLayer(CollisionLayer::Enemy);
	pCollider->SetCollisionMask(std::numeric_limits<uint8_t>::max() & ~CollisionLayer::Enemy);  // Collide with everything except enemy
	pCollider->SetCollisionCallback([pTank](GameObject* pOther) {
		if (pOther->GetTag() == "Bullet")
		{
			pTank->GetComponent<HealthComponent>()->TakeDamage(1);
		}
	});


	NPCControlComponent* pNPC{ pTank->AddComponent<NPCControlComponent>() };
	pNPC->SetTarget(pTarget);
	pTank->GetTransform()->SetLocalPosition(200.0f, 195.0f);
	pScene->AddChild(pTank);

	pTank->AddComponent<NavmeshComponent>();

	GameObject* pTankTurret{ new GameObject };
	{
		pTank->AddChild(pTankTurret);
		TankTurretComponent* pTurretComp{ pTankTurret->AddComponent<TankTurretComponent>() };
		pTankTurret->GetTransform()->SetLocalPosition(16.0f, 16.0f); // Set to center of tank
		pTurretComp->SetRotationSpeed(1024.0f);  // Increased speed for faster aiming (since enemy is not human controlled)
		pNPC->SetTurretComponent(pTurretComp);

	}
	GameObject* pTankGun{ new GameObject() };
	{
		pTankTurret->AddChild(pTankGun);
		RenderComponent* pRenderComponent{ pTankGun->AddComponent<RenderComponent>() };
		pRenderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BlueTankGun.png"));
		pRenderComponent->SetTextureOffset({ 0.5f, 0.5f });
		TankGunComponent* pGunComp{ pTankGun->AddComponent<TankGunComponent>() };
		pGunComp->SetBulletCollisionLayer(CollisionLayer::Enemy);
		pGunComp->SetBulletTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletNPC.png"));
		pTankGun->GetTransform()->SetLocalPosition(16.0f, 16.0f);

		pNPC->SetGunComponent(pGunComp);
	}


	return pTank;
}

void CreateLevel(Engine::Scene* pScene, GameMode* /*pGameMode*/)
{
	using namespace Engine;
	//Renderer& pRenderer{ Renderer::GetInstance() };

	LevelSettings levelSettings{};
	levelSettings.FilePath = "Level/Level_1.csv";
	levelSettings.CellSize = 16;
	levelSettings.Position = { 100.0f, 16.0f };
	levelSettings.WallTexturePath = "Sprites/Level/CircuitBoard.png";

	NavmeshManager& navMeshManager{ NavmeshManager::GetInstance() };
	glm::ivec2 outGridSize{};
	LevelLoader::LoadLevel(levelSettings, pScene, outGridSize);  // Loads in all the walls

	// Create navmesh
	NavmeshSettings navMeshSettings{};
	navMeshSettings.CellSize = { levelSettings.CellSize, levelSettings.CellSize };
	navMeshSettings.Position = levelSettings.Position;
	navMeshSettings.GridSize = outGridSize;

	navMeshManager.GenerateNavMesh(navMeshSettings);
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