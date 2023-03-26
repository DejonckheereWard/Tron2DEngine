#pragma once
#include <iostream>

namespace Engine
{
	class GameObject;
	class Command
	{
	public:
		Command(GameObject* actor): m_Actor(actor) {};
		virtual ~Command() {};
		virtual void Execute() = 0;

		GameObject* GetActor() { return m_Actor; };


	private:
		GameObject* m_Actor;
	};


	class AxisCommand
	{
	public:
		AxisCommand(GameObject* actor, bool invertAxis = false):
			m_Actor(actor), 
			m_InvertAxis{ invertAxis } 
		{};

		virtual ~AxisCommand() {};
		virtual void Execute(float value) = 0;

		GameObject* GetActor() { return m_Actor; };

	protected:
		bool m_InvertAxis;

	private:
		GameObject* m_Actor;

	};
}