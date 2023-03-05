#include "FPSComponent.h"
#include "TextComponent.h"
#include <numeric>

dae::FPSComponent::FPSComponent(GameObject* pOwner):
	BaseComponent(pOwner)
{
	m_DeltaTimeSamples.reserve(m_NrOfSamples);
}

void dae::FPSComponent::Init()
{
	// Runs after construction, so then all addcomponents should be added already
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
	m_pTextComponent->SetText("0");
}

void dae::FPSComponent::Update(float deltaTime)
{
	if(m_DeltaTimeSamples.size() >= m_NrOfSamples)
	{
		m_DeltaTimeSamples.erase(m_DeltaTimeSamples.begin()); // Delete oldest time
	}
	m_DeltaTimeSamples.push_back(deltaTime);
	const float totalTime{ std::accumulate(begin(m_DeltaTimeSamples), end(m_DeltaTimeSamples), 0.f)};
	const float averageTime{ totalTime / m_DeltaTimeSamples.size() };
	
	m_Fps = static_cast<int>(1.0f / averageTime);  // Rounding or not doesnt really matter
	m_pTextComponent->SetText(std::to_string(m_Fps));
}
