#pragma once
#include "Command.h"
#include "ServiceLocator.h"
#include "TankGunComponent.h"

using Engine::Command;


// Testing
class OnReleaseCommand final: public Command
{
public:
	// Inherited via Command
	virtual void Execute(const glm::vec2&) override
	{
		std::cout << "OnReleaseCommand\n";
	};
};

class OnPressCommand final: public Command
{
public:
	// Inherited via Command
	virtual void Execute(const glm::vec2&) override
	{
		std::cout << "OnPressCommand\n";
		Engine::ServiceLocator::GetAudioService().PlayEffect("Sounds/Background.mp3", 1.0f);
	};
};

class PressedCommand final: public Command
{
public:
	// Inherited via Command
	virtual void Execute(const glm::vec2&) override
	{
		std::cout << "PressedCommand\n";
	};
};


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


class DamagePlayer final: public Command
{
public:
	DamagePlayer(Engine::GameObject* actor, int dmgAmount):
		Command(actor),
		m_DamageAmount{ dmgAmount }
	{
		m_pHealthComponent = GetOwner()->GetComponent<HealthComponent>();
	};

	// Inherited via Command
	virtual void Execute(const glm::vec2&) override
	{
		m_pHealthComponent->TakeDamage(m_DamageAmount);
	};

private:
	HealthComponent* m_pHealthComponent;
	int m_DamageAmount;
};

class AddScore final: public Command
{
public:
	AddScore(Engine::GameObject* actor, int scoreAmount):
		Command(actor),
		m_ScoreAmount{ scoreAmount }
	{
		m_pScoreComponent = GetOwner()->GetComponent<ScoreComponent>();
	};

	// Inherited via Command
	virtual void Execute(const glm::vec2&) override
	{
		m_pScoreComponent->AddScore(m_ScoreAmount);
	};

private:
	ScoreComponent* m_pScoreComponent;
	int m_ScoreAmount;

};

class AimGunCommand final: public Command
{
public:
	AimGunCommand(Engine::GameObject* pOwner):
		Command(pOwner)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override
	{
		GetOwner()->GetComponent<TankGunComponent>()->Shoot();

		const glm::vec2 normalizedValue{ glm::normalize(value) };
		GetOwner()->GetComponent<TankGunComponent>()->SetGunDirection(normalizedValue);
	}
};