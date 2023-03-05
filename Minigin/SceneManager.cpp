#include "SceneManager.h"
#include "Scene.h"

void Engine::SceneManager::Init()
{
	for(auto& scene : m_scenes)
	{
		scene->Init();
	}
}

void Engine::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void Engine::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

Engine::Scene& Engine::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
