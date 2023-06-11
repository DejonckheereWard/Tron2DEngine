#pragma once
#include "BaseComponent.h"
#include "glm/vec2.hpp"
#include <functional>

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
		typedef std::function<void(GameObject* otherGameObject)> OnCollisionCallback;

		CollisionComponent(Engine::GameObject* pParent);
		virtual ~CollisionComponent() = default;

		// Inherited via BaseComponent
		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() const override;
		virtual void RenderDebug() const override;

		void SetCollisionCallback(OnCollisionCallback callback) { m_OnCollision = callback; }

		void SetColliderOffset(const glm::vec2& offset) { m_ColliderOffset = offset; }
		const glm::vec2& GetColliderOffset() const { return m_ColliderOffset; }

		glm::vec2 GetColliderPosition() const;
		glm::vec2 GetColliderCenter() const;

		void SetColliderSize(const glm::vec2& size) { m_ColliderSize = size; }
		const glm::vec2& GetColliderSize() { return m_ColliderSize; }

		void OnCollision(CollisionComponent* pOther);
		void OnCollision(GameObject* pOther);

		void SetDebugDraw(bool draw) { m_DrawDebug = draw; }

		void SetLayer(CollisionLayer layer) { m_Layer = layer; }
		CollisionLayer GetLayer() const { return m_Layer; }

		void SetCollisionMask(uint8_t mask) { m_CollisionMask = mask; }
		void AddCollisionMask(CollisionLayer layer) { m_CollisionMask |= layer; }
		void RemoveCollisionMask(CollisionLayer layer) { m_CollisionMask &= ~layer; }
		uint8_t GetCollisionMask() const { return m_CollisionMask; }


	private:
		bool m_DrawDebug{ false };

		CollisionLayer m_Layer{ CollisionLayer::World }; // Layer of the collider
		uint8_t m_CollisionMask{ std::numeric_limits<uint8_t>::max() }; // Mask of the collider, default to collide with everything
		

		glm::vec2 m_ColliderSize{ 2.0f, 2.0f}; // Collider size
		glm::vec2 m_ColliderOffset{}; // Offset from the transform position (to position colider)

		OnCollisionCallback m_OnCollision{ nullptr };  // Callback function for when a collision happens
	};
}

