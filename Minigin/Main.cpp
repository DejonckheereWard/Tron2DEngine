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
#include "TextObject.h"
#include "Scene.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "UIComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->SetTexture("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto fpsCounter = std::make_shared<dae::GameObject>();
	fpsCounter->AddComponent<dae::FPSComponent>();
	fpsCounter->AddComponent<dae::TextComponent>();
	fpsCounter->AddComponent<dae::UIComponent>();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);	

	fpsCounter->GetComponent<dae::TextComponent>()->SetFont(font);
	fpsCounter->GetComponent<dae::TextComponent>()->SetText("Test");
	fpsCounter->SetPosition(10, 10);
	scene.Add(fpsCounter);

	auto to = std::make_shared<dae::TextObject>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	scene.Add(to);
	

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}