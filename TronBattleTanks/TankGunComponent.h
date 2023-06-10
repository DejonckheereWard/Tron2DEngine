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

private:
	Engine::TransformComponent* m_pTransform{ nullptr };

	float m_CooldownTime{ 0.5f };  // Time before can shoot again
	float m_CooldownElapsed{ FLT_MAX };  // Time since last shot



};

