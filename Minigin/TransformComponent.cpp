#include "TransformComponent.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner):
	BaseComponent(pOwner)
{
}

void dae::TransformComponent::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void dae::TransformComponent::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

const glm::vec3& dae::TransformComponent::GetPosition() const
{
	return m_Position;
}
