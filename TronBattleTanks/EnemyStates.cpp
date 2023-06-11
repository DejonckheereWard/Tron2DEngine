#include "EnemyStates.h"
#include "NavmeshManager.h"

#include "NPCControlComponent.h"
#include "RenderComponent.h"
#include "NavmeshComponent.h"
#include "NPCControlComponent.h"
#include "TransformComponent.h"

namespace States
{

	/* ------- Roaming State ------- */
	void RoamingState::OnEnter()
	{
		m_pNPCControlComponent = GetOwner()->GetComponent<NPCControlComponent>();
		m_pNavmeshComponent = GetOwner()->GetComponent<NavmeshComponent>();

		if (m_pNavmeshComponent->GetDestination() == glm::vec2{})
		{
			// Only when no initial destination is set, set a random destination, otherwise keep the current destination
			m_pNavmeshComponent->SetDestination(NavmeshManager::GetInstance().GetRandomNode()->Position);
		}

	}

	State* RoamingState::Update()
	{
		Engine::GameObject* pTarget{ m_pNPCControlComponent->LookForTarget() };
		if (pTarget)
		{
			m_pNPCControlComponent->SetTarget(pTarget);
			return new ChasingState();
		}
		else
		{
			if (m_pNavmeshComponent->IsAtDestination())
			{
				m_pNavmeshComponent->SetDestination(NavmeshManager::GetInstance().GetRandomNode()->Position);
			}
		}
		return nullptr;
	}

	void RoamingState::OnExit()
	{
		m_pNPCControlComponent->SetTarget(nullptr);
	}


	/* ------- Chasing State -------  */
	void ChasingState::OnEnter()
	{
		m_pNPCControlComponent = GetOwner()->GetComponent<NPCControlComponent>();
		m_pNavmeshComponent = GetOwner()->GetComponent<NavmeshComponent>();

	}

	State* ChasingState::Update()
	{
		Engine::GameObject* pTarget{ m_pNPCControlComponent->LookForTarget() };
		if (pTarget)
		{
			m_pNPCControlComponent->SetTarget(pTarget);
			m_pNavmeshComponent->SetDestination(pTarget->GetTransform()->GetPosition());
		}
		else
		{
			return new RoamingState();
		}
		return nullptr;
	}

	void ChasingState::OnExit()
	{
	}
}
