#include "GameMode.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "SceneManager.h"


void GameMode::Init()
{
}

void GameMode::Update()
{
}

void GameMode::Render() const
{
}

void GameMode::ResetGameMode()
{
	m_GameOver = false;
	m_pPlayerA = nullptr;
	m_pPlayerB = nullptr;
	m_CurrentLevel = 0;
}

bool GameMode::DoResetLevel()
{
	return m_ResetLevel;
}

void GameMode::NextLevel()
{
	m_CurrentLevel++;
	if (m_CurrentLevel >= m_LevelPaths.size())
	{
		m_CurrentLevel = 0;
	}
}

void GameMode::OnNotify(Engine::GameObject* entity, const std::string& eventName)
{
	if (eventName == "PlayerDeath")
	{
		if (entity == m_pPlayerA || entity == m_pPlayerB)
		{
			HealthComponent* pHealthComp{ entity->GetComponent<HealthComponent>() };

			if (entity == m_pPlayerA)
			{
				m_PlayerALives = pHealthComp->GetExtraLives();
			}
			else
			{
				m_PlayerBLives = pHealthComp->GetExtraLives();
			}

			if (pHealthComp->GetExtraLives() < 0)
			{
				// No more lives, game over
				m_GameOver = true;
			}
			else
			{
				m_ResetLevel = true;
			}

		}
		else
		{
			entity->MarkForDeletion();

			// Get all the enemies, if all dead, go to next level
			std::vector<Engine::GameObject*> enemies{ Engine::SceneManager::GetInstance().GetChildrenWithTag("EnemyTank") };
			if (enemies.size() == 0)
			{
				enemies = Engine::SceneManager::GetInstance().GetChildrenWithTag("EnemyRecognizer");
				if (enemies.size() == 0)
				{
					NextLevel();
					m_ResetLevel = true;
				}
			}
		}

	}

}