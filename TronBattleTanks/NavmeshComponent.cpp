#include "NavmeshComponent.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "NavmeshManager.h"
#include "GameTimer.h"
#include "Renderer.h"
#include "TransformComponent.h"

void NavmeshComponent::Init()
{
	m_pNavmeshManager = &NavmeshManager::GetInstance();
}

void NavmeshComponent::Update()
{
	const float deltaTime = GameTimer::GetInstance().GetDeltaTime();
	m_UpdateTimer += deltaTime;

	if (m_UpdateTimer >= m_UpdateInterval)
	{
		m_UpdateTimer = 0.f;
		m_Path = m_pNavmeshManager->GetPath(m_Position, m_Destination);
		std::reverse(begin(m_Path), end(m_Path));
	}

}

void NavmeshComponent::Render() const
{
}

void NavmeshComponent::RenderDebug() const
{
	const SDL_Color color = { 255, 0, 0, 255 };

	// Draw path
	if (not m_Path.empty())
	{
		for (size_t i = 0; i < m_Path.size() - 1; ++i)
		{
			const glm::vec2& point1 = m_Path[i];
			const glm::vec2& point2 = m_Path[i + 1];
			Engine::Renderer::GetInstance().RenderLine(point1, point2, color);
		}
	}
}

glm::vec2 NavmeshComponent::GetNextInPath()
{
	const float distanceThreshold{ 8.0f };
	if (not m_Path.empty())
	{
		if(glm::distance(m_Position, m_Path.back()) < distanceThreshold)
		{
			m_Path.pop_back();
		}


		if (not m_Path.empty())
		{
			return m_Path.back();
		}
		else
		{
			return m_Destination;
		}

	}
	return GetTransform()->GetPosition();

}

void NavmeshComponent::SetDestination(const glm::vec2& destination)
{
	m_Destination = destination;
}

bool NavmeshComponent::IsAtDestination() const
{
	const float distanceThreshold{ 16.0f };
	return glm::distance(m_Position, m_Destination) < distanceThreshold;
}
