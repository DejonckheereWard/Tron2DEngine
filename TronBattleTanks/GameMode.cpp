#include "GameMode.h"
#include "GameObject.h"


void GameMode::Init()
{
}

void GameMode::Update()
{
}

void GameMode::Render() const
{
}

void GameMode::OnNotify(Engine::GameObject* entity, const std::string& eventName)
{
	if (eventName == "PlayerDeath")
	{
		if (entity == m_pPlayerA || entity == m_pPlayerB)
		{
			// Respawn the entity
			// If out of lives, move far away
			// If not, respawn at spawn point
			// if all players are out of lives, end the game

			//m_pPlayerA->GetComponent<HealthComponent>()-
		}
		else
		{
			entity->MarkForDeletion();
		}

	}

}