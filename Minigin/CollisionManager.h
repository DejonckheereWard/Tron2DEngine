#pragma once
#include "Singleton.h"
#include <vector>
#include "glm/vec2.hpp"

namespace Engine
{
	class CollisionComponent;

	struct HitInfo
	{
		CollisionComponent* pCollider{ nullptr };
		glm::vec2 point{};
		float distance{};
		bool isHit{ false };
	};

	class CollisionManager final: public Singleton<CollisionManager>
	{
	public:
		void AddCollision(CollisionComponent* component);
		void RemoveCollision(CollisionComponent* component);

		void FixedUpdate();

		bool Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, HitInfo& outHit);
		bool RaycastSingle(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, Engine::CollisionComponent* pOther, HitInfo& outHit);

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;
		~CollisionManager() = default;
		std::vector<CollisionComponent*> m_Colliders{};
		std::vector<CollisionComponent*> m_CollidersToRemove{};

		bool IsColliding(CollisionComponent* pFirst, CollisionComponent* pSecond );
	};
}

