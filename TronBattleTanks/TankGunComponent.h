#pragma once

#include "BaseComponent.h"
#include "glm/vec2.hpp"

class TankGunComponent final: public Engine::BaseComponent
{
public:
	TankGunComponent(Engine::GameObject* pParent): 
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	void Shoot();

private:
	Engine::TransformComponent* m_pTransform{ nullptr };

};

