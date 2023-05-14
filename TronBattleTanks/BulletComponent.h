#pragma once
#include "BaseComponent.h"
#include "glm/vec2.hpp"

class BulletComponent final: public Engine::BaseComponent
{
public:
	BulletComponent(Engine::GameObject* pParent): 
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	void SetDirection(const glm::vec2& dir);

private:
	Engine::TransformComponent* m_pTransform{ nullptr };
	const float m_Speed{ 200.0f };
	glm::vec2 m_Direction{};

	const float m_LifeTime{ 10.0f };
	float m_LifeTimer{ 0.0f };
};

