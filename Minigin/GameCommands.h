#pragma once
#include "Command.h"


using Engine::Command;
using Engine::AxisCommand;

class OnReleaseCommand final: public Command
{
public:
	OnReleaseCommand(Engine::GameObject* actor): Command(actor) {};
	// Inherited via Command
	virtual void Execute() override
	{
		std::cout << "OnReleaseCommand\n";
	};
};

class OnPressCommand final: public Command
{
public:
	OnPressCommand(Engine::GameObject* actor): Command(actor) {};
	// Inherited via Command
	virtual void Execute() override
	{
		std::cout << "OnPressCommand\n";
	};
};

class PressedCommand final: public Command
{
public:
	PressedCommand(Engine::GameObject* actor): Command(actor) {};
	// Inherited via Command
	virtual void Execute() override
	{
		std::cout << "PressedCommand\n";
	};
};


class MoveRight final: public AxisCommand
{

public:
	MoveRight(Engine::GameObject* actor, bool invertAxis = false): 
		AxisCommand(actor, invertAxis) {};

	// Inherited via Command
	virtual void Execute(float value) override
	{
		if(m_InvertAxis)
			value = -value;
		auto localPos = GetActor()->GetTransform()->GetLocalPosition();
		localPos.x = localPos.x + value;
		GetActor()->GetTransform()->SetLocalPosition(localPos);
	};
};

class MoveForward final: public AxisCommand
{

public:
	MoveForward(Engine::GameObject* actor, bool invertAxis = false):
		AxisCommand(actor, invertAxis) {};

	// Inherited via Command
	virtual void Execute(float value) override
	{
		if(m_InvertAxis)
			value = -value;

		auto localPos = GetActor()->GetTransform()->GetLocalPosition();
		localPos.y = localPos.y - value;
		GetActor()->GetTransform()->SetLocalPosition(localPos);
	};
};