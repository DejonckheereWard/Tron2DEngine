#include "NPCControlComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include "MoveComponent.h"
#include "TankGunComponent.h"
#include "TankTurretComponent.h"

void NPCControlComponent::Init()
{
	RequireComponent<MoveComponent>();
	m_pMoveComponent = GetOwner()->GetComponent<MoveComponent>();
}



void NPCControlComponent::Update(float /*deltaTime*/)
{
	// Find the target
	if(!m_pTarget)
		return;

	// Get the direction to the target
	glm::vec2 targetPos{ m_pTarget->GetTransform()->GetPosition() };
	glm::vec2 ownPos{ GetOwner()->GetTransform()->GetPosition() };
	glm::vec2 directionToTarget{ glm::normalize(targetPos - ownPos) };

	m_pMoveComponent->Move(directionToTarget);

	// Shoot target
	m_pTurret->SetTurretDirection(directionToTarget);
	m_pGun->Shoot();

}

void NPCControlComponent::Render() const
{
}
