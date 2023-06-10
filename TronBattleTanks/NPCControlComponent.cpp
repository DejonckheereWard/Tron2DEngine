#include "NPCControlComponent.h"

#include "CollisionManager.h"
#include "CollisionComponent.h"

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

	RequireComponent<Engine::CollisionComponent>();
	m_pCollisionComponent = GetOwner()->GetComponent<Engine::CollisionComponent>();
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

	// Check if we have direct line of sight

	using namespace Engine;
	const glm::vec2& center{ m_pCollisionComponent->GetColliderCenter() };

	Engine::HitInfo hitInfo{};
	if (CollisionManager::GetInstance().Raycast(center, directionToTarget, 100.0f, hitInfo))
	{
		if (hitInfo.pCollider->GetOwner() != m_pTarget)
			return; // No direct line of sight

	}


	m_pTurret->SetTurretDirection(directionToTarget);
	m_pGun->Shoot();


}

void NPCControlComponent::Render() const
{
}
