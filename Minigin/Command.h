#pragma once
#include <iostream>
#include <optional>
#include <glm/vec2.hpp>

namespace Engine
{
	class GameObject;
	class Command
	{
	public:
		Command() = default;
		Command(GameObject* pOwner) : m_pOwner{ pOwner } {};
		virtual ~Command() = default;


		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		void Execute() { Execute(glm::vec2{ 1.0f, 0 }); }
		virtual void Execute(const glm::vec2& inputValue) = 0; 

		GameObject* GetOwner() { return m_pOwner; };

	private:
		GameObject* m_pOwner{ nullptr };

		
	};
	
}