#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

Engine::GameObject::GameObject()
{
	AddComponent<TransformComponent>();  // Add a transformcomponent by default;
}

Engine::GameObject::~GameObject()
{
	for(std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		delete component.second;
	}
	m_Components.clear();
};

void Engine::GameObject::Init()
{
	for(std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		component.second->Init();
	}	
}

void Engine::GameObject::Update(float deltaTime)
{
	// Go through all the components in the map and run update
	for(std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		component.second->Update(deltaTime);
	}
}

void Engine::GameObject::Render() const
{
	// Go through all the components in the map and run render
	for(const std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		component.second->Render();
	}
}

