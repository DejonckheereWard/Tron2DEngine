#pragma once
#include "BaseComponent.h"

namespace Engine
{
	class GameObject;
	class CollisionComponent;
}

class TankTurretComponent;
class TankGunComponent;
class MoveComponent;
class NavmeshComponent;


class NPCControlComponent final: public Engine::BaseComponent
{
public:
	NPCControlComponent(Engine::GameObject* pParent):
		BaseComponent(pParent) {}

	// Inherited via BaseComponent
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;

	void SetTarget(Engine::GameObject* pTarget) { m_pTarget = pTarget; }
	void SetGunComponent(TankGunComponent* pGun) { m_pGun = pGun; }
	void SetTurretComponent(TankTurretComponent* pTurret) { m_pTurret = pTurret;  }

	Engine::GameObject* LookForTarget() const;

private:
	Engine::CollisionComponent* m_pCollisionComponent{};
	MoveComponent* m_pMoveComponent{};
	NavmeshComponent* m_pNavmeshComponent{};
	TankTurretComponent* m_pTurret{};
	TankGunComponent* m_pGun{};
	Engine::GameObject* m_pTarget{};
	const float m_DetectionRange{ 150.0f };
};

