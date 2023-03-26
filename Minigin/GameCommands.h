#pragma once
#include "Command.h"


using Engine::Command;
using Engine::AxisCommand;

class OnReleaseCommand: public Command
{
public:
	OnReleaseCommand(Engine::GameObject* actor): Command(actor) {};
	// Inherited via Command
	virtual void Execute() override
	{
		std::cout << "OnReleaseCommand\n";
	};
};

class OnPressCommand: public Command
{
public:
	OnPressCommand(Engine::GameObject* actor): Command(actor) {};
	// Inherited via Command
	virtual void Execute() override
	{
		std::cout << "OnPressCommand\n";
	};
};

class PressedCommand: public Command
{
public:
	PressedCommand(Engine::GameObject* actor): Command(actor) {};
	// Inherited via Command
	virtual void Execute() override
	{
		std::cout << "PressedCommand\n";
	};
};


class MoveRight: public AxisCommand
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

class MoveForward: public AxisCommand
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