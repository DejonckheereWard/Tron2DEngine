#pragma once
#include "Command.h"
#include "ServiceLocator.h"
#include <functional>

#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/gtx/norm.hpp>
# pragma warning(pop)


using Engine::Command;
class MoveComponent;
class TankTurretComponent;
class TankGunComponent;

// Game commands
class MoveCommand final : public Command
{

public:
	MoveCommand(Engine::GameObject* actor) :
		Command(actor),
		m_pMoveComponent{ nullptr }
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override;

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
	virtual void Execute(const glm::vec2& value) override;
};

class ShootCommand final : public Command
{
public:
	ShootCommand(Engine::GameObject* pOwner) :
		Command(pOwner)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override;
};


// Special commands
class CallbackCommand final : public Command
{
public:
	CallbackCommand(Engine::GameObject* actor, std::function<void()> callback) :
		Command(actor),
		m_Callback{ callback }
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override;

private:
	std::function<void()> m_Callback;

};


// Debug commands
class DebugMoveCommand final : public Command
{

public:
	DebugMoveCommand(Engine::GameObject* actor) :
		Command(actor)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override;

};

class DebugPrintPositionCommand final : public Command
{
	DebugPrintPositionCommand(Engine::GameObject* actor) :
		Command(actor)
	{};

	// Inherited via Command
	virtual void Execute(const glm::vec2& value) override;

};
