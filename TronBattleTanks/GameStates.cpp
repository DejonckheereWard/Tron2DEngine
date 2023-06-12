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
		pTextComp->SetText("TAB TO CHANGE GAMEMODE");
	}
	{
		// Current gamemode
		GameObject* pObject{ pScene->AddChild(new GameObject()) };
		pObject->GetTransform()->SetLocalPosition(windowSize.x / 2.0f, windowSize.y * 0.35f);
		pObject->AddComponent<RenderComponent>()->SetTextureOffset({ 0.5f, 0.5f });
		TextComponent* pTextComp{ pObject->AddComponent<TextComponent>() };
		pTextComp->SetFont(defaultFont);
		pTextComp->SetColor(glm::vec3{ 0.f, 1.f, 1.f });		
		pTextComp->SetText("SOLO");
	}

	pScene->Init();
}

State* States::MainMenuState::Update()
{
	using namespace Engine;
	return nullptr;
}

void States::MainMenuState::OnExit()
{
	using namespace Engine;
	SceneManager::GetInstance().RemoveScene("MainMenu");
}



void States::HighScoreMenuState::OnEnter()
{
	using namespace Engine;

	// Load the main menu
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
}

State* States::HighScoreMenuState::Update()
{
	return nullptr;
}

void States::HighScoreMenuState::OnExit()
{
	using namespace Engine;
	SceneManager::GetInstance().RemoveScene("HighScoreMenu");
}
