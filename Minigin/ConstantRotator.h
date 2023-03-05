#pragma once
#include "BaseComponent.h"

namespace Engine
{
	class TransformComponent;
	class ConstantRotator: public BaseComponent
	{
	public:
		ConstantRotator(GameObject* parent);
		// Inherited via BaseComponent
		virtual void Init() override;
		virtual void Update(float deltaTime) override;
		virtual void Render() const override {};

	private:
		float m_AnglePerSec{ 45.0f };
		

		TransformComponent* m_pTransform{ nullptr };
	};
}

