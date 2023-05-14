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
	MoveCommand(Engine::GameObject* actor, float movementspeed):
		Command(actor),
		m_MovementSpeed{ movementspeed }
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override
	{
		auto localPos = GetOwner()->GetTransform()->GetLocalPosition();
		localPos += value * m_MovementSpeed;
		GetOwner()->GetTransform()->SetLocalPosition(localPos);
	};

private:
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
