#include "CollisionManager.h"
#include "BulletComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"

void BulletComponent::Init()
{
	m_pTransform = GetOwner()->GetTransform();
}

void BulletComponent::Update()
{
	const float deltaTime{ GameTimer::GetInstance().GetDeltaTime() };
	m_LifeTimer += deltaTime;
	if(m_LifeTimer >= m_LifeTime || m_NrOfBounces == m_MaxBounces)
	{
		GetOwner()->MarkForDeletion();
		return;
	}


}

void BulletComponent::FixedUpdate()
{
	uint8_t collisionMask{ std::numeric_limits<uint8_t>::max() };
	collisionMask &= ~m_CollisionLayer;  // Dont collide with given layer

	const float fixedDelta{ GameTimer::GetInstance().GetFixedDeltaTime() };
	const glm::vec2& currentPos{ m_pTransform->GetPosition() };

	Engine::HitInfo hitInfo{};
	if (Engine::CollisionManager::GetInstance().Raycast(currentPos, m_Direction, m_Speed * fixedDelta, hitInfo, collisionMask))
	{
		// Reflect the bullet using the normal of the hit

		const glm::vec2 reflectedDir{ glm::reflect(m_Direction, hitInfo.normal) };
		SetDirection(reflectedDir);

		hitInfo.pCollider->OnCollision(GetOwner());

		assert(hitInfo.pCollider != nullptr);
		Engine::CollisionLayer otherLayer{ hitInfo.pCollider->GetLayer() };
		if (otherLayer != Engine::CollisionLayer::World)
		{
			GetOwner()->MarkForDeletion();
		}

		++m_NrOfBounces;
	}

	const glm::vec2 newPos{ currentPos + m_Direction * m_Speed * fixedDelta };
	m_pTransform->SetLocalPosition(newPos);
}

void BulletComponent::Render() const
{
}

void BulletComponent::SetDirection(const glm::vec2& dir)
{
	assert(glm::length(dir) > 0.0f && "Direction vector cannot be zero length!");
	assert(std::roundf(glm::length(dir)) == 1.0f && "Direction vector needs to be normalized!");
	m_Direction = dir;
}
