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
#include "GameStates.h"

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


void InitGame()
{
	using namespace Engine;

	/*Scene* pScene = */Engine::SceneManager::GetInstance().CreateMainScene();
	Scene* pPermanentScene = Engine::SceneManager::GetInstance().CreateScene("PermanentScene");

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

	// Add game state manager
	{
		GameObject* pGameStateManager{ pPermanentScene->AddChild(new GameObject()) };
		auto* mainMenuState{ new States::MainMenuState() };
		mainMenuState->SetGameMode(pGameMode);
		pGameStateManager->AddComponent<StateComponent>(mainMenuState);
	}
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
	PrintManual();

	// Create main scene
	Scene* pScene = Engine::SceneManager::GetInstance().CreateMainScene();
	Scene* pPermanentScene = Engine::SceneManager::GetInstance().CreateScene("PermanentScene");

	// Create the gamemode
	GameObject* pGameObject{ pPermanentScene->AddChild(new GameObject()) };
	pGameObject->SetTag("GameMode");
	GameMode* pGameMode{ pGameObject->AddComponent<GameMode>() };
	pGameMode->SetGameModeType(GameModeType::COOP);


	// Set up services
	//ServiceLocator::RegisterAudioService(std::make_unique<AudioServiceLogger>(std::make_unique<MainAudioService>()));
	ServiceLocator::RegisterAudioService(std::make_unique<AudioServiceLogger>(std::make_unique<NullAudioService>()));


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


}

Engine::GameObject* SpawnPlayerA(Engine::Scene* pScene, GameMode* pGameMode, const glm::vec2& pos, unsigned int controllerIdx)
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
	pPlayerTextObj->GetComponent<TextComponent>()->SetText("PLAYER A");

	GameObject* pLivesDisplayObj{ pHUD->AddChild(new GameObject()) };
	LivesDisplay* pLivesDisplay{ pLivesDisplayObj->AddComponent<LivesDisplay>() };
	pLivesDisplay->SetPrefix("LIVES: ");
	pLivesDisplayObj->AddComponent<RenderComponent>();
	pLivesDisplayObj->AddComponent<TextComponent>()->SetFont(defaultFont);
	pLivesDisplayObj->GetTransform()->SetLocalPosition(10.0f, windowSize.y - 50.0f);

	GameObject* pScoreDisplayObj{ pHUD->AddChild(new GameObject()) };
	ScoreDisplay* pScoreDisplay{ pScoreDisplayObj->AddComponent<ScoreDisplay>() };
	pScoreDisplay->SetPrefix("PTS: ");
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

	pPlayerTank->GetTransform()->SetLocalPosition(pos);
	pPlayerTank->SetTag("Player");
	CollisionComponent* pCollider{ pPlayerTank->AddComponent<CollisionComponent>() };
	pCollider->SetColliderSize({ 24.0f, 24.0f });
	pCollider->SetColliderOffset({ 4.0f, 4.0f });
	pCollider->SetLayer(CollisionLayer::Player);
	pCollider->SetCollisionMask(CollisionLayer::World);  // Collide with everything except player
	pCollider->SetCollisionCallback([pPlayerTank](GameObject* pOther) {
		if (pOther->GetTag() == "Bullet" || pOther->GetTag() == "EnemyRecognizer")
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




	// MOVEMENT
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_D, std::make_unique<MoveCommand>(pPlayerTank), glm::vec2{ 1.0f, 0.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_A, std::make_unique<MoveCommand>(pPlayerTank), glm::vec2{ -1.0f, 0.0f });

	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_W, std::make_unique<MoveCommand>(pPlayerTank), glm::vec2{ 0.0f, 1.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_S, std::make_unique<MoveCommand>(pPlayerTank), glm::vec2{ 0.0f, -1.0f });

	// AIMING
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_RIGHT, std::make_unique<AimTurretCommand>(pPlayerTankTurret), glm::vec2{ 1.0f, 0.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_LEFT, std::make_unique<AimTurretCommand>(pPlayerTankTurret), glm::vec2{ -1.0f, 0.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_UP, std::make_unique<AimTurretCommand>(pPlayerTankTurret), glm::vec2{ 0.0f, 1.0f });
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_DOWN, std::make_unique<AimTurretCommand>(pPlayerTankTurret), glm::vec2{ 0.0f, -1.0f });

	// Shooting
	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, Engine::InputState::Pressed, std::make_unique<ShootCommand>(pPlayerTankGun));

	if (controllerIdx != UINT_MAX)
	{
		// Controller
		InputManager::GetInstance().AddAxisMapping(controllerIdx, Engine::XController::ControllerAxis::LeftThumbXY, std::make_unique<MoveCommand>(pPlayerTank));
		InputManager::GetInstance().AddAxisMapping(controllerIdx, Engine::XController::ControllerAxis::RightThumbXY, std::make_unique<AimTurretCommand>(pPlayerTankTurret));
		InputManager::GetInstance().AddAction(controllerIdx, Engine::XController::ControllerButton::RightShoulder, InputState::Pressed, std::make_unique<ShootCommand>(pPlayerTankGun));
	}
	
	
	return pPlayerTank;
}

Engine::GameObject* SpawnPlayerB(Engine::Scene* pScene, GameMode* pGameMode, const glm::vec2& pos, unsigned int controllerIdx)
{
	using namespace Engine;

	const glm::vec2& windowSize{ Renderer::GetInstance().GetWindowSize() };
	std::shared_ptr<Font> defaultFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };

	// Create HUD for the player
	GameObject* pHUD{ pScene->AddChild(new GameObject()) };
	pHUD->GetTransform()->SetLocalPosition(0, 0);

	GameObject* pPlayerTextObj{ pHUD->AddChild(new GameObject()) };
	pPlayerTextObj->GetTransform()->SetLocalPosition(windowSize.x - 85.0f, windowSize.y - 25.0f);
	pPlayerTextObj->AddComponent<RenderComponent>();
	pPlayerTextObj->AddComponent<TextComponent>()->SetFont(defaultFont);
	pPlayerTextObj->GetComponent<TextComponent>()->SetText("PLAYER B");

	GameObject* pLivesDisplayObj{ pHUD->AddChild(new GameObject()) };
	LivesDisplay* pLivesDisplay{ pLivesDisplayObj->AddComponent<LivesDisplay>() };
	pLivesDisplay->SetPrefix("LIVES: ");
	pLivesDisplayObj->AddComponent<RenderComponent>();
	pLivesDisplayObj->AddComponent<TextComponent>()->SetFont(defaultFont);
	pLivesDisplayObj->GetTransform()->SetLocalPosition(windowSize.x - 85.0f, windowSize.y - 50.0f);

	GameObject* pScoreDisplayObj{ pHUD->AddChild(new GameObject()) };
	ScoreDisplay* pScoreDisplay{ pScoreDisplayObj->AddComponent<ScoreDisplay>() };
	pScoreDisplay->SetPrefix("PTS: ");
	pScoreDisplayObj->AddComponent<RenderComponent>();
	pScoreDisplayObj->AddComponent<TextComponent>()->SetFont(defaultFont);
	pScoreDisplayObj->GetTransform()->SetLocalPosition(windowSize.x - 85.0f, windowSize.y - 75.0f);

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

	pPlayerTank->GetTransform()->SetLocalPosition(pos);
	pPlayerTank->SetTag("Player");
	CollisionComponent* pCollider{ pPlayerTank->AddComponent<CollisionComponent>() };
	pCollider->SetColliderSize({ 24.0f, 24.0f });
	pCollider->SetColliderOffset({ 4.0f, 4.0f });
	pCollider->SetLayer(CollisionLayer::Player);
	pCollider->SetCollisionMask(CollisionLayer::World);  // Collide with everything except player
	pCollider->SetCollisionCallback([pPlayerTank](GameObject* pOther) {
		if (pOther->GetTag() == "Bullet" || pOther->GetTag() == "EnemyRecognizer")
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

	assert(controllerIdx != UINT_MAX && "PlayerB requires a controller");

	// MOVEMENT
	InputManager::GetInstance().AddAxisMapping(controllerIdx, Engine::XController::ControllerAxis::LeftThumbXY, std::make_unique<MoveCommand>(pPlayerTank));

	// AIMING
	InputManager::GetInstance().AddAxisMapping(controllerIdx, Engine::XController::ControllerAxis::RightThumbXY, std::make_unique<AimTurretCommand>(pPlayerTankTurret));

	// Shooting
	InputManager::GetInstance().AddAction(controllerIdx, Engine::XController::ControllerButton::RightShoulder, InputState::Pressed, std::make_unique<ShootCommand>(pPlayerTankGun));

	return pPlayerTank;
}

Engine::GameObject* SpawnEnemyTank(Engine::Scene* pScene, GameMode* pGameMode, const glm::vec2& pos)
{
	using namespace Engine;
	GameObject* pTank{ new GameObject() };
	HealthComponent* pHealthComp{ pTank->AddComponent<HealthComponent>() };
	pHealthComp->SetHealth(3);
	pHealthComp->SetExtraLives(0);
	pHealthComp->GetSubject()->AddObserver(pGameMode);

	GameObject* pPlayerA{ pGameMode->GetPlayerA() };
	if (pPlayerA)
	{
		pHealthComp->GetSubject()->AddObserver(pPlayerA->GetComponent<ScoreComponent>());
	}

	GameObject* pPlayerB{ pGameMode->GetPlayerB() };
	if (pPlayerB)
	{
		pHealthComp->GetSubject()->AddObserver(pPlayerB->GetComponent<ScoreComponent>());
	}

	pTank->AddComponent<StateComponent>(new States::RoamingState());
	pTank->SetTag("EnemyInterceptor");

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
	pTank->GetTransform()->SetLocalPosition(pos);
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

Engine::GameObject* SpawnEnemyRecognizer(Engine::Scene* pScene, GameMode* pGameMode, const glm::vec2& pos)
{
	using namespace Engine;
	GameObject* pTank{ new GameObject() };
	HealthComponent* pHealthComp{ pTank->AddComponent<HealthComponent>() };
	pHealthComp->SetHealth(3);
	pHealthComp->SetExtraLives(0);
	pHealthComp->GetSubject()->AddObserver(pGameMode);

	GameObject* pPlayerA{ pGameMode->GetPlayerA() };
	if (pPlayerA)
	{
		pHealthComp->GetSubject()->AddObserver(pPlayerA->GetComponent<ScoreComponent>());
	}

	GameObject* pPlayerB{ pGameMode->GetPlayerB() };
	if (pPlayerB)
	{
		pHealthComp->GetSubject()->AddObserver(pPlayerB->GetComponent<ScoreComponent>());
	}

	pTank->AddComponent<StateComponent>(new States::RoamingState());
	pTank->SetTag("EnemyRecognizer");

	MoveComponent* pMoveComp{ pTank->AddComponent<MoveComponent>() };
	pMoveComp->SetSpeed(100.0f);

	// Make texture subobject for easy rotation around the center;
	GameObject* pTankTextureObj{ pTank->AddChild(new GameObject()) };
	pTankTextureObj->GetTransform()->SetLocalPosition(16.0f, 16.0f);
	RenderComponent* pTankRenderComp{ pTankTextureObj->AddComponent<RenderComponent>() };
	pTankRenderComp->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/Recognizer.png"));
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


	pTank->AddComponent<NPCControlComponent>();
	pTank->GetTransform()->SetLocalPosition(pos);
	pScene->AddChild(pTank);

	pTank->AddComponent<NavmeshComponent>();

	//GameObject* pTankTurret{ new GameObject };
	//{
	//	pTank->AddChild(pTankTurret);
	//	TankTurretComponent* pTurretComp{ pTankTurret->AddComponent<TankTurretComponent>() };
	//	pTankTurret->GetTransform()->SetLocalPosition(16.0f, 16.0f); // Set to center of tank
	//	pTurretComp->SetRotationSpeed(1024.0f);  // Increased speed for faster aiming (since enemy is not human controlled)
	//	pNPC->SetTurretComponent(pTurretComp);

	//}
	//GameObject* pTankGun{ new GameObject() };
	//{
	//	pTankTurret->AddChild(pTankGun);
	//	RenderComponent* pRenderComponent{ pTankGun->AddComponent<RenderComponent>() };
	//	pRenderComponent->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BlueTankGun.png"));
	//	pRenderComponent->SetTextureOffset({ 0.5f, 0.5f });
	//	TankGunComponent* pGunComp{ pTankGun->AddComponent<TankGunComponent>() };
	//	pGunComp->SetBulletCollisionLayer(CollisionLayer::Enemy);
	//	pGunComp->SetBulletTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletNPC.png"));
	//	pTankGun->GetTransform()->SetLocalPosition(16.0f, 16.0f);

	//	pNPC->SetGunComponent(pGunComp);
	//}


	return pTank;
}

void CreateLevel(Engine::Scene* pScene, GameMode* pGameMode)
{
	using namespace Engine;
	//Renderer& pRenderer{ Renderer::GetInstance() };

	LevelSettings levelSettings{};
	levelSettings.FilePath = "Level/Level_0.csv";
	levelSettings.CellSize = 16;
	levelSettings.Position = { 100.0f, 16.0f };
	levelSettings.WallTexturePath = "Sprites/Level/CircuitBoard.png";

	NavmeshManager& navMeshManager{ NavmeshManager::GetInstance() };
	glm::ivec2 outGridSize{};
	std::unordered_map<std::string, std::vector<glm::vec2>> outObjectPositions{};
	LevelLoader::LoadLevel(levelSettings, pScene, outGridSize, outObjectPositions);  // Loads in all the walls

	// Create navmesh
	NavmeshSettings navMeshSettings{};
	navMeshSettings.CellSize = { levelSettings.CellSize, levelSettings.CellSize };
	navMeshSettings.Position = levelSettings.Position;
	navMeshSettings.GridSize = outGridSize;

	navMeshManager.GenerateNavMesh(navMeshSettings);


	// Spawn diamond in the middle of the map
	GameObject* pDiamond{ pScene->AddChild(new GameObject()) };
	{
		// Center of level
		const glm::vec2 center{ levelSettings.Position + glm::vec2{outGridSize.x* levelSettings.CellSize * 0.5f} };

		RenderComponent* pRenderComp{ pDiamond->AddComponent<RenderComponent>() };
		pRenderComp->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/Level/Diamond.png"));
		pRenderComp->SetTextureOffset({ 0.5f, 0.5f });
		pDiamond->GetTransform()->SetLocalPosition(center);

		CollisionComponent* pCollisionComp{ pDiamond->AddComponent<CollisionComponent>() };
		pCollisionComp->SetColliderSize({ 32.0f, 32.0f });
		pCollisionComp->SetColliderOffset({ -16.0f, -16.0f });
		pCollisionComp->SetLayer(CollisionLayer::TriggerLayer);
		pCollisionComp->SetCollisionMask(CollisionLayer::Player);
		pCollisionComp->SetCollisionCallback(
			[navMeshSettings](GameObject* pOther) 
		{
			if (pOther->GetTag() == "Player")
			{
				 //Teleport player to random location in the map
				Engine::TransformComponent* pTransform{ pOther->GetTransform() };
				pTransform->SetLocalPosition(NavmeshManager::GetInstance().GetRandomNode()->Position - navMeshSettings.CellSize / 2.0f );
			}
		});
	}


	// Load in the objects

	const std::vector<glm::vec2>& playerPositions{ outObjectPositions["Player"] };


	if (pGameMode->GetGameModeType() == GameModeType::SOLO)
	{
		// Only spawn one player, and map both keyboard and controller to it
		unsigned int controllerIdx = InputManager::GetInstance().AddController();
		GameObject* pPlayerA{ SpawnPlayerA(pScene, pGameMode, playerPositions[0] + levelSettings.Position, controllerIdx) };
		pGameMode->SetPlayerA(pPlayerA);
	}
	else
	{
		// Spawn two players, one for each controller
		GameObject* pPlayerA{ SpawnPlayerA(pScene, pGameMode, playerPositions[0] + levelSettings.Position) };
		pGameMode->SetPlayerA(pPlayerA);

		unsigned int controllerIdx = InputManager::GetInstance().AddController();
		GameObject* pPlayerB{ SpawnPlayerB(pScene, pGameMode, playerPositions[1], controllerIdx) };
		pGameMode->SetPlayerB(pPlayerB);
	}

	const std::vector<glm::vec2>& enemyTankPositions{ outObjectPositions["EnemyTank"] };
	for (const glm::vec2& pos : enemyTankPositions)
	{
		SpawnEnemyTank(pScene, pGameMode, pos + levelSettings.Position);
	}

	const std::vector<glm::vec2>& enemyRecognizerPositions{ outObjectPositions["EnemyRecognizer"] };
	for (const glm::vec2& pos : enemyRecognizerPositions)
	{
		SpawnEnemyRecognizer(pScene, pGameMode, pos + levelSettings.Position);
	}
}


void load()
{

	InitGame();

	//MainScene();
}

int main(int, char* [])
{
	Engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}