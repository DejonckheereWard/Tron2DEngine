#include "NPCControlComponent.h"

#include "SceneManager.h"

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
	bool aimTowardsMovement{ true };
	

	const glm::vec2& centerPos{ m_pCollisionComponent->GetColliderCenter() };
	m_pNavmeshComponent->SetPosition(centerPos);

	// Move towards the target using the navmesh
	const glm::vec2& navmeshPos{ m_pNavmeshComponent->GetNextInPath() };
	if (glm::length2(navmeshPos - centerPos) >= FLT_EPSILON)
	{
		const glm::vec2 navmeshDirection{ glm::normalize(navmeshPos - centerPos) };
		m_pMoveComponent->Move(navmeshDirection);

		if (aimTowardsMovement)
		{
			m_pTurret->SetTurretDirection(navmeshDirection);
		}
	}

	// Check if we can shoot target if we have one
	if (m_pTarget)
	{
		// Get the direction to the target
		const glm::vec2 targetPos{ m_pTarget->GetComponent<CollisionComponent>()->GetColliderCenter() };
		const glm::vec2 vectorToTarget{targetPos - centerPos};
		const glm::vec2 directionToTarget{ glm::normalize(vectorToTarget) };

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
					// Direct line of sight -> Shoot
					m_pTurret->SetTurretDirection(directionToTarget);
					m_pGun->Shoot();
					aimTowardsMovement = false;
				}
			}
		}
	}




}

void NPCControlComponent::Render() const
{
}

Engine::GameObject* NPCControlComponent::LookForTarget() const
{
	// Find all objects with player tag
	Engine::SceneManager& sceneManager{ Engine::SceneManager::GetInstance() };
	const std::vector<Engine::GameObject*> pPlayers{ sceneManager.GetChildrenWithTag("Player") };

	// Find the closest player
	Engine::GameObject* pClosestPlayer{ nullptr };
	float closestDistance{ FLT_MAX };
	glm::vec2 directionToTarget{ 0.f, 0.f };

	const glm::vec2& ownPos{ GetTransform()->GetPosition()  };
	for (Engine::GameObject* pPlayer : pPlayers)
	{
		const glm::vec2& playerPos{ pPlayer->GetTransform()->GetPosition() };
		const float distance{ glm::distance(playerPos, ownPos) };
		if (distance < closestDistance)
		{
			pClosestPlayer = pPlayer;
			closestDistance = distance;
			directionToTarget = glm::normalize(playerPos - ownPos);
		}
	}

	Engine::HitInfo hitInfo{};
	if (Engine::CollisionManager::GetInstance().Raycast(ownPos, directionToTarget, FLT_MAX, hitInfo, m_pCollisionComponent->GetCollisionMask() | Engine::CollisionLayer::Player | Engine::CollisionLayer::Friendly))
	{
		if (hitInfo.pCollider != nullptr)
		{
			Engine::GameObject* pOther{ hitInfo.pCollider->GetOwner() };
			if (pOther == pClosestPlayer)
			{
				return pClosestPlayer;
			}
		}
	}

	if (closestDistance <= m_DetectionRange)
	{
		return pClosestPlayer;
	}
	else
	{
		return nullptr;
	}
}
