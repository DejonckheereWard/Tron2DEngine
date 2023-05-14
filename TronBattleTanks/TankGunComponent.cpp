#include "TankGunComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"

void TankGunComponent::Init()
{
	m_pTransform = GetOwner()->GetComponent<Engine::TransformComponent>();
	m_pTransform->SetLocalPosition(0, 0);
	m_pTransform->SetLocalRotation(0);
}

void TankGunComponent::Update(float deltaTime)
{
	const float maxAngleOffset{ 1.0f };
	const float currentGunAngle{ m_pTransform->GetLocalRotation() };
	const float angleDifference{  fmod(m_DesiredGunAngle - currentGunAngle, 180.0f) };

	if(angleDifference > maxAngleOffset)
	{
		m_pTransform->SetLocalRotation(currentGunAngle + m_GunRotationSpeed * deltaTime);
	}
	else if(angleDifference < (-maxAngleOffset))
	{
		m_pTransform->SetLocalRotation(currentGunAngle - m_GunRotationSpeed * deltaTime);
	}
}

void TankGunComponent::Render() const
{
}

void TankGunComponent::SetGunAngle(float angle)
{
	m_DesiredGunAngle = angle;
}

void TankGunComponent::Shoot()
{
	using Engine::Scene, Engine::SceneManager, Engine::GameObject;

	Scene* pScene = SceneManager::GetInstance().GetMainScene();

	GameObject* pBullet = new GameObject();
	pScene->AddChild(pBullet);
	
}
