#pragma once
#include "BaseComponent.h"
#include "glm/vec2.hpp"
#include "CollisionComponent.h"
#include <functional>

class BulletComponent final: public Engine::BaseComponent
{
public:

	BulletComponent(Engine::GameObject* pParent): 
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() const override;

	void SetDirection(const glm::vec2& dir);
	void SetCollisionLayer(uint8_t layer) { m_CollisionLayer = layer; }


private:
	Engine::TransformComponent* m_pTransform{ nullptr };
	uint8_t m_CollisionLayer{ 0 };

	const float m_Speed{ 150.0f };
	glm::vec2 m_Direction{};

	const int m_MaxBounces{ 5 };
	int m_NrOfBounces{ 0 };
	const float m_LifeTime{ 10.0f };
	float m_LifeTimer{ 0.0f };

};

