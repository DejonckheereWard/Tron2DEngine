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
	virtual void FixedUpdate() override;
	virtual void Render() const override;
	virtual void RenderDebug() const override;


	void Move(const glm::vec2& direction) { m_MoveDirection = direction; }
	bool CanMove(const glm::vec2& direction) const;

	void SetSpeed(float speed) { m_Speed = speed; }

	const glm::vec2& GetPreviousMoveDirection() { return m_PreviousMoveDirection; }

	static void ClampDirectionToAxis(glm::vec2& direction);  // Not private since it can be useful while working with movements

private:
	Engine::CollisionComponent* m_pCollisionComponent{};

	glm::vec2 m_PreferedMoveDirection{ 1.0f, 1.0f };  // Based on previous inputs

	glm::vec2 m_MoveDirection{};
	float m_Speed{ 70.f };
	

	// Debug rendering
	glm::vec2 m_PreviousMoveDirection{};  // Used for debug rendering
	bool m_RenderDebug{ true };


};

