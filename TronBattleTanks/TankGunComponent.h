#pragma once

#include "BaseComponent.h"

class TankGunComponent final: public Engine::BaseComponent
{
public:
	TankGunComponent(Engine::GameObject* pParent): 
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	void SetGunAngle(float angle);
	void Shoot();

private:
	Engine::TransformComponent* m_pTransform{ nullptr };

	const float m_GunRotationSpeed{ 180.0f };   // Can be quite high but dont want instant snapping
	float m_DesiredGunAngle{ 0.0f };

};

