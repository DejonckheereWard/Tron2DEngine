#include "NPCControlComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include "MoveComponent.h"
#include "TankGunComponent.h"
#include "TankTurretComponent.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/gtx/norm.hpp>
# pragma warning(pop)

void NPCControlComponent::Init()
{
	RequireComponent<MoveComponent>();
	m_pMoveComponent = GetOwner()->GetComponent<MoveComponent>();
}



void NPCControlComponent::Update()
{
	// Find the target
	if (!m_pTarget)
		return;

	// Get the direction to the target
	const glm::vec2 targetPos{ m_pTarget->GetTransform()->GetPosition() };
	const glm::vec2 ownPos{ GetOwner()->GetTransform()->GetPosition() };
	const glm::vec2 vectorToTarget{targetPos - ownPos};

	if (glm::length2(vectorToTarget) >= (m_DetectionRange * m_DetectionRange))
		return;  // Target too far

	const glm::vec2 directionToTarget{ glm::normalize(vectorToTarget) };


	m_pMoveComponent->Move(directionToTarget);

	// Shoot target
	m_pTurret->SetTurretDirection(directionToTarget);
	m_pGun->Shoot();

}

void NPCControlComponent::Render() const
{
}
