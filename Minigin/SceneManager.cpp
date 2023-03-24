#include "SceneManager.h"
#include "Scene.h"

void Engine::SceneManager::Init()
{
	for(auto& scene : m_Scenes)
	{
		scene->OnCreate();
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

void Engine::SceneManager::Render() const
{
	for(auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

void Engine::SceneManager::OnImGui()
{
	for(Scene* scene : m_Scenes)
	{
		scene->OnImGui();
	}
}

Engine::SceneManager::~SceneManager()
{
	for(Scene* scene : m_Scenes)
	{
		delete scene;
	}
	m_Scenes.clear();
}

Engine::SceneManager::SceneManager()
{}

void Engine::SceneManager::AddScene(Scene* scene)
{
	m_Scenes.push_back(scene);
}

Engine::Scene* Engine::SceneManager::GetScene(const std::string& name) const
{
	return *std::find_if(begin(m_Scenes), end(m_Scenes), [name](Scene* scene) { return scene->GetName() == name; });
}

void Engine::SceneManager::RemoveScene(const std::string& name)
{
	std::erase_if(m_Scenes, [name](Scene* scene) { return scene->GetName() == name; });
}
