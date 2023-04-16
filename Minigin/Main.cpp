#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "CSteamAchievements.h"
#include "EAchievements.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "Renderer.h"
#include <steam_api.h>

// Components
#include "FPSComponent.h" 
#include "TextComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

// Custom Components
#include "ConstantRotator.h"
#include "HealthDisplay.h"
#include "LivesDisplay.h"
#include "HealthComponent.h"
#include "ScoreDisplay.h"
#include "ScoreComponent.h"

#include "GameCommands.h"

void MainScene()
{
	using namespace Engine;


	glm::vec2 windowSize{ Renderer::GetInstance().GetWindowSize() };

	// Add background
	Scene* scene = Engine::SceneManager::GetInstance().CreateScene("MainScene");
	auto go = new GameObject();
	std::shared_ptr<Texture2D> bgTexture{ ResourceManager::GetInstance().LoadTexture("background.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(bgTexture);
	go->GetComponent<TransformComponent>()->SetLocalScale(2.0f, 2.0f);
	scene->AddChild(go);

	// Add title
	go = new GameObject();
	go->GetTransform()->SetLocalPosition(80, 20);
	go->AddComponent<Engine::RenderComponent>();
	auto textComponent = go->AddComponent<TextComponent>();
	textComponent->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20));
	textComponent->SetText("Programming 4 Assignment");
	scene->AddChild(go);

	// Add Logo
	go = new GameObject();
	std::shared_ptr<Texture2D> logoTexture{ ResourceManager::GetInstance().LoadTexture("logo.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(logoTexture);
	go->GetTransform()->SetLocalPosition(216.0f, 180.0f);
	scene->AddChild(go);

	// Add fps counter
	go = new GameObject();
	go->AddComponent<Engine::FPSComponent>();
	go->AddComponent<Engine::RenderComponent>();
	go->AddComponent<Engine::TextComponent>();
	auto fpsFont = Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	fpsFont->GetFont();
	go->GetComponent<Engine::TextComponent>()->SetFont(fpsFont);
	go->GetComponent<Engine::TransformComponent>()->SetLocalPosition(10.0f, 10.0f);
	scene->AddChild(go);

	// Health displays
	auto healthDisplayA = new GameObject();
	healthDisplayA->AddComponent<RenderComponent>();
	healthDisplayA->AddComponent<TextComponent>()->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	auto healthDisplayComponentA = healthDisplayA->AddComponent<HealthDisplay>();
	healthDisplayComponentA->SetPrefix("PlayerA ");
	healthDisplayA->GetTransform()->SetLocalPosition(20, windowSize.y - 50);
	scene->AddChild(healthDisplayA);

	auto healthDisplayB = new GameObject();
	healthDisplayB->AddComponent<RenderComponent>();
	healthDisplayB->AddComponent<RenderComponent>();
	healthDisplayB->AddComponent<TextComponent>()->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	auto healthDisplayComponentB = healthDisplayB->AddComponent<HealthDisplay>();
	healthDisplayComponentB->SetPrefix("PlayerB ");
	healthDisplayB->GetTransform()->SetLocalPosition(200, windowSize.y - 50);
	scene->AddChild(healthDisplayB);

	// Lives displays
	auto livesDisplayA = new GameObject();
	livesDisplayA->AddComponent<RenderComponent>();
	livesDisplayA->AddComponent<TextComponent>()->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	auto livesDisplayComponentA = livesDisplayA->AddComponent<LivesDisplay>();
	livesDisplayComponentA->SetPrefix("PlayerA ");
	livesDisplayA->GetTransform()->SetLocalPosition(20, windowSize.y - 70);
	scene->AddChild(livesDisplayA);

	auto livesDisplayB = new GameObject();
	livesDisplayB->AddComponent<RenderComponent>();
	livesDisplayB->AddComponent<RenderComponent>();
	livesDisplayB->AddComponent<TextComponent>()->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	auto livesDisplayComponentB = livesDisplayB->AddComponent<LivesDisplay>();
	livesDisplayComponentB->SetPrefix("PlayerB ");
	livesDisplayB->GetTransform()->SetLocalPosition(200, windowSize.y - 70);
	scene->AddChild(livesDisplayB);

	// Score displays
	auto scoreDisplayA = new GameObject();
	scoreDisplayA->AddComponent<RenderComponent>();
	scoreDisplayA->AddComponent<TextComponent>()->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	auto scoreDisplayAComp = scoreDisplayA->AddComponent<ScoreDisplay>();
	scoreDisplayAComp->SetPrefix("PlayerA ");
	scoreDisplayA->GetTransform()->SetLocalPosition(20, windowSize.y - 30);
	scene->AddChild(scoreDisplayA);

	auto scoreDisplayB = new GameObject();
	scoreDisplayB->AddComponent<RenderComponent>();
	scoreDisplayB->AddComponent<TextComponent>()->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	auto scoreDisplayBComp = scoreDisplayB->AddComponent<ScoreDisplay>();
	scoreDisplayBComp->SetPrefix("PlayerB ");
	scoreDisplayB->GetTransform()->SetLocalPosition(200, windowSize.y - 30);
	scene->AddChild(scoreDisplayB);



	// Players
	auto tankA = new GameObject();
	tankA->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletNPC.png"));
	tankA->GetTransform()->SetLocalPosition(200, 200);
	auto healthCompA = tankA->AddComponent<HealthComponent>()->GetSubject();
	auto scoreCompA = tankA->AddComponent<ScoreComponent>()->GetSubject();
	healthCompA->AddObserver(healthDisplayComponentA);
	healthCompA->AddObserver(livesDisplayComponentA);
	scoreCompA->AddObserver(scoreDisplayAComp);
	scoreCompA->AddObserver(&AchievementHandler::GetInstance());
	scene->AddChild(tankA);

	auto tankB = new GameObject();
	tankB->GetTransform()->SetLocalPosition(200, 200);
	tankB->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletPlayer.png"));
	auto healthCompB = tankB->AddComponent<HealthComponent>()->GetSubject();
	auto scoreCompB = tankB->AddComponent<ScoreComponent>()->GetSubject();
	healthCompB->AddObserver(healthDisplayComponentB);
	healthCompB->AddObserver(livesDisplayComponentB);
	scoreCompB->AddObserver(scoreDisplayBComp);
	scene->AddChild(tankB);

	// INPUT
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_D, std::make_unique<MoveCommand>(tankA, 2.0f, glm::vec2(1.0f, 0.0f)));
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_A, std::make_unique<MoveCommand>(tankA, 2.0f, glm::vec2(-1.0f, 0.0f)));
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_W, std::make_unique<MoveCommand>(tankA, 2.0f, glm::vec2(0.0f, -1.0f)));
	InputManager::GetInstance().AddAxisMapping(SDL_SCANCODE_S, std::make_unique<MoveCommand>(tankA, 2.0f, glm::vec2(0.0f, 1.0f)));


	unsigned int controllerIdx = InputManager::GetInstance().AddController();
	InputManager::GetInstance().AddAxisMapping(controllerIdx, Engine::XController::ControllerAxis::LeftThumbY, std::make_unique<MoveCommand>(tankB, 2.0f, glm::vec2(0.0f, -1.0f)));
	InputManager::GetInstance().AddAxisMapping(controllerIdx, Engine::XController::ControllerAxis::LeftThumbX, std::make_unique<MoveCommand>(tankB, 2.0f, glm::vec2(1.0f, 0.0f)));

	InputManager::GetInstance().AddAction(SDL_SCANCODE_DOWN, Engine::InputState::OnPress, std::make_unique<DamagePlayer>(tankA, 10));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_LEFT, Engine::InputState::OnPress, std::make_unique<DamagePlayer>(tankB, 10));

	InputManager::GetInstance().AddAction(SDL_SCANCODE_UP, Engine::InputState::OnPress, std::make_unique<AddScore>(tankA, 10));
	InputManager::GetInstance().AddAction(SDL_SCANCODE_RIGHT, Engine::InputState::OnPress, std::make_unique<AddScore>(tankB, 10));
}




void load()
{
	using namespace Engine;
	//ControllerInputTest();
	MainScene();
}

int main(int, char* [])
{
	if(!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
	{
		g_SteamAchievements = new Engine::CSteamAchievements(g_Achievements, 4);
		std::cout << "Successfully initialized steam." << std::endl;
	}

	Engine::Minigin engine("../Data/");
	engine.Run(load);

	SteamAPI_Shutdown();

	return 0;
}