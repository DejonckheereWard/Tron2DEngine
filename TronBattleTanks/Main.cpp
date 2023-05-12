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
#include "SDLAudioService.h"

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

void PrintManual()
{
	std::cout << std::endl;
	std::cout << "Controls:" << std::endl;
	std::cout << "Player 1:" << std::endl;
	std::cout << "WASD - Movement" << std::endl;
	std::cout << "Player 2:" << std::endl;
	std::cout << "Controller Joystick Left - Movement" << std::endl;

	std::cout << std::endl;
	std::cout << "TESTING CONTROLS" << std::endl;
	std::cout << "DOWN - LEFT ARROW: Decrease Health Player A/B" << std::endl;
	std::cout << "UP - RIGHT ARROW: Increase Points Player A/B" << std::endl;
	std::cout << std::endl;	
}

void TestScene()
{
	using namespace Engine;
	PrintManual();

	glm::vec2 windowSize{ Renderer::GetInstance().GetWindowSize() };



	// Add background
	Scene* scene = Engine::SceneManager::GetInstance().CreateScene("MainScene");
	auto go = new GameObject();
	std::shared_ptr<Texture2D> bgTexture{ ResourceManager::GetInstance().LoadTexture("background.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(bgTexture);
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
	tankA->GetTransform()->SetLocalPosition(20, 20);
	auto healthCompA = tankA->AddComponent<HealthComponent>()->GetSubject();
	auto scoreCompA = tankA->AddComponent<ScoreComponent>()->GetSubject();
	healthCompA->AddObserver(healthDisplayComponentA);
	healthCompA->AddObserver(livesDisplayComponentA);
	scoreCompA->AddObserver(scoreDisplayAComp);
	scene->AddChild(tankA);

	auto tankB = new GameObject();
	tankB->GetTransform()->SetLocalPosition(20, 20);
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
	Scene* scene = Engine::SceneManager::GetInstance().CreateScene("MainScene");

	// Set up services
	ServiceLocator::RegisterAudioService(std::make_unique<AudioServiceLogger>(std::make_unique<SDLAudioService>()));

	AudioService& audioService = ServiceLocator::GetAudioService();
	audioService.Play(0);

	// Spawn in player
	GameObject* playerTank = new GameObject();
	playerTank->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/GreenTank.png"));
	playerTank->AddComponent<HealthComponent>()->SetHealth(1);
	playerTank->AddComponent<ScoreComponent>();
	scene->AddChild(playerTank);




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