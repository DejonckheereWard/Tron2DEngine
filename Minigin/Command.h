#pragma once
#include <iostream>

namespace Engine
{
	class GameObject;
	class Command
	{
	public:
		Command(GameObject* actor = nullptr):
			m_Actor{ actor }
		{};

		virtual ~Command() {};
		virtual void Execute() = 0;

		GameObject* GetActor() { return m_Actor; };

	private:
		GameObject* m_Actor;
	};
	
}