#include "TankTurretComponent.h"
#include "TransformComponent.h"
#include <cassert>

void TankTurretComponent::Init()
{
	m_pTransform = GetOwner()->GetTransform();
}

void TankTurretComponent::Update()
{
	const float deltaTime{ GameTimer::GetInstance().GetDeltaTime() };
	const float maxAngleOffset{ 1.0f };
	const float currentGunAngle{ m_pTransform->GetLocalRotation() };

	const float desiredAngle{ glm::degrees(std::atan2(m_DesiredDirection.y, m_DesiredDirection.x)) };
	const float angleBetween = fmod((desiredAngle - currentGunAngle + 540.0f), 360.0f) - 180.0f;

	if(angleBetween > maxAngleOffset)
	{
		m_pTransform->SetLocalRotation(currentGunAngle + m_RotationSpeed * deltaTime);
	}
	else if(angleBetween < -maxAngleOffset)
	{
		m_pTransform->SetLocalRotation(currentGunAngle - m_RotationSpeed * deltaTime);
	}
}

void TankTurretComponent::Render() const
{
}

void TankTurretComponent::SetTurretDirection(const glm::vec2& dir)
{
	assert(glm::length(dir) > 0.0f && "Direction vector cannot be zero length!");
	assert(std::roundf(glm::length(dir)) == 1.0f && "Direction vector needs to be normalized!");

	m_DesiredDirection = dir;
}

