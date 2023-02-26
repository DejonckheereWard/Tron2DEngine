#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"

dae::GameObject::~GameObject()
{
	for(std::pair<const std::type_index, dae::BaseComponent*>& component : m_Components)
	{
		delete component.second;
	}
	m_Components.clear();
};

// [[maybe_unused]] stops compiler errors when a parameter is unused when compiling
void dae::GameObject::Update(float deltaTime)
{
	// Go through all the components in the map and run update
	for(std::pair<const std::type_index, dae::BaseComponent*>& component : m_Components)
	{
		component.second->Update(*this, deltaTime);
	}
}

void dae::GameObject::Render() const
{
	const auto& pos = m_Transform.GetPosition();
	if(m_texture)
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);

	for(const std::pair<const std::type_index, dae::BaseComponent*>& component : m_Components)
	{
		component.second->Render(*this);
	}
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
