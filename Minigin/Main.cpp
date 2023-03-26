#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"

// Components
#include "FPSComponent.h" 
#include "TextComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

// Custom Components
#include "ConstantRotator.h"

#include "GameCommands.h"

void MainScene()
{
	using namespace Engine;

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
	textComponent->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
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

	// Week 02
	auto rotationPoint = new GameObject();
	rotationPoint->GetTransform()->SetLocalPosition(250, 300);  // Parent
	scene->AddChild(rotationPoint);

	auto tank = new GameObject();
	tank->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletNPC.png"));
	tank->GetTransform()->SetLocalPosition(20, 20);
	tank->AddComponent<ConstantRotator>();
	rotationPoint->AddChild(tank);

	auto bullet = new GameObject();
	bullet->GetTransform()->SetLocalPosition(20, 20);
	bullet->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletPlayer.png"));
	bullet->AddComponent<ConstantRotator>()->SetRotationSpeed(180.0f);
	tank->AddChild(bullet);

	//
	//unsigned int controllerIdx = InputManager::GetInstance().AddController();
	//InputManager::GetInstance().AddAction<OnPressCommand>(controllerIdx, Engine::XController::ControllerButton::ButtonA, InputState::OnPress);
	//InputManager::GetInstance().AddAction<OnReleaseCommand>(controllerIdx, Engine::XController::ControllerButton::ButtonA, InputState::OnRelease);
	//InputManager::GetInstance().AddAction<PressedCommand>(controllerIdx, Engine::XController::ControllerButton::ButtonA, InputState::Pressed);
	//InputManager::GetInstance().AddAction<PressedCommand>(SDL_SCANCODE_A,InputState::Pressed);
	//InputManager::GetInstance().AddAxisMapping<MoveRight>(SDL_SCANCODE_D);
	//InputManager::GetInstance().AddAxisMapping<MoveRight>(controllerIdx, Engine::XController::ControllerAxis::LeftThumbX);
}

void ControllerInputTest()
{
	std::cout << "ControllerInputTest" << std::endl;
	std::cout << "Left Thumbstick to control first player" << std::endl;
	std::cout << "WASD to control second player" << std::endl;

	using namespace Engine;

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
	textComponent->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
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


	auto tank = new GameObject();
	tank->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletNPC.png"));
	tank->GetTransform()->SetLocalPosition(100, 100);
	scene->AddChild(tank);

	auto bullet = new GameObject();
	bullet->GetTransform()->SetLocalPosition(150, 100);
	bullet->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletPlayer.png"));
	scene->AddChild(bullet);


	unsigned int controllerIdx = InputManager::GetInstance().AddController();
	InputManager::GetInstance().AddAxisMapping<MoveRight>(SDL_SCANCODE_D, false, bullet);
	InputManager::GetInstance().AddAxisMapping<MoveRight>(SDL_SCANCODE_A, true, bullet);
	InputManager::GetInstance().AddAxisMapping<MoveForward>(SDL_SCANCODE_W, false, bullet);
	InputManager::GetInstance().AddAxisMapping<MoveForward>(SDL_SCANCODE_S, true, bullet);


	// Second player with joystick rather than dpad
	InputManager::GetInstance().AddAxisMapping<MoveForward>(controllerIdx, Engine::XController::ControllerAxis::LeftThumbY, tank);
	InputManager::GetInstance().AddAxisMapping<MoveRight>(controllerIdx, Engine::XController::ControllerAxis::LeftThumbX, tank);
}



void load()
{
	using namespace Engine;
	ControllerInputTest();
}

int main(int, char* []) 
{
	Engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}