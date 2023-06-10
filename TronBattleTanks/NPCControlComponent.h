#pragma once
#include "GameObject.h"
#include "BaseComponent.h"

class TankTurretComponent;
class TankGunComponent;
class MoveComponent;

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
private:
	MoveComponent* m_pMoveComponent{};
	TankTurretComponent* m_pTurret{};
	TankGunComponent* m_pGun{};
	Engine::GameObject* m_pTarget{};
	const float m_DetectionRange{ 200.0f };
};

