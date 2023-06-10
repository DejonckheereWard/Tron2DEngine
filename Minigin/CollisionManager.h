#pragma once
#include "Singleton.h"
#include <vector>
#include "glm/vec2.hpp"
#include <limits>


namespace Engine
{
	class CollisionComponent;

	struct HitInfo
	{
		CollisionComponent* pCollider{ nullptr };
		glm::vec2 point{};
		glm::vec2 normal{};
		float distance{};
		bool isHit{ false };
	};

	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void AddCollision(CollisionComponent* component);
		void RemoveCollision(CollisionComponent* component);

		void FixedUpdate();


		bool Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, HitInfo& outHit, uint8_t collisionMask = UINT8_MAX);  // Numeric limits not working due to C4003 error
		bool RaycastSingle(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, Engine::CollisionComponent* pOther, HitInfo& outHit, uint8_t collisionMask = UINT8_MAX);

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;
		~CollisionManager() = default;
		std::vector<CollisionComponent*> m_Colliders{};
		std::vector<CollisionComponent*> m_CollidersToRemove{};

		bool IsColliding(CollisionComponent* pFirst, CollisionComponent* pSecond);
	};
}

