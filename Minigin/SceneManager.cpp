#include "SceneManager.h"
#include "SceneComponent.h"
#include "GameObject.h"



void Engine::SceneManager::Init()
{
	for(auto& scene : m_Scenes)
	{
		scene->Init();
	}
}

void Engine::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_Scenes)
	{
		scene->Update(deltaTime);
	}
}

void Engine::SceneManager::Render()
{
	for(auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

Engine::GameObject* Engine::SceneManager::CreateScene(const std::string& name)
{
	auto& scene = m_Scenes.emplace_back(std::make_unique<GameObject>());
	scene->AddComponent<SceneComponent>()->SetName(name);

	// Return the scene
	return scene.get();
}

Engine::GameObject* Engine::SceneManager::GetScene(const std::string& name) const
{
	for(const auto& scene : m_Scenes)
	{
		if(scene->GetComponent<SceneComponent>()->GetName() == name)
		{
			return scene.get();
		}
	}
	return nullptr;  // Not found
}

void Engine::SceneManager::RemoveScene(const std::string& name)
{
	for(auto& scene : m_Scenes)
	{
		if(scene->GetComponent<SceneComponent>()->GetName() == name)
		{
			std::erase(m_Scenes, scene);  // Unique pointer, so will auto clean up
		}
	}
}
