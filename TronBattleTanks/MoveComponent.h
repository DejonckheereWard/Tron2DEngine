#pragma once
#include "BaseComponent.h"
#include <glm/vec2.hpp>

class MoveComponent final: public Engine::BaseComponent
{
public:
	MoveComponent(Engine::GameObject* pParent):
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	void Move(const glm::vec2& direction) { m_MoveDirection = direction; }

private:
	glm::vec2 m_MoveDirection{};
	float m_Speed{ 100.f };


};

