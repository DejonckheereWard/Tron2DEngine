#include "GameStates.h"
#include <fstream>
#include <sstream>

#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "CollisionManager.h"

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


void States::MainMenuState::OnEnter()
{
	using namespace Engine;

	// Load the main menu
	Scene* pScene{ Engine::SceneManager::GetInstance().CreateScene("MainMenu") };
	const glm::vec2& windowSize{ Engine::Renderer::GetInstance().GetWindowSize() };

	// Load a font
	std::shared_ptr<Font> titleFont{  Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48) };
	std::shared_ptr<Font> defaultFont{  Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };

	{
		// Title
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.85f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(titleFont);
		pTextComp->SetColor(glm::vec3{ 0.f, 1.f, 0.f });
		pTextComp->SetText("Main Menu");
	}
	{
		// Start
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.65f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(defaultFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 1.f, 0.f });
		pTextComp->SetText("ENTER/A TO START");
	}
	{
		// Change gamemode
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.45f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(defaultFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 1.f, 0.f });
		pTextComp->SetText("TAB/Y TO CHANGE GAMEMODE");
	}
	{
		// Current gamemode
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.35f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		m_pGameModeTextComp = pObject->AddComponent<TextComponent>();
		m_pGameModeTextComp->SetFont(defaultFont);
		m_pGameModeTextComp->SetColor(glm::vec3{ 0.f, 1.f, 1.f });
		m_pGameModeTextComp->SetText("SOLO");
	}

	// Add controller input
	GameMode* pGameMode{ m_pGameMode };

	std::function<void()> changeGameMode{[pGameMode]() {
		int idx = static_cast<int>(pGameMode->GetGameModeType());
		idx = (idx + 1) % (static_cast<int>(GameModeType::VERSUS) + 1);
		pGameMode->SetGameModeType(static_cast<GameModeType>(idx));			
	}};

	std::function<void()> continueToPlay{[this]() {
		auto pNextState = new PlayingState();
		pNextState->SetGameMode(m_pGameMode);
		m_pNextState = pNextState;
	}};

	InputManager::GetInstance().AddAction(SDL_SCANCODE_TAB, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, changeGameMode));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_RETURN, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));


	unsigned int controllerIdx{ InputManager::GetInstance().AddController() };
	InputManager::GetInstance().AddAction(controllerIdx, XController::ControllerButton::ButtonY, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, changeGameMode));
	InputManager::GetInstance().AddAction(controllerIdx, XController::ControllerButton::ButtonA, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));

	controllerIdx = InputManager::GetInstance().AddController();
	InputManager::GetInstance().AddAction(controllerIdx, XController::ControllerButton::ButtonY, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, changeGameMode));
	InputManager::GetInstance().AddAction(controllerIdx, XController::ControllerButton::ButtonA, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));

	pScene->Init();
}

State* States::MainMenuState::Update()
{
	using namespace Engine;

	if(m_pNextState)
		return m_pNextState;

	switch (m_pGameMode->GetGameModeType())
	{
	case GameModeType::SOLO:
		m_pGameModeTextComp->SetText("SOLO");
		break;
	case GameModeType::COOP:
		m_pGameModeTextComp->SetText("COOP");
		break;
	case GameModeType::VERSUS:
		m_pGameModeTextComp->SetText("VERSUS");
		break;
	default:
		break;
	}

	return nullptr;
}

void States::MainMenuState::OnExit()
{
	using namespace Engine;
	InputManager::GetInstance().ClearAllMappings();
	InputManager::GetInstance().ClearControllers();
	SceneManager::GetInstance().RemoveScene("MainMenu");
}

void States::PlayingState::OnEnter()
{
	using namespace Engine;

	if (m_pGameMode->IsGameOver())
	{
		m_pGameMode->ResetGameMode();
	}

	if (m_pGameMode->DoResetLevel())
	{
		m_pGameMode->ResetResetLevel();
	}

	SceneManager::GetInstance().CreateMainScene()->Init();
	Scene* pScene{ Engine::SceneManager::GetInstance().CreateScene("LevelScene") };
	m_pScene = pScene;

	std::function<void()> nextLevel{[this]() {
		auto pNextState = new PlayingState();
		m_pGameMode->NextLevel();
		pNextState->SetGameMode(m_pGameMode);
		m_pNextState = pNextState;
	}};

	InputManager::GetInstance().AddAction(SDL_SCANCODE_F1, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, nextLevel));

	InitLevel();

	pScene->Init();

}

State* States::PlayingState::Update()
{
	if (m_pGameMode->IsGameOver())
	{
		auto pPlayingState = new GameOverState();
		pPlayingState->SetGameMode(m_pGameMode);
		return pPlayingState;
	}
	else if(m_pGameMode->DoResetLevel())
	{
		auto pPlayingState = new PlayingState();
		pPlayingState->SetGameMode(m_pGameMode);
		return pPlayingState;
	}

	if (m_pNextState)
		return m_pNextState;

	return nullptr;
}

void States::PlayingState::OnExit()
{
	using namespace Engine;
	InputManager::GetInstance().ClearAllMappings();
	InputManager::GetInstance().ClearControllers();
	SceneManager::GetInstance().RemoveScene("LevelScene");
	Engine::CollisionManager::GetInstance().Cleanup();
}

void States::PlayingState::InitLevel()
{
	using namespace Engine;

	LevelSettings levelSettings{};
	levelSettings.FilePath = m_pGameMode->GetCurrentLevelPath();
	levelSettings.CellSize = 16;
	levelSettings.Position = { 100.0f, 16.0f };
	levelSettings.WallTexturePath = "Sprites/Level/CircuitBoard.png";

	NavmeshManager& navMeshManager{ NavmeshManager::GetInstance() };
	glm::ivec2 outGridSize{};
	std::unordered_map<std::string, std::vector<glm::vec2>> outObjectPositions{};
	LevelLoader::LoadLevel(levelSettings, m_pScene, outGridSize, outObjectPositions);  // Loads in all the walls

	// Create navmesh
	NavmeshSettings navMeshSettings{};
	navMeshSettings.CellSize = { levelSettings.CellSize, levelSettings.CellSize };
	navMeshSettings.Position = levelSettings.Position;
	navMeshSettings.GridSize = outGridSize;

	navMeshManager.GenerateNavMesh(navMeshSettings);


	// Spawn diamond in the middle of the map
	GameObject* pDiamond{ m_pScene->AddChild(new GameObject()) };
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
					pTransform->SetLocalPosition(NavmeshManager::GetInstance().GetRandomNode()->Position - navMeshSettings.CellSize / 2.0f);
				}
			});
	}


	// Load in the objects
	const std::vector<glm::vec2>& playerPositions{ outObjectPositions["Player"] };

	if (m_pGameMode->GetGameModeType() == GameModeType::SOLO)
	{
		// Only spawn one player, and map both keyboard and controller to it
		unsigned int controllerIdx = InputManager::GetInstance().AddController();
		GameObject* pPlayerA{ SpawnPlayerA(playerPositions[0] + levelSettings.Position, controllerIdx) };
		m_pGameMode->SetPlayerA(pPlayerA);
	}
	else
	{
		// Spawn two players, one for each controller
		GameObject* pPlayerA{ SpawnPlayerA(playerPositions[0] + levelSettings.Position) };
		m_pGameMode->SetPlayerA(pPlayerA);

		unsigned int controllerIdx = InputManager::GetInstance().AddController();
		GameObject* pPlayerB{ SpawnPlayerB(playerPositions[1], controllerIdx) };
		m_pGameMode->SetPlayerB(pPlayerB);
	}



	if (not (m_pGameMode->GetGameModeType() == GameModeType::VERSUS))
	{
		const std::vector<glm::vec2>& enemyTankPositions{ outObjectPositions["EnemyTank"] };
		for (const glm::vec2& pos : enemyTankPositions)
		{
			SpawnEnemyTank(pos + levelSettings.Position);
		}

		const std::vector<glm::vec2>& enemyRecognizerPositions{ outObjectPositions["EnemyRecognizer"] };
		for (const glm::vec2& pos : enemyRecognizerPositions)
		{
			SpawnEnemyRecognizer(pos + levelSettings.Position);
		}
	}

}

Engine::GameObject* States::PlayingState::SpawnPlayerA(const glm::vec2& pos, unsigned int controllerIdx)
{
	using namespace Engine;

	const glm::vec2& windowSize{ Renderer::GetInstance().GetWindowSize() };
	std::shared_ptr<Font> defaultFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };

	// Create HUD for the player
	GameObject* pHUD{ m_pScene->AddChild(new GameObject()) };
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
	if (m_pGameMode->GetPlayerA())
	{
		int extraLives{ m_pGameMode->GetPlayerALives() };
		pHealthComponent->SetExtraLives(extraLives);

	}
	pHealthComponent->GetSubject()->AddObserver(pLivesDisplay); // Add the Display as an observer to the health component
	pHealthComponent->GetSubject()->AddObserver(m_pGameMode);


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
	pTankTextureObj->AddComponent<RotateWithMovementComponent>()->SetMovementComponent(pMoveComp);

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

	m_pScene->AddChild(pPlayerTank);


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

Engine::GameObject* States::PlayingState::SpawnPlayerB(const glm::vec2& pos, unsigned int controllerIdx)
{
	using namespace Engine;

	const glm::vec2& windowSize{ Renderer::GetInstance().GetWindowSize() };
	std::shared_ptr<Font> defaultFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };

	// Create HUD for the player
	GameObject* pHUD{ m_pScene->AddChild(new GameObject()) };
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
	if (m_pGameMode->GetPlayerB())
	{
		int extraLives{ m_pGameMode->GetPlayerBLives() };
		pHealthComponent->SetExtraLives(extraLives);

	}
	pHealthComponent->GetSubject()->AddObserver(pLivesDisplay); // Add the Display as an observer to the health component
	pHealthComponent->GetSubject()->AddObserver(m_pGameMode);


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

	m_pScene->AddChild(pPlayerTank);


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

Engine::GameObject* States::PlayingState::SpawnEnemyTank(const glm::vec2& pos)
{
	using namespace Engine;
	GameObject* pTank{ new GameObject() };
	HealthComponent* pHealthComp{ pTank->AddComponent<HealthComponent>() };
	pHealthComp->SetHealth(3);
	pHealthComp->SetExtraLives(0);
	pHealthComp->GetSubject()->AddObserver(m_pGameMode);

	GameObject* pPlayerA{ m_pGameMode->GetPlayerA() };
	if (pPlayerA)
	{
		pHealthComp->GetSubject()->AddObserver(pPlayerA->GetComponent<ScoreComponent>());
	}

	GameObject* pPlayerB{ m_pGameMode->GetPlayerB() };
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
	m_pScene->AddChild(pTank);

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

Engine::GameObject* States::PlayingState::SpawnEnemyRecognizer(const glm::vec2& pos)
{
	using namespace Engine;
	GameObject* pTank{ new GameObject() };
	HealthComponent* pHealthComp{ pTank->AddComponent<HealthComponent>() };
	pHealthComp->SetHealth(3);
	pHealthComp->SetExtraLives(0);
	pHealthComp->GetSubject()->AddObserver(m_pGameMode);

	GameObject* pPlayerA{ m_pGameMode->GetPlayerA() };
	if (pPlayerA)
	{
		pHealthComp->GetSubject()->AddObserver(pPlayerA->GetComponent<ScoreComponent>());
	}

	GameObject* pPlayerB{ m_pGameMode->GetPlayerB() };
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
	m_pScene->AddChild(pTank);

	pTank->AddComponent<NavmeshComponent>();


	return pTank;
}



void States::GameOverState::OnEnter()
{
	using namespace Engine;
	Scene* pScene{ Engine::SceneManager::GetInstance().CreateScene("GameOverMenu") };
	const glm::vec2& windowSize{ Engine::Renderer::GetInstance().GetWindowSize() };

	// Load a font
	std::shared_ptr<Font> titleFont{  Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48) };
	std::shared_ptr<Font> defaultFont{  Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	{
		// Title
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.85f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(titleFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 0.f, 0.f });
		pTextComp->SetText("Game Over");
	}
	{
		// Continue
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.65f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(defaultFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 1.f, 0.f });
		pTextComp->SetText("ENTER/A TO CONTINUE");
	}

	std::function<void()> continueToPlay{[this]() {
		auto pNextState = new HighScoreMenuState();
		pNextState->SetGameMode(m_pGameMode);
		m_pNextState = pNextState;
	}};

	InputManager::GetInstance().AddAction(SDL_SCANCODE_RETURN, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));


	unsigned int controllerIdx{ InputManager::GetInstance().AddController() };
	InputManager::GetInstance().AddAction(controllerIdx, XController::ControllerButton::ButtonA, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));

	controllerIdx = InputManager::GetInstance().AddController();
	InputManager::GetInstance().AddAction(controllerIdx, XController::ControllerButton::ButtonA, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));


	pScene->Init();

}

State* States::GameOverState::Update()
{
	if (m_pNextState)
		return m_pNextState;
	return nullptr;
}

void States::GameOverState::OnExit()
{
	using namespace Engine;
	InputManager::GetInstance().ClearAllMappings();
	InputManager::GetInstance().ClearControllers();
	SceneManager::GetInstance().RemoveScene("GameOverMenu");
}


void States::HighScoreMenuState::OnEnter()
{
	using namespace Engine;

	// Load menu
	Scene* pScene{ Engine::SceneManager::GetInstance().CreateScene("HighScoreMenu") };
	const glm::vec2& windowSize{ Engine::Renderer::GetInstance().GetWindowSize() };

	// Load a font
	std::shared_ptr<Font> titleFont{  Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	std::shared_ptr<Font> defaultFont{  Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24) };
	{
		// Title
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.90f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(titleFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 1.f, 1.f });
		pTextComp->SetText("High Scores");
	}

	float heightMultiplier{ 0.80f };
	int cntLines{};
	for (const std::string& highScoreString : GetHighScores())
	{
		if(cntLines > 10)
			break;

		// Title
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * heightMultiplier);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(defaultFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 1.f, 1.f });
		pTextComp->SetText(highScoreString);

		heightMultiplier -= 0.06f;
		++cntLines;
	}

	{
		// Continue
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.15f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(defaultFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 1.f, 0.f });
		pTextComp->SetText("ENTER/A TO CONTINUE");
	}

	std::function<void()> continueToPlay{[this]() {
		auto pNextState = new MainMenuState();
		pNextState->SetGameMode(m_pGameMode);
		m_pNextState = pNextState;
	}};

	InputManager::GetInstance().AddAction(SDL_SCANCODE_RETURN, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));


	unsigned int controllerIdx{ InputManager::GetInstance().AddController() };
	InputManager::GetInstance().AddAction(controllerIdx, XController::ControllerButton::ButtonA, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));

	controllerIdx = InputManager::GetInstance().AddController();
	InputManager::GetInstance().AddAction(controllerIdx, XController::ControllerButton::ButtonA, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));


	pScene->Init();
}

State* States::HighScoreMenuState::Update()
{
	using namespace Engine;

	if(m_pNextState)
		return m_pNextState;

	return nullptr;
}

void States::HighScoreMenuState::OnExit()
{
	using namespace Engine;
	InputManager::GetInstance().ClearAllMappings();
	InputManager::GetInstance().ClearControllers();
	SceneManager::GetInstance().RemoveScene("HighScoreMenu");
}

std::vector<std::string> States::HighScoreMenuState::GetHighScores()
{
	std::vector<std::string> highScores{};
	std::ifstream file{ Engine::ResourceManager::GetInstance().GetResourcePath("highscores.csv") };

	if (file.is_open())
	{
		std::string inputLine{};
		while (std::getline(file, inputLine))
		{
			std::stringstream ss{ inputLine };

			std::string name{};
			std::string score{};

			std::getline(ss, name, ',');
			std::getline(ss, score, ',');

			std::string highscoreString{ name + ": " + score };

			highScores.push_back(highscoreString);
		}
	}



	return highScores;
}

