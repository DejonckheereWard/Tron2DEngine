#pragma once
#include "BaseComponent.h"

class MoveComponent;
class RotateWithMovementComponent final : public Engine::BaseComponent
{

public:
	RotateWithMovementComponent(Engine::GameObject* pParent) :
		BaseComponent(pParent) {}


	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override {};

	void SetMovementComponent(MoveComponent* pMoveComponent) { m_pMoveComponent = pMoveComponent; }

private:
	MoveComponent* m_pMoveComponent{ nullptr };
};

