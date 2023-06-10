#pragma once

#include "BaseComponent.h"
#include "glm/vec2.hpp"

class TankTurretComponent final: public Engine::BaseComponent
{
public:

	TankTurretComponent(Engine::GameObject* pParent):
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;

	void SetTurretDirection(glm::vec2 dir);

private:
	Engine::TransformComponent* m_pTransform{ nullptr };

	const float m_RotationSpeed{ 720.0f };   // Can be quite high but dont want instant snapping
	glm::vec2 m_DesiredDirection{ 0.0f };
};

