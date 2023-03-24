#include "SceneManager.h"
#include "Scene.h"

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

void Engine::SceneManager::Render() const
{
	for(auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

void Engine::SceneManager::OnImGui()
{

}

Engine::SceneManager::~SceneManager()
{
	//m_Scenes.clear();
}

Engine::SceneManager::SceneManager():
	m_Scenes{}
{}

Engine::Scene* Engine::SceneManager::CreateScene(const std::string& name)
{
	auto& scene = m_Scenes.emplace_back(std::make_unique<Scene>(name));

	// Return the scene
	return scene.get();
}

Engine::Scene* Engine::SceneManager::GetScene(const std::string& name) const
{
	auto sceneIt = std::find_if(begin(m_Scenes), end(m_Scenes), [name](const std::unique_ptr<Engine::Scene>& scene) { 
		return scene->GetName() == name; 
	});

	if(sceneIt != end(m_Scenes))
	{
		return (*sceneIt).get();
	}
	return nullptr;
}

void Engine::SceneManager::RemoveScene(const std::string& name)
{
	std::erase_if(m_Scenes, [name](std::unique_ptr<Engine::Scene>& scene) {
		return scene->GetName() == name;
	});
}
