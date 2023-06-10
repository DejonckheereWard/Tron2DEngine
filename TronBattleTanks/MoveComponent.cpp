#include "MoveComponent.h"
#include "TransformComponent.h"
#include <algorithm>

#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/gtx/norm.hpp>
# pragma warning(pop)

void MoveComponent::Init()
{
	RequireComponent<Engine::TransformComponent>();
}

void MoveComponent::Update()
{
	const float deltaTime{ GameTimer::GetInstance().GetDeltaTime() };
	// Limit the movement direction to only 4 directions
	// Get the biggest value of the direction vector and set the other values to 0
	if (abs(m_MoveDirection.x) > abs(m_MoveDirection.y))
	{
		m_MoveDirection.x = std::clamp(m_MoveDirection.x, -1.0f, 1.0f);
		m_MoveDirection.y = 0;
	}
	else
	{
		m_MoveDirection.y = std::clamp(m_MoveDirection.y, -1.0f, 1.0f);
		m_MoveDirection.x = 0;
	}

	// Also check if the movement is significant enough to move
	if(glm::length2(m_MoveDirection) <= (FLT_EPSILON * FLT_EPSILON))
	{
		return;
	}

	// Move the owner
	glm::vec2 localPos = GetOwner()->GetTransform()->GetLocalPosition();
	localPos += m_MoveDirection * m_Speed * deltaTime;  // Frame independent movement
	GetOwner()->GetTransform()->SetLocalPosition(localPos);

	m_MoveDirection = glm::vec2(0, 0); // Reset the movement direction
}

void MoveComponent::Render() const
{
}
