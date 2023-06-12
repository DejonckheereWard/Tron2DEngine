#include "GameStates.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Font.h"
#include "TransformComponent.h"	
#include "TextComponent.h"
#include "RenderComponent.h"
#include "GameMode.h"
#include "InputManager.h"
#include "GameCommands.h"

#include <glm/vec2.hpp>


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
		pTextComp->SetText("SPACE/A TO START");
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
		auto pNextState = new GameOverState();
		pNextState->SetGameMode(m_pGameMode);
		m_pNextState = pNextState;
	}};

	InputManager::GetInstance().AddAction(SDL_SCANCODE_TAB, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, changeGameMode));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));


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

	//Scene* pScene{ Engine::SceneManager::GetInstance().CreateScene("LevelScene") };



}

State* States::PlayingState::Update()
{
	if (m_pGameMode->IsGameOver())
	{
		return new GameOverState();
	}


	return nullptr;
}

void States::PlayingState::OnExit()
{
	using namespace Engine;
	InputManager::GetInstance().ClearAllMappings();
	InputManager::GetInstance().ClearControllers();
	SceneManager::GetInstance().RemoveScene("LevelScene");
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
		pTextComp->SetText("SPACE/A TO CONTINUE");
	}

	std::function<void()> continueToPlay{[this]() {
		auto pNextState = new HighScoreMenuState();
		pNextState->SetGameMode(m_pGameMode);
		m_pNextState = pNextState;
	}};

	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));


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
	std::shared_ptr<Font> titleFont{  Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48) };
	std::shared_ptr<Font> defaultFont{  Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	{
		// Title
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.85f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(titleFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 1.f, 1.f });
		pTextComp->SetText("High Scores");
	}
	{
		// Continue
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.65f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(defaultFont);
		pTextComp->SetColor(glm::vec3{ 1.f, 1.f, 0.f });
		pTextComp->SetText("SPACE/A TO CONTINUE");
	}

	std::function<void()> continueToPlay{[this]() {
		auto pNextState = new MainMenuState();
		pNextState->SetGameMode(m_pGameMode);
		m_pNextState = pNextState;
	}};

	InputManager::GetInstance().AddAction(SDL_SCANCODE_SPACE, Engine::InputState::OnPress, std::make_unique<CallbackCommand>(nullptr, continueToPlay));


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


