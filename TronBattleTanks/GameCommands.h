#pragma once
#include "Command.h"
#include "ServiceLocator.h"
#include "TankGunComponent.h"
#include "TankTurretComponent.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/gtx/norm.hpp>
# pragma warning(pop)


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
		if (glm::length2(value) <= 0.2f)
			return;  // Only significant input wanted (even if outside deadzone)
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



// Debug commands
class DebugMoveCommand final : public Command
{

public:
	DebugMoveCommand(Engine::GameObject* actor) :
		Command(actor)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override
	{
		// Only for debugging, not frame independent, can make any object move this way.
		glm::vec2 localPos = GetOwner()->GetTransform()->GetLocalPosition();
		localPos += value;
		GetOwner()->GetTransform()->SetLocalPosition(localPos);
	};

};

class DebugPrintPositionCommand final : public Command
{
	DebugPrintPositionCommand(Engine::GameObject* actor) :
		Command(actor)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& /*value*/) override
	{
		// Only for debugging, not frame independent, can make any object move this way.
		glm::vec2 localPos = GetOwner()->GetTransform()->GetLocalPosition();

		std::cout << "(" << localPos.x << ", " << localPos.y << ")\n";

	};

};
