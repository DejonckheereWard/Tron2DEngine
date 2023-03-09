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
	// Delete children
	for(GameObject* child : m_Children)
	{
		delete child;  // Will run destructor and delete subchildren too
	}
	m_Children.clear();


	// Delete components
	for(std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		delete component.second;
	}
	m_Components.clear();
}


void Engine::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	// Parent shouldnt be null, should default to the parent scene gameobject.
	assert(parent != nullptr);

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
		m_Parent->RemoveFromChildCollection(this);
	}

	// Update the parent
	m_Parent = parent;

	// Add to the new parent if it exists
	if(m_Parent)
	{
		m_Parent->AddToChildCollection(this);
		//m_Parent->Add(this);
	}
}

void Engine::GameObject::AddChild(GameObject* child, bool keepWorldPosition)
{
	// Takes ownership over this child, calls setparent to do the work.
	child->SetParent(this, keepWorldPosition);
	//m_Children.emplace_back(child);
}

void Engine::GameObject::RemoveChild(GameObject* child)
{
	std::erase(m_Children, child);  // Remove from children collection
	delete child;  // Will call its destructor, deleting subchildren too
}

void Engine::GameObject::AddToChildCollection(GameObject* child)
{
	m_Children.emplace_back(child);
}

void Engine::GameObject::RemoveFromChildCollection(GameObject* child)
{
	std::erase(m_Children, child);
}

void Engine::GameObject::Init()
{
	// GameObject has ownership over its children, init these
	for(GameObject* child : m_Children)
	{
		child->Init();
	}

	// Init components
	for(std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		component.second->Init();
	}
}

void Engine::GameObject::Update(float deltaTime)
{
	// GameObject has ownership over its children, update these
	for(GameObject* child: m_Children)
	{ 
		child->Update(deltaTime);
	}

	// Go through all the components in the map and run update
	for(std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		component.second->Update(deltaTime);
	}
}

void Engine::GameObject::Render() const
{
	// GameObject has ownership over its children, render these
	for(const GameObject* child : m_Children)
	{
		child->Render();
	}

	// Go through all the components in the map and run render
	for(const std::pair<const std::type_index, Engine::BaseComponent*>& component : m_Components)
	{
		component.second->Render();
	}
}

