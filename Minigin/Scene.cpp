#include "Scene.h"
#include "GameObject.h"

void Engine::Scene::AddChild(GameObject* child)
{
	m_Children.push_back(std::move(child));
}

void Engine::Scene::RemoveChild(GameObject* child)
{
	std::erase(m_Children, child);
}

Engine::Scene::Scene(const std::string& name):
	m_SceneName{ name }
{
}

Engine::Scene::~Scene()
{
	for(Engine::GameObject* child : m_Children)
	{
		delete child;
	}
}

void Engine::Scene::Init()
{
	for(Engine::GameObject* child : m_Children)
	{
		child->Init();
	}
}

void Engine::Scene::Update(float deltaTime)
{
	for(Engine::GameObject* child : m_Children)
	{
		child->Update(deltaTime);
	}
}

void Engine::Scene::Render() const
{
	for(Engine::GameObject* child : m_Children)
	{
		child->Render();
	}
}