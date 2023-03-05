#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

dae::GameObject::GameObject()
{
	AddComponent<TransformComponent>();  // Add a transformcomponent by default;
}

dae::GameObject::~GameObject()
{
	for(std::pair<const std::type_index, dae::BaseComponent*>& component : m_Components)
	{
		delete component.second;
	}
	m_Components.clear();
};

void dae::GameObject::Init()
{
	for(std::pair<const std::type_index, dae::BaseComponent*>& component : m_Components)
	{
		component.second->Init();
	}	
}

void dae::GameObject::Update(float deltaTime)
{
	// Go through all the components in the map and run update
	for(std::pair<const std::type_index, dae::BaseComponent*>& component : m_Components)
	{
		component.second->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	// Go through all the components in the map and run render
	for(const std::pair<const std::type_index, dae::BaseComponent*>& component : m_Components)
	{
		component.second->Render();
	}
}

