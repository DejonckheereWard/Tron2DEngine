#include "BulletComponent.h"
#include "TransformComponent.h"

void BulletComponent::Init()
{
	m_pTransform = GetOwner()->GetTransform();
}

void BulletComponent::Update()
{
	const float deltaTime{ GameTimer::GetInstance().GetDeltaTime() };
	m_LifeTimer += deltaTime;
	if(m_LifeTimer >= m_LifeTime)
	{
		//GetOwner()->Destroy();
		return;
	}

	glm::vec2 currentPos{ m_pTransform->GetLocalPosition() };
	currentPos += m_Direction * m_Speed * deltaTime;
	m_pTransform->SetLocalPosition(currentPos);
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
