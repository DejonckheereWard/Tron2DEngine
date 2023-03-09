#include "Scene.h"
#include "GameObject.h"


using namespace Engine;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() 
{
	for(GameObject* child : m_Children)
	{
		delete child;
	}
}

void Scene::Add(GameObject* object)
{
	m_Children.emplace_back(std::move(object));
}

void Scene::Remove(GameObject* object)
{
	std::erase(m_Children, object);
	//m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), object), m_Children.end());
}

void Scene::RemoveAll()
{
	m_Children.clear();
}

void Engine::Scene::Init()
{
	for(GameObject* child : m_Children)
	{
		child->Init();
	}
}

void Scene::Update(float deltaTime)
{
	for(GameObject* child : m_Children)
	{
		child->Update(deltaTime);
	}
}

void Scene::Render() const
{
	for(const GameObject* child : m_Children)
	{
		child->Render();
	}
}

