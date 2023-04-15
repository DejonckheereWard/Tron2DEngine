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
		virtual void Execute(float inputValue=1.0f) = 0;  // Value for analog input

		GameObject* GetActor() { return m_Actor; };

	private:
		GameObject* m_Actor;
	};
	
}