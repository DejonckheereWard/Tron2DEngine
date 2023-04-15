#pragma once
#include "Command.h"


using Engine::Command;

class OnReleaseCommand final: public Command
{
public:
	// Inherited via Command
	virtual void Execute(float) override
	{
		std::cout << "OnReleaseCommand\n";
	};
};

class OnPressCommand final: public Command
{
public:
	// Inherited via Command
	virtual void Execute(float) override
	{
		std::cout << "OnPressCommand\n";
	};
};

class PressedCommand final: public Command
{
public:
	// Inherited via Command
	virtual void Execute(float) override
	{
		std::cout << "PressedCommand\n";
	};
};


class MoveCommand final: public Command
{

public:
	MoveCommand(Engine::GameObject* actor, float movementspeed, const glm::vec2& movedirection): 
		Command(actor),
		m_MoveDirection{ movedirection },
		m_MovementSpeed{ movementspeed }
	{};

	// Inherited via Command
	virtual void Execute(float value) override
	{
		auto localPos = GetActor()->GetTransform()->GetLocalPosition();
		localPos += m_MoveDirection * m_MovementSpeed * value;
		GetActor()->GetTransform()->SetLocalPosition(localPos);
	};

private:
	glm::vec2 m_MoveDirection;
	float m_MovementSpeed;
};
