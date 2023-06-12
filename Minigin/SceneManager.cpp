#include "SceneManager.h"
#include "Scene.h"
#include <cassert>

void Engine::SceneManager::Init()
{
	if(m_MainScene)
		m_MainScene->Init();
	for(auto& scene : m_Scenes)
	{
		scene->Init();
	}
}

void Engine::SceneManager::Update()
{
	if(m_MainScene)
		m_MainScene->Update();
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void Engine::SceneManager::FixedUpdate()
{
	if(m_MainScene)
		m_MainScene->FixedUpdate();
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void Engine::SceneManager::Render() const
{
	if(m_MainScene)
		m_MainScene->Render();
	for(auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

void Engine::SceneManager::RenderDebug() const
{
	// Seperate loop for debug rendering so it's always on top
	if(m_MainScene)
		m_MainScene->RenderDebug();
	for (auto& scene : m_Scenes)
	{
		scene->RenderDebug();
	}
}

void Engine::SceneManager::OnImGui()
{
	if(m_MainScene)
		m_MainScene->OnImGui();
	for(auto& scene : m_Scenes)
	{
		scene->OnImGui();
	}
}

void Engine::SceneManager::Cleanup()
{
	if(m_MainScene)
		m_MainScene->Cleanup();
	for(auto& scene : m_Scenes)
	{
		scene->Cleanup();
	}
}

void Engine::SceneManager::DestroyAll()
{
	if(m_MainScene)
		m_MainScene.reset();
	for (auto& scene : m_Scenes)
	{
		scene.reset();
	}
}

std::vector<Engine::GameObject*> Engine::SceneManager::GetChildrenWithTag(const std::string& tag) const
{
	std::vector<GameObject*> children{};
	if (m_MainScene)
	{
		const std::vector<GameObject*>& mainChildren = m_MainScene->GetChildrenWithTag(tag);
		children.insert(children.end(), mainChildren.begin(), mainChildren.end());
	}
	for (auto& scene : m_Scenes)
	{
		const std::vector<GameObject*>& sceneChildren = scene->GetChildrenWithTag(tag);
		children.insert(children.end(), sceneChildren.begin(), sceneChildren.end());
	}
	return children;

}

Engine::SceneManager::~SceneManager()
{
	
	//m_Scenes.clear();
}

Engine::SceneManager::SceneManager():
	m_Scenes{}
{}

Engine::Scene* Engine::SceneManager::CreateMainScene()
{
	m_MainScene = std::make_unique<Scene>("MainScene");
	return m_MainScene.get();
}

Engine::Scene* Engine::SceneManager::CreateScene(const std::string& name)
{
	auto& scene = m_Scenes.emplace_back(std::make_unique<Scene>(name));

	// Return the scene
	return scene.get();
}

Engine::Scene* Engine::SceneManager::GetMainScene() const
{
	assert(m_MainScene != nullptr && "Main scene is not created");
	return m_MainScene.get();
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
