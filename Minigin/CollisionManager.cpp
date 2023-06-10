#include "CollisionManager.h"
#include "CollisionComponent.h"
#include <glm/glm.hpp>


void Engine::CollisionManager::AddCollision(CollisionComponent* component)
{
	m_Colliders.emplace_back(component);
}

void Engine::CollisionManager::RemoveCollision(CollisionComponent* component)
{
	m_CollidersToRemove.push_back(component);
}

void Engine::CollisionManager::FixedUpdate()
{
	// Check for collisions on every pair of colliders
	for (size_t i{}; i < m_Colliders.size(); ++i)
	{
		// j = i + 1 to prevent checking the same collision twice, since previous collisions are already checked
		for (size_t j{ i + 1 }; j < m_Colliders.size(); ++j)
		{
			if (IsColliding(m_Colliders[i], m_Colliders[j]))
			{
				m_Colliders[i]->OnCollision(m_Colliders[j]);
				m_Colliders[j]->OnCollision(m_Colliders[i]);
			}
		}
	}

	// Remove all colliders that are marked for removal
	for (CollisionComponent* pCollider : m_CollidersToRemove)
	{
		std::erase_if(m_Colliders, [pCollider](CollisionComponent* pOther) { return pCollider == pOther; });
	}
	m_CollidersToRemove.clear();

}

bool Engine::CollisionManager::Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, HitInfo& outHit)
{
	outHit.distance = maxDistance;
	// Check for collisions on every collider
	for (CollisionComponent* pCollider : m_Colliders)
	{
		RaycastSingle(origin, direction, maxDistance, pCollider, outHit);
	}
	return outHit.isHit;
}

bool Engine::CollisionManager::RaycastSingle(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, Engine::CollisionComponent* pOther, HitInfo& outHit)
{
	const glm::vec2& size{ pOther->GetSize() };
	const glm::vec2& pos{ pOther->GetColliderPosition() };

	float txmin = (pos.x - origin.x) / direction.x;
	float txmax = (pos.x + size.x - origin.x) / direction.x;

	if (txmin > txmax) {
		std::swap(txmin, txmax);
	}

	float tymin = (pos.y - origin.y) / direction.y;
	float tymax = (pos.y + size.y - origin.y) / direction.y;

	if (tymin > tymax) {
		std::swap(tymin, tymax);
	}

	if ((txmin > tymax) || (tymin > txmax)) {
		return false;
	}

	if (tymin > txmin) {
		txmin = tymin;
	}

	if (tymax < txmax) {
		txmax = tymax;
	}

	if (txmin < 0) {
		txmin = 0;
	}

	float distance{ txmin };

	if (distance > maxDistance) {
		return false;
	}

	outHit.isHit = true;
	outHit.distance = distance;
	outHit.point = origin + direction * distance;
	outHit.pCollider = pOther;

	return true;


}

bool Engine::CollisionManager::IsColliding(CollisionComponent* pFirst, CollisionComponent* pSecond)
{
	const glm::vec2& firstSize{ pFirst->GetSize() };
	const glm::vec2& firstPos{ pFirst->GetColliderPosition() };

	const glm::vec2& secondSize{ pSecond->GetSize() };
	const glm::vec2& secondPos{ pSecond->GetColliderPosition() };

	// Check if the two rectangles are colliding
	if (firstPos.x < secondPos.x + secondSize.x &&
		firstPos.x + firstSize.x > secondPos.x &&
		firstPos.y < secondPos.y + secondSize.y &&
		firstPos.y + firstSize.y > secondPos.y)
	{
		return true;
	}

	return false;
}