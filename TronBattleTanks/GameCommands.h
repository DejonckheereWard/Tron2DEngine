#pragma once
#include "Command.h"
#include "ServiceLocator.h"
#include "TankGunComponent.h"
#include "TankTurretComponent.h"

using Engine::Command;

// Game commands
class MoveCommand final : public Command
{

public:
	MoveCommand(Engine::GameObject* actor) :
		Command(actor),
		m_pMoveComponent{ nullptr }
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override
	{
		// Get movement component and move the actor
		if (!m_pMoveComponent)
		{
			m_pMoveComponent = GetOwner()->GetComponent<MoveComponent>();
		}

		m_pMoveComponent->Move(value);
	};

private:
	MoveComponent* m_pMoveComponent;
};

class AimTurretCommand final : public Command
{
public:
	AimTurretCommand(Engine::GameObject* pOwner) :
		Command(pOwner)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override
	{
		const glm::vec2 normalizedValue{ glm::normalize(value) };
		GetOwner()->GetComponent<TankTurretComponent>()->SetTurretDirection(normalizedValue);
	}
};

class ShootCommand final : public Command
{
public:
	ShootCommand(Engine::GameObject* pOwner) :
		Command(pOwner)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& /*value*/) override
	{
		GetOwner()->GetComponent<TankGunComponent>()->Shoot();
	}
};
