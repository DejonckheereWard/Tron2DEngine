#pragma once

#include "BaseComponent.h"
#include "glm/vec2.hpp"

class TankGunComponent final: public Engine::BaseComponent
{
public:
	TankGunComponent(Engine::GameObject* pParent): 
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;

	void Shoot();

	// Set what bullet can hit
	void SetBulletCollisionLayer(uint8_t layer) { m_BulletCollisionLayer = layer; }

private:
	Engine::TransformComponent* m_pTransform{ nullptr };
	uint8_t m_BulletCollisionLayer{ 0 };

	float m_CooldownTime{ 0.5f };  // Time before can shoot again
	float m_CooldownElapsed{ FLT_MAX };  // Time since last shot



};

