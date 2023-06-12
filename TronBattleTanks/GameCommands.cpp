#include "GameCommands.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include "TankGunComponent.h"
#include "TankTurretComponent.h"
#include "MoveComponent.h"

void MoveCommand::Execute(const glm::vec2& value)
{
	// Get movement component and move the actor
	if (!m_pMoveComponent)
	{
		m_pMoveComponent = GetOwner()->GetComponent<MoveComponent>();
	}

	m_pMoveComponent->Move(value);
}

void AimTurretCommand::Execute(const glm::vec2& value)
{
	constexpr float inputMargin{ 0.2f * 0.2f };
	if (glm::length2(value) <= inputMargin)
		return;  // Only significant input wanted (even if outside deadzone)
	const glm::vec2 normalizedValue{ glm::normalize(value) };
	GetOwner()->GetComponent<TankTurretComponent>()->SetTurretDirection(normalizedValue);
}

void ShootCommand::Execute(const glm::vec2& /*value*/)
{
	GetOwner()->GetComponent<TankGunComponent>()->Shoot();
}

void DebugMoveCommand::Execute(const glm::vec2& value)
{
	// Only for debugging, not frame independent, can make any object move this way.
	glm::vec2 localPos = GetOwner()->GetTransform()->GetLocalPosition();
	localPos += value;
	GetOwner()->GetTransform()->SetLocalPosition(localPos);
}

void DebugPrintPositionCommand::Execute(const glm::vec2& /*value*/)
{
	// Only for debugging, not frame independent, can make any object move this way.
	glm::vec2 localPos = GetOwner()->GetTransform()->GetLocalPosition();

	std::cout << "(" << localPos.x << ", " << localPos.y << ")\n";
}

void CallbackCommand::Execute(const glm::vec2& /*value*/)
{
	if (m_Callback)
	{
		m_Callback();
	}
}
