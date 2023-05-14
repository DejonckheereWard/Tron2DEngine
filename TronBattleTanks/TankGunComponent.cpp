#include "TankGunComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include <RenderComponent.h>
#include <ResourceManager.h>
#include "BulletComponent.h"

void TankGunComponent::Init()
{
	m_pTransform = GetOwner()->GetComponent<Engine::TransformComponent>();
	m_pTransform->SetLocalPosition(0, 0);
	m_pTransform->SetLocalRotation(0);
}

void TankGunComponent::Update(float /*deltaTime*/)
{

}

void TankGunComponent::Render() const
{
}


void TankGunComponent::Shoot()
{
	using Engine::Scene, Engine::SceneManager, Engine::GameObject;

	Scene* pScene = SceneManager::GetInstance().GetMainScene();

	const float currentGunAngle{ glm::radians(m_pTransform->GetRotation()) };
	const glm::vec2 bulletDirection{ glm::cos(currentGunAngle), glm::sin(currentGunAngle) };
	const glm::vec2 bulletOrigin{ bulletDirection * 20.0f };

	GameObject* pBullet = new GameObject();
	auto* renderComponent = pBullet->AddComponent<Engine::RenderComponent>();
	renderComponent->SetTexture(Engine::ResourceManager::GetInstance().LoadTexture("Sprites/BulletPlayer.png"));
	renderComponent->SetTextureOffset({0.5f, 0.5f});
	pBullet->GetTransform()->SetLocalPosition(m_pTransform->GetPosition() + bulletOrigin);
	pBullet->AddComponent<BulletComponent>()->SetDirection(bulletDirection);
	pScene->AddChild(pBullet);
	
}
