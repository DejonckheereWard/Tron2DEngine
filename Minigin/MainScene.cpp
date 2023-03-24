#include "MainScene.h"
#include "ResourceManager.h"
#include "GameObject.h"

// Components
#include "FPSComponent.h" 
#include "TextComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ConstantRotator.h"

void MainScene::OnCreate()
{
	using namespace Engine;
	// Add background
	auto go = new GameObject();
	std::shared_ptr<Texture2D> bgTexture{ ResourceManager::GetInstance().LoadTexture("background.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(bgTexture);
	AddChild(go);

	// Add title
	go = new GameObject();
	go->GetTransform()->SetLocalPosition(80, 20);
	go->AddComponent<Engine::RenderComponent>();
	auto textComponent = go->AddComponent<TextComponent>();
	textComponent->SetFont(Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	textComponent->SetText("Programming 4 Assignment");
	AddChild(go);

	// Add Logo
	go = new GameObject();
	std::shared_ptr<Texture2D> logoTexture{ ResourceManager::GetInstance().LoadTexture("logo.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(logoTexture);
	go->GetTransform()->SetLocalPosition(216.0f, 180.0f);
	AddChild(go);

	// Add fps counter
	go = new GameObject();
	go->AddComponent<Engine::FPSComponent>();
	go->AddComponent<Engine::RenderComponent>();
	go->AddComponent<Engine::TextComponent>();
	auto fpsFont = Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	fpsFont->GetFont();
	go->GetComponent<Engine::TextComponent>()->SetFont(fpsFont);
	go->GetComponent<Engine::TransformComponent>()->SetLocalPosition(10.0f, 10.0f);
	AddChild(go);

	// Week 02
	auto rotationPoint = new GameObject();
	rotationPoint->GetTransform()->SetLocalPosition(250, 300);  // Parent
	AddChild(rotationPoint);

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
}

