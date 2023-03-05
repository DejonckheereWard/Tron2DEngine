#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

Engine::GameObject::GameObject():
	m_pTransform{ AddComponent<TransformComponent>() }
{
}

Engine::GameObject::~GameObject()
{
	for(std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		delete component.second;
	}
	m_Components.clear();
}

Engine::GameObject* Engine::GameObject::GetParent()
{
	return m_Parent;
}

void Engine::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if(parent)
	{
		// Update the worldtransform such that the object stays in the same position/rotation and scale
		if(keepWorldPosition)
		{
			m_pTransform->SetLocalPosition(m_pTransform->GetLocalPosition() - parent->GetTransform()->GetPosition());
		}
		m_pTransform->SetDirty();
	}
	else 
	{
		m_pTransform->SetLocalPosition(m_pTransform->GetPosition());
	}


	// Remove from previous parent if exist
	if(m_Parent)
	{		
		m_Parent->RemoveChild(this);
	}

	// Update the parent
	m_Parent = parent;

	// Add to the new parent if it exists
	if(m_Parent)
	{
		m_Parent->AddChild(this);
	}



}

void Engine::GameObject::AddChild(GameObject* child)
{
	m_Children.emplace_back(child);
}

void Engine::GameObject::RemoveChild(GameObject* child)
{
	std::erase(m_Children, child);
}

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

