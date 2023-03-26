#include "Scene.h"
#include "GameObject.h"
#include <cassert>


Engine::Scene::Scene(const std::string& name):
	m_SceneName{ name },
	m_Children{}
{
}

Engine::Scene::~Scene()
{
	for(auto& child : m_Children)
	{
		delete child;
	}
}

void Engine::Scene::Init()
{
	for(auto& child : m_Children)
	{
		child->Init();
	}
}

void Engine::Scene::Update(float deltaTime)
{
	for(auto& child : m_Children)
	{
		child->Update(deltaTime);
	}
}

void Engine::Scene::Render() const
{
	for(auto& child : m_Children)
	{
		child->Render();
	}
}

void Engine::Scene::OnImGui()
{
	for(auto& child : m_Children)
	{
		child->OnImGui();
	}
}

void Engine::Scene::AddChild(GameObject* child)
{
	m_Children.push_back(child);
}

void Engine::Scene::RemoveChild(GameObject* child)
{
	std::erase(m_Children, child);
}

void Engine::Scene::RemoveChildIndex(size_t index)
{
	assert(index >= 0 && index < m_Children.size() && "Index out of bounds");
	m_Children.erase(m_Children.begin() + index);
}