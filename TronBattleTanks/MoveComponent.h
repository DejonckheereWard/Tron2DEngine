#pragma once
#include "BaseComponent.h"
#include <glm/vec2.hpp>

namespace Engine
{
	class CollisionComponent;
}

class MoveComponent final: public Engine::BaseComponent
{
public:
	MoveComponent(Engine::GameObject* pParent):
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;
	virtual void RenderDebug() const override;


	void Move(const glm::vec2& direction) { m_MoveDirection = direction; }
	bool CanMove(const glm::vec2& direction) const;

private:
	Engine::CollisionComponent* m_pCollisionComponent{};

	glm::vec2 m_PreferedMoveDirection{ 1.0f, 1.0f };  // Based on previous inputs

	glm::vec2 m_MoveDirection{};
	float m_Speed{ 100.f };
	
	static void ClampDirectionToAxis(glm::vec2& direction);

	// Debug rendering
	glm::vec2 m_PreviousMoveDirection{};  // Used for debug rendering
	bool m_RenderDebug{ false };


};

