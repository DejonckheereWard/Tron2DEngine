#pragma once
#include "BaseComponent.h"
#include <glm/glm.hpp>

namespace Engine
{
	class TransformComponent: public BaseComponent
	{
	public:
		TransformComponent(GameObject* pOwner);

		// Inherited via BaseComponent
		virtual void Init() override {};
		virtual void Update(float) override {};
		virtual void Render() const override {};

		void SetPosition(float x, float y);
		void SetPosition(float x, float y, float z);
		const glm::vec3& GetPosition() const;
		
	private:
		glm::vec3 m_Position{};
		
	};
}

