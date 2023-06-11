#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "CollisionManager.h"
#include "Renderer.h"


Engine::CollisionComponent::CollisionComponent(Engine::GameObject* pParent):
	BaseComponent(pParent)
{
	CollisionManager::GetInstance().AddCollision(this);
}

void Engine::CollisionComponent::Init()
{
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

void Engine::CollisionComponent::RenderDebug() const
{
	if(m_DrawDebug)
		Renderer::GetInstance().RenderRect(glm::vec4(GetColliderPosition(), m_ColliderSize));
}

glm::vec2 Engine::CollisionComponent::GetColliderPosition() const
{
	return GetTransform()->GetPosition() + m_ColliderOffset;
}

glm::vec2 Engine::CollisionComponent::GetColliderCenter() const
{
	return GetColliderPosition() + m_ColliderSize * 0.5f;
}

void Engine::CollisionComponent::OnCollision(CollisionComponent* pOther)
{
	if (m_OnCollision)
		m_OnCollision(pOther->GetOwner());
}
