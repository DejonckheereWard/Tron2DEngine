#include "FPSComponent.h"
#include <numeric>

dae::FPSComponent::FPSComponent():
	m_FPS{0},
	m_DeltaTimeSamples{}
{
	m_DeltaTimeSamples.reserve(10);
}

void dae::FPSComponent::Update(GameObject&, float deltaTime)
{
	if(m_DeltaTimeSamples.size() >= 10)
	{
		m_DeltaTimeSamples.erase(m_DeltaTimeSamples.begin()); // Delete oldest time
	}
	m_DeltaTimeSamples.push_back(deltaTime);
	const float totalTime{ std::accumulate(begin(m_DeltaTimeSamples), end(m_DeltaTimeSamples), 0.f)};
	const float averageTime{ totalTime / m_DeltaTimeSamples.size() };
	
	m_FPS = static_cast<int>(1.0f / averageTime);  // Rounding or not doesnt really matter
}
