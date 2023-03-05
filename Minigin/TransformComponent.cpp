#include "TransformComponent.h"

Engine::TransformComponent::TransformComponent(GameObject* pOwner):
	BaseComponent(pOwner)
{
}

void Engine::TransformComponent::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void Engine::TransformComponent::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

const glm::vec3& Engine::TransformComponent::GetPosition() const
{
	return m_Position;
}
