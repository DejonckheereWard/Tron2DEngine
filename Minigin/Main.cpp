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

// Components
#include "FPSComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

#include "ConstantRotator.h"

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
	go->GetTransform()->SetLocalPosition(80, 20);
	go->AddComponent<Engine::RenderComponent>();
	auto textComponent = go->AddComponent<TextComponent>();
	textComponent->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	textComponent->SetText("Programming 4 Assignment");
	scene.Add(go);

	// Add Logo
	go = std::make_shared<Engine::GameObject>();
	std::shared_ptr<Texture2D> logoTexture{ ResourceManager::GetInstance().LoadTexture("logo.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(logoTexture);
	go->GetTransform()->SetLocalPosition(216.0f, 180.0f);
	scene.Add(go);

	// Add fps counter
	go = std::make_shared<Engine::GameObject>();
	go->AddComponent<Engine::FPSComponent>();
	go->AddComponent<Engine::RenderComponent>();
	go->AddComponent<Engine::TextComponent>();
	auto fpsFont = Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	fpsFont->GetFont();
	go->GetComponent<Engine::TextComponent>()->SetFont(fpsFont);
	go->GetComponent<Engine::TransformComponent>()->SetLocalPosition(10.0f, 10.0f);
	scene.Add(go);

	// Week 02
	auto rotationPoint = std::make_shared<GameObject>();
	rotationPoint->GetTransform()->SetLocalPosition(250, 300);  // Parent
	rotationPoint->AddComponent<ConstantRotator>();
	scene.Add(rotationPoint);

	auto tank = std::make_shared<GameObject>();
	tank->SetParent(rotationPoint.get());
	tank->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletNPC.png"));
	tank->GetTransform()->SetLocalPosition(20, 20);
	tank->AddComponent<ConstantRotator>()->SetRotationSpeed(180.0f);
	scene.Add(tank);

	auto bullet = std::make_shared<GameObject>();
	bullet->GetTransform()->SetLocalPosition(20, 20);
	bullet->SetParent(tank.get(), false);
	bullet->AddComponent<RenderComponent>()->SetTexture(ResourceManager::GetInstance().LoadTexture("Sprites/BulletPlayer.png"));
	scene.Add(bullet);
	
	
	
}

int main(int, char* []) {
	Engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}