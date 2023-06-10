#include "TankTurretComponent.h"
#include "TransformComponent.h"
#include <cassert>
#include <algorithm>

void TankTurretComponent::Init()
{
	m_pTransform = GetOwner()->GetTransform();
}

void TankTurretComponent::Update()
{
	const float deltaTime{ GameTimer::GetInstance().GetDeltaTime() };
	const float maxAngleOffset{ 0.5f };
	const float currentGunAngle{ m_pTransform->GetLocalRotation() };

	const float desiredAngle{ glm::degrees(std::atan2(m_DesiredDirection.y, m_DesiredDirection.x)) };
	const float angleBetween = fmod((desiredAngle - currentGunAngle + 540.0f), 360.0f) - 180.0f;

	const float multiplier = std::clamp(std::abs(angleBetween / 10.0f), 0.f, 1.0f);
	if(angleBetween > maxAngleOffset)
	{
		m_pTransform->SetLocalRotation(currentGunAngle + m_RotationSpeed * deltaTime * multiplier);
	}
	else if(angleBetween < -maxAngleOffset)
	{
		m_pTransform->SetLocalRotation(currentGunAngle - m_RotationSpeed * deltaTime * multiplier);
	}
}

void TankTurretComponent::Render() const
{
}

void TankTurretComponent::SetTurretDirection(glm::vec2 dir)
{
	assert(glm::length(dir) > 0.0f && "Direction vector cannot be zero length!");
	assert(std::roundf(glm::length(dir)) == 1.0f && "Direction vector should be normalized!");

	if (abs(dir.x) > abs(dir.y))
	{
		dir.y = 0;
		dir.x = (1.0f / dir.x) * abs(dir.x);  // Eh? it works ("normalizes" the value)
	}
	else
	{
		dir.x = 0;
		dir.y = (1.0f / dir.y) * abs(dir.y);
	}

	m_DesiredDirection = dir;
}

