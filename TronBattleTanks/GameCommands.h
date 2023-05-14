#pragma once
#include "Command.h"
#include "ServiceLocator.h"
#include "TankGunComponent.h"
#include "TankTurretComponent.h"

using Engine::Command;

// Game commands
class MoveCommand final: public Command
{

public:
	MoveCommand(Engine::GameObject* actor, float movementspeed, const glm::vec2& movedirection):
		Command(actor),
		m_MoveDirection{ movedirection },
		m_MovementSpeed{ movementspeed }
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override
	{
		auto localPos = GetOwner()->GetTransform()->GetLocalPosition();
		localPos += m_MoveDirection * m_MovementSpeed * value;
		GetOwner()->GetTransform()->SetLocalPosition(localPos);
	};

private:
	glm::vec2 m_MoveDirection;
	float m_MovementSpeed;
};

class AimTurretCommand final: public Command
{
public:
	AimTurretCommand(Engine::GameObject* pOwner):
		Command(pOwner)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override
	{
		const glm::vec2 normalizedValue{ glm::normalize(value) };
		GetOwner()->GetComponent<TankTurretComponent>()->SetTurretDirection(normalizedValue);
	}
};

class ShootCommand final: public Command
{
public:
	ShootCommand(Engine::GameObject* pOwner):
		Command(pOwner)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& /*value*/) override
	{
		GetOwner()->GetComponent<TankGunComponent>()->Shoot();
	}
};
