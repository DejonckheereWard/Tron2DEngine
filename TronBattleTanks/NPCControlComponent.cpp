#include "NPCControlComponent.h"

#include "CollisionManager.h"
#include "CollisionComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "MoveComponent.h"
#include "TankGunComponent.h"
#include "TankTurretComponent.h"
#include "NavmeshComponent.h"

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

	RequireComponent<NavmeshComponent>();
	m_pNavmeshComponent = GetOwner()->GetComponent<NavmeshComponent>();
}



void NPCControlComponent::Update()
{
	using namespace Engine;
	
	// Find the target
	if (!m_pTarget)
		return;

	// Get the direction to the target
	const glm::vec2 targetPos{ m_pTarget->GetTransform()->GetPosition() };
	const glm::vec2 ownPos{ GetTransform()->GetPosition() };
	const glm::vec2 vectorToTarget{targetPos - ownPos};
	const glm::vec2 directionToTarget{ glm::normalize(vectorToTarget) };

	// Set own position in the navmesh
	const glm::vec2& centerPos{ m_pCollisionComponent->GetColliderCenter() };
	m_pNavmeshComponent->SetPosition(centerPos);
	
	// if the target is in range, set the destination to the target
	if (glm::length2(vectorToTarget) <= (m_DetectionRange * m_DetectionRange))
	{
		m_pNavmeshComponent->SetDestination(targetPos);
	}

	// Check if we have direct line of sight
	const glm::vec2& center{ m_pCollisionComponent->GetColliderCenter() };
	uint8_t collisionMask{ std::numeric_limits<uint8_t>::max() };
	collisionMask &= ~m_pCollisionComponent->GetLayer();  // Dont collide own layer

	Engine::HitInfo hitInfo{};
	if (CollisionManager::GetInstance().Raycast(center, directionToTarget, FLT_MAX, hitInfo, collisionMask))
	{
		if (hitInfo.pCollider != nullptr)
		{
			GameObject* pOther{ hitInfo.pCollider->GetOwner() };
			if (pOther == m_pTarget)
			{
				// Direct line of sight
				m_pNavmeshComponent->SetDestination(targetPos);
				m_pTurret->SetTurretDirection(directionToTarget);
				m_pGun->Shoot();
			}
		}
	}


	// Move towards the target using the navmesh
	const glm::vec2& navmeshPos{ m_pNavmeshComponent->GetNextInPath() };
	if (glm::length2(navmeshPos - ownPos) >= FLT_EPSILON)
	{
		const glm::vec2 navmeshDirection{ glm::normalize(navmeshPos - centerPos) };
		m_pMoveComponent->Move(navmeshDirection);
	}


}

void NPCControlComponent::Render() const
{
}
