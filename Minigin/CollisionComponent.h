#pragma once
#include "BaseComponent.h"
#include "glm/vec2.hpp"

namespace Engine
{
	enum CollisionLayer: uint8_t  // 8 bits, not class for easier conversion to bitmask
	{
		World = 1 << 0,
		Player = 1 << 1,
		Friendly = 1 << 2,
		Enemy = 1 << 3,
		CustomLayer0 = 1 << 4,
		CustomLayer1 = 1 << 5,
		CustomLayer2 = 1 << 6,
		CustomLayer3 = 1 << 7
	};


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
		virtual void RenderDebug() const override;

		void SetCollisionCallback(CollisionCallback callback) { m_OnCollision = callback; }

		void SetColliderPosition(const glm::vec2& localPosition) { m_Position = localPosition; }
		glm::vec2 GetColliderPosition() const;

		void SetSize(const glm::vec2& size) { m_Size = size; }
		const glm::vec2& GetSize() { return m_Size; }

		void OnCollision(CollisionComponent* pOther);

		void SetDebugDraw(bool draw) { m_DrawDebug = draw; }

		void SetLayer(CollisionLayer layer) { m_Layer = layer; }
		CollisionLayer GetLayer() const { return m_Layer; }

		void SetCollisionMask(uint8_t mask) { m_CollisionMask = mask; }
		void AddCollisionMask(CollisionLayer layer) { m_CollisionMask |= layer; }
		void RemoveCollisionMask(CollisionLayer layer) { m_CollisionMask &= ~layer; }
		uint8_t GetCollisionMask() const { return m_CollisionMask; }


	private:
		bool m_DrawDebug{ true };

		CollisionLayer m_Layer{ CollisionLayer::World }; // Layer of the collider
		uint8_t m_CollisionMask{ std::numeric_limits<uint8_t>::max() }; // Mask of the collider, default to collide with everything
		

		glm::vec2 m_Size{ 2.0f, 2.0f}; // Collider size
		glm::vec2 m_Position{}; // Offset from the transform position (to position colider)

		CollisionCallback m_OnCollision{ nullptr };  // Callback function for when a collision happens
	};
}

