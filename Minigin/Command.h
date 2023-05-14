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

		void Execute() { Execute(m_DefaultValue); }
		virtual void Execute(const glm::vec2& inputValue) = 0; 

		GameObject* GetOwner() { return m_pOwner; };
		void SetDefaultValue(const glm::vec2& defaultValue) { m_DefaultValue = defaultValue; }

	private:
		GameObject* m_pOwner{ nullptr };
		glm::vec2 m_DefaultValue{ 1.0f, 0.0f };

		
	};
	
}