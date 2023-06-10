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
	m_IsInitialized = true;
}

void Engine::Scene::Update()
{
	for(auto& child : m_Children)
	{
		child->Update();
	}


	if (m_ChildrenToAdd.size() > 0)
	{
		m_Children.reserve(m_ChildrenToAdd.size());
		for (auto& child : m_ChildrenToAdd)
		{
			m_Children.emplace_back(child);
		}
		m_ChildrenToAdd.clear();
	}
}

void Engine::Scene::FixedUpdate()
{
	for (auto& child : m_Children)
	{
		child->FixedUpdate();
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

Engine::GameObject* Engine::Scene::AddChild(GameObject* child)
{
	if (m_IsInitialized)
	{
		child->Init();
		m_ChildrenToAdd.emplace_back(child);
		return child;  // Return child for chaining, saving inplace
	}
	m_Children.emplace_back(child);
	return child;
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