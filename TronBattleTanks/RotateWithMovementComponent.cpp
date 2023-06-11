#include "RotateWithMovementComponent.h"
#include "MoveComponent.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>

void RotateWithMovementComponent::Init()
{
	if (!m_pMoveComponent)
	{
		RequireComponent<MoveComponent>();
		m_pMoveComponent = GetOwner()->GetComponent<MoveComponent>();
	}
}

void RotateWithMovementComponent::Update()
{
	// Get rotation from movement
	// Set own rotation
	const glm::vec2 desiredDirection{ m_pMoveComponent->GetPreviousMoveDirection() };
	

	const float angle{ glm::degrees(atan2f(desiredDirection.y, desiredDirection.x)) };

	GetOwner()->GetTransform()->SetLocalRotation(angle);


}
