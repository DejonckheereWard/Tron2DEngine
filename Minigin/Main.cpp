#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"

void load()
{
	using namespace Engine;
	auto& scene = Engine::SceneManager::GetInstance().CreateScene("Demo");

	// Week 01

	// Add background
	auto go = std::make_shared<GameObject>();
	std::shared_ptr<Texture2D> bgTexture{ ResourceManager::GetInstance().LoadTexture("background.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(bgTexture);
	scene.Add(go);

	// Add title
	go = std::make_shared<Engine::GameObject>();
	go->GetComponent<TransformComponent>()->SetPosition(80, 20);
	go->AddComponent<Engine::RenderComponent>();
	auto textComponent = go->AddComponent<TextComponent>();
	textComponent->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	textComponent->SetText("Programming 4 Assignment");
	scene.Add(go);

	// Add Logo
	go = std::make_shared<Engine::GameObject>();
	std::shared_ptr<Texture2D> logoTexture{ ResourceManager::GetInstance().LoadTexture("logo.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(logoTexture);
	go->GetComponent<TransformComponent>()->SetPosition(216.0f, 180.0f);
	scene.Add(go);

	// Add fps counter
	go = std::make_shared<Engine::GameObject>();
	go->AddComponent<Engine::FPSComponent>();
	go->AddComponent<Engine::RenderComponent>();
	go->AddComponent<Engine::TextComponent>();
	auto fpsFont = Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	fpsFont->GetFont();
	go->GetComponent<Engine::TextComponent>()->SetFont(fpsFont);
	go->GetComponent<Engine::TransformComponent>()->SetPosition(10.0f, 10.0f);
	scene.Add(go);

	// Week 02
	auto rotationPoint = std::make_shared<GameObject>();
	rotationPoint->GetComponent<TransformComponent>()->SetPosition(100, 100);  // Parent
	scene.Add(rotationPoint);

	auto tank = std::make_shared<GameObject>();
	//go->SetParent(rotationPoint);
	tank->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BlueTank.png"));
	tank->GetComponent<TransformComponent>()->SetPosition(100, 150);
	scene.Add(tank);

	auto bullet = std::make_shared<GameObject>();
	//bullet->SetParent(tank);
	bullet->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletPlayer.png"));
	bullet->GetComponent<TransformComponent>()->SetPosition(100, 200);
	scene.Add(bullet);
	
	
	
}

int main(int, char* []) {
	Engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}