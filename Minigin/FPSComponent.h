#pragma once
#include "BaseComponent.h"
#include <vector>

namespace Engine
{	
	class TextComponent;
	class FPSComponent final: public BaseComponent
	{
	public:
		FPSComponent(GameObject* pOwner);
		virtual void Init() override;
		virtual void Update() override;	
		virtual void Render() const override {};

	private:
		const int m_NrOfSamples{ 100 };
		std::vector<float> m_DeltaTimeSamples{};
		int m_Fps{ 0 };

		TextComponent* m_pTextComponent{ nullptr };

	};
}


