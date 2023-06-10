#include "GameTimer.h"

void Engine::GameTimer::UpdateDeltaTime(float deltaTime)
{
	m_TotalTime += deltaTime;
	m_DeltaTime = deltaTime;
}
