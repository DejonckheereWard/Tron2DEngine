#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "CollisionManager.h"


void Engine::CollisionComponent::Init()
{
	CollisionManager::GetInstance().AddCollision(this);
}

void Engine::CollisionComponent::Update()
{
}

void Engine::CollisionComponent::FixedUpdate()
{
}

void Engine::CollisionComponent::Render() const
{
}

glm::vec2 Engine::CollisionComponent::GetColliderPosition()
{
	return GetTransform()->GetPosition() + m_PositionOffset;
}

void Engine::CollisionComponent::OnCollision(CollisionComponent* pOther)
{
	if (m_OnCollision)
		m_OnCollision(pOther->GetOwner());
}
