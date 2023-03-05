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
	using namespace dae;
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// Week 01

	// Add background
	auto go = std::make_shared<dae::GameObject>();
	std::shared_ptr<Texture2D> bgTexture{ ResourceManager::GetInstance().LoadTexture("background.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(bgTexture);
	scene.Add(go);

	// Add title
	go = std::make_shared<dae::GameObject>();
	go->GetComponent<TransformComponent>()->SetPosition(80, 20);
	go->AddComponent<dae::RenderComponent>();
	auto textComponent = go->AddComponent<dae::TextComponent>();
	textComponent->SetFont(dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	textComponent->SetText("Programming 4 Assignment");
	scene.Add(go);

	// Add Logo
	go = std::make_shared<dae::GameObject>();
	std::shared_ptr<Texture2D> logoTexture{ ResourceManager::GetInstance().LoadTexture("logo.tga") };
	go->AddComponent<RenderComponent>()->SetTexture(logoTexture);
	go->GetComponent<TransformComponent>()->SetPosition(216.0f, 180.0f);
	scene.Add(go);

	// Add fps counter
	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::FPSComponent>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>();
	auto fpsFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	fpsFont->GetFont();
	go->GetComponent<dae::TextComponent>()->SetFont(fpsFont);
	go->GetComponent<dae::TransformComponent>()->SetPosition(10.0f, 10.0f);
	scene.Add(go);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}