#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"

dae::GameObject::~GameObject() = default;

// [[maybe_unused]] stops compiler errors when a parameter is unused when compiling
void dae::GameObject::Update(float deltaTime)
{
	// Go through all the components in the map and run update
	for(std::pair<const int, dae::BaseComponent*>& component : m_Components)
	{
		component.second->Update(*this, deltaTime);
	}
}

void dae::GameObject::Render() const
{
	const auto& pos = m_transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
