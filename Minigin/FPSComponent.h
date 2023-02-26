#pragma once
#include "BaseComponent.h"
#include <vector>

namespace dae
{	
	class FPSComponent final: public BaseComponent
	{
		// Inherited via BaseComponent
	public:
		FPSComponent();
		virtual void Update(GameObject& gameObject, float deltaTime) override;
		

	public:
		// Public because i want to be able to access this from outside?
		int m_FPS;

	private:
		std::vector<float> m_DeltaTimeSamples;
	};
}


