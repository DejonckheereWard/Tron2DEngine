#include "TankGunComponent.h"

#include "ServiceLocator.h"
#include "SceneManager.h"

#include "TransformComponent.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "BulletComponent.h"

void TankGunComponent::Init()
{
	m_pTransform = GetOwner()->GetComponent<Engine::TransformComponent>();
	m_pTransform->SetLocalPosition(0, 0);
	m_pTransform->SetLocalRotation(0);
}

void TankGunComponent::Update()
{
	const float deltaTime{ GameTimer::GetInstance().GetDeltaTime() };
	if (m_CooldownElapsed < m_CooldownTime)
	{
		m_CooldownElapsed += deltaTime;
	}


}

void TankGunComponent::Render() const
{
}


void TankGunComponent::Shoot()
{
	if (m_CooldownElapsed < m_CooldownTime)
		return;
	
	m_CooldownElapsed = 0;

	using Engine::Scene, Engine::SceneManager, Engine::GameObject;

	Scene* pScene = SceneManager::GetInstance().GetMainScene();

	const float currentGunAngle{ glm::radians(m_pTransform->GetRotation()) };
	const glm::vec2 bulletDirection{ glm::cos(currentGunAngle), glm::sin(currentGunAngle) };

	GameObject* pBullet{ new GameObject() };
	Engine::RenderComponent* pRenderComponent{ pBullet->AddComponent<Engine::RenderComponent>() };
	pRenderComponent->SetTexture(m_pBulletTexture);
	pRenderComponent->SetTextureOffset({0.5f, 0.5f});
	pBullet->GetTransform()->SetLocalPosition(m_pTransform->GetPosition());
	pBullet->GetTransform()->SetLocalScale(0.7f, 0.7f);
	pBullet->SetTag("Bullet");

	BulletComponent* pBulletComponent{ pBullet->AddComponent<BulletComponent>() };
	pBulletComponent->SetDirection(bulletDirection);
	pBulletComponent->SetCollisionLayer(m_BulletCollisionLayer);
	pScene->AddChild(pBullet);

	// Play sound
	Engine::ServiceLocator::GetAudioService().PlayEffect("Sounds/Shoot.mp3", 0.05f);
	
}
