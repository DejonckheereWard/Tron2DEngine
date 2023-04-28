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


		void SetRotationSpeed(float anglePerSecond) { m_AnglePerSec = anglePerSecond; };

	private:
		//float m_Radius{};  // Set at start and stays at this radius constantly.
		
		float m_AnglePerSec{ 45.0f };
		

		TransformComponent* m_pTransform{ nullptr };
	};
}

