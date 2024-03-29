#include "MoveComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include <algorithm>
#include "Renderer.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/gtx/norm.hpp>
# pragma warning(pop)

void MoveComponent::Init()
{
	if (GetOwner()->HasComponent<Engine::CollisionComponent>())
	{
		m_pCollisionComponent = GetOwner()->GetComponent<Engine::CollisionComponent>();
	}
}

void MoveComponent::Update()
{
	
}

void MoveComponent::FixedUpdate()
{
	const float fixedDeltaTime{ GameTimer::GetInstance().GetFixedDeltaTime() };

	// Also check if the movement is significant enough to move
	if (glm::length2(m_MoveDirection) <= FLT_EPSILON)
	{
		return;
	}

	ClampDirectionToAxis(m_MoveDirection);  // Inplace
	m_PreviousMoveDirection = m_MoveDirection;

	// Can we move in the direction we want to move?
	if (!CanMove(m_MoveDirection))
	{
		// If not, try to move in the other axis, use the prefered direction  for that axis
		glm::vec2 temp{};
		if (abs(m_MoveDirection.x) > abs(m_MoveDirection.y))
		{
			temp = glm::vec2(0, m_PreferedMoveDirection.y);


		}
		else
		{
			temp = glm::vec2(m_PreferedMoveDirection.x, 0);
		}

		if (CanMove(temp))
		{
			m_MoveDirection = temp;
		}
		else if (CanMove(-temp))
		{
			m_MoveDirection = -temp;
		}
		else
		{
			// If we can't move in any direction, don't move
			m_MoveDirection = glm::vec2(0, 0);
			return;
		}
	}

	// Move the owner
	glm::vec2 localPos = GetOwner()->GetTransform()->GetLocalPosition();
	localPos += m_MoveDirection * m_Speed * fixedDeltaTime;  // Frame independent movement
	GetOwner()->GetTransform()->SetLocalPosition(localPos);

	// Get dominant axis, save that axis's direction as prefered direction for future movement
	if (std::abs(m_MoveDirection.x) > std::abs(m_MoveDirection.y))
	{
		m_PreferedMoveDirection.x = m_MoveDirection.x;
	}
	else
	{
		m_PreferedMoveDirection.y = m_MoveDirection.y;
	}

	m_MoveDirection = glm::vec2(0, 0); // Reset the movement direction
}

void MoveComponent::Render() const
{
}

void MoveComponent::RenderDebug() const
{
	using namespace Engine;

	if (not m_RenderDebug)
		return;

	// Get center of the owner
	glm::vec2 center{ GetTransform()->GetPosition() };

	if (GetOwner()->HasComponent<CollisionComponent>())
	{
		CollisionComponent* pCollision{ GetOwner()->GetComponent<CollisionComponent>() };
		center = pCollision->GetColliderCenter();
	}

	// Draw the movement direction
	Renderer::GetInstance().RenderLine(center, center + m_PreviousMoveDirection * 20.0f);
}

bool MoveComponent::CanMove(const glm::vec2& direction) const
{
	assert(glm::length(direction) > 0.0f && "Direction vector cannot be zero length!");

	using namespace Engine;
	if (m_pCollisionComponent == nullptr)
	{
		return true;  // If the owner doesn't have a collision component, we can always move
	}

	// Get the collision layer of the owner
	uint8_t collisionMask{ m_pCollisionComponent->GetCollisionMask() };
	collisionMask &= ~static_cast<uint8_t>(CollisionLayer::TriggerLayer);  // Ignore trigger layers

	// Get center of the owner
	const glm::vec2& center{ m_pCollisionComponent->GetColliderCenter() };

	// Offset to get both side of the owner
	glm::vec2 offset{ m_pCollisionComponent->GetColliderSize() / 2.0f };
	offset -= 2.0f; // Subtract for smoother working (doesnt get stuck in wall in 2 directions)

	float distance{};
	if (std::abs(direction.x) > std::abs(direction.y))
	{
		distance = m_pCollisionComponent->GetColliderSize().x / 2.0f;
		offset.x = 0;
	}
	else
	{
		distance = m_pCollisionComponent->GetColliderSize().y / 2.0f;
		offset.y = 0;
	}

	// Check if we can move in the given direction
	Engine::HitInfo hitInfo{};
	if (CollisionManager::GetInstance().Raycast(center + offset, direction, distance, hitInfo, collisionMask))
	{
		return false;
	}

	hitInfo = Engine::HitInfo{};  // Reset the hit info
	if (CollisionManager::GetInstance().Raycast(center, direction, distance, hitInfo, collisionMask))
	{
		return false;
	}

	hitInfo = Engine::HitInfo{};  // Reset the hit info
	if (CollisionManager::GetInstance().Raycast(center - offset, direction, distance, hitInfo, collisionMask))
	{
		return false;
	}

	return true;
}

void MoveComponent::ClampDirectionToAxis(glm::vec2& direction)
{

	// Limit the movement direction to only 4 directions
	// Fixes the direction so it it only goes along either the x or y axis
	if (abs(direction.x) > abs(direction.y))
	{
		direction.x = (direction.x > 0) ? 1.0f : -1.0f;
		direction.y = 0;
	}
	else
	{
		direction.x = 0;
		direction.y = (direction.y > 0) ? 1.0f : -1.0f;
	}

}
