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
		// Since A collide with B is not the same as B colliding with A we check all pairs regardless if they have been checked before
		// A could ignore B but B could still collide with A
		for (size_t j{}; j < m_Colliders.size(); ++j)
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

bool Engine::CollisionManager::IsPointInCollider(const glm::vec2& point, uint8_t collisionMask) const
{
	// Check for collisions on every collider
	for (CollisionComponent* pCollider : m_Colliders)
	{
		// Check collision mask
		if ((collisionMask & pCollider->GetLayer()) == 0)
					continue;

		const glm::vec2& size{ pCollider->GetColliderSize() };
		const glm::vec2& pos{ pCollider->GetColliderPosition() };

		// Check if the point is inside the collider
		if (point.x > pos.x && 
			point.x < pos.x + size.x &&
			point.y > pos.y && 
			point.y < pos.y + size.y)
		{
			return true;
		}
	}
	return false;
}


bool Engine::CollisionManager::Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, HitInfo& outHit, uint8_t collisionMask) const
{
	outHit.distance = maxDistance;
	// Check for collisions on every collider
	for (CollisionComponent* pCollider : m_Colliders)
	{
		RaycastSingle(origin, direction, outHit.distance, pCollider, outHit, collisionMask);
	}
	return outHit.isHit;
}

bool Engine::CollisionManager::RaycastSingle(const glm::vec2& origin, const glm::vec2& direction, float maxDistance, Engine::CollisionComponent* pOther, HitInfo& outHit, uint8_t collisionMask) const
{
	// Check collision mask
	if ((collisionMask & pOther->GetLayer()) == 0)
		return false;

	const glm::vec2& size{ pOther->GetColliderSize() };
	const glm::vec2& pos{ pOther->GetColliderPosition() };

	const float tx1{ (pos.x - origin.x) / direction.x };
	const float tx2{ (pos.x + size.x - origin.x) / direction.x };

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	const float ty1{ (pos.y - origin.y) / direction.y };
	const float ty2{ (pos.y + size.y - origin.y) / direction.y };

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	if (not (tmax > 0 && tmax >= tmin)) {
		return false;
	}

	if (tmin > maxDistance) {
		return false;
	}	

	// Hit has been found
	// Calculate normal

	if (tmin == tx1) {
		outHit.normal = { -1, 0 };
	}
	else if (tmin == tx2) {
		outHit.normal = { 1, 0 };
	}
	else if (tmin == ty1) {
		outHit.normal = { 0, -1 };
	}
	else if (tmin == ty2) {
		outHit.normal = { 0, 1 };
	}

	
	// Fill in the HitInfo
	outHit.isHit = true;
	outHit.distance = tmin;
	outHit.point = origin + direction * tmin;
	outHit.pCollider = pOther;


	return true;


}

bool Engine::CollisionManager::IsColliding(CollisionComponent* pFirst, CollisionComponent* pSecond)
{
	// Check collisions mask with second layer
	if ((pFirst->GetCollisionMask() & pSecond->GetLayer()) == 0)
		return false;
	
	
	const glm::vec2& firstSize{ pFirst->GetColliderSize() };
	const glm::vec2& firstPos{ pFirst->GetColliderPosition() };

	const glm::vec2& secondSize{ pSecond->GetColliderSize() };
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