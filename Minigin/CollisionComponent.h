#pragma once
#include "BaseComponent.h"
#include "glm/vec2.hpp"

namespace Engine
{
	class CollisionComponent final: public BaseComponent
	{
	public:
		typedef void(*CollisionCallback)(GameObject* pOther);

		CollisionComponent(Engine::GameObject* pParent) :
			BaseComponent(pParent) {}


		virtual ~CollisionComponent() = default;

		// Inherited via BaseComponent
		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() const override;

		glm::vec2 GetColliderPosition();
		void SetSize(const glm::vec2& size) { m_Size = size; }
		const glm::vec2& GetSize() { return m_Size; }

		void OnCollision(CollisionComponent* pOther);

	private:
		glm::vec2 m_Size{ 2.0f, 2.0f}; // Collider size
		glm::vec2 m_PositionOffset{}; // Offset from the transform position (to position colider)

		CollisionCallback m_OnCollision{ nullptr };  // Callback function for when a collision happens
	};
}

