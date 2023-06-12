#pragma once
#include "State.h"
#include <climits>
#include <glm/vec2.hpp>
#include "CollisionComponent.h"

namespace Engine
{
	class GameObject;
	class Scene;
	class TextComponent;
	class Scene;
}

class GameMode;
namespace States
{
	class MainMenuState : public State
	{
	public:
		// Inherited via State
		virtual void OnEnter() override;
		virtual State* Update() override;
		virtual void OnExit() override;

		void SetGameMode(GameMode* pGameMode) { m_pGameMode = pGameMode; }

	private:
		GameMode* m_pGameMode{};
		State* m_pNextState{};

		Engine::TextComponent* m_pGameModeTextComp{};
	};

	class PlayingState : public State
	{
	public:
		// Inherited via State
		virtual void OnEnter() override;
		virtual State* Update() override;
		virtual void OnExit() override;

		void SetGameMode(GameMode* pGameMode) { m_pGameMode = pGameMode; }

	private:
		GameMode* m_pGameMode{};
		bool m_HasSurvived{};
		Engine::Scene* m_pScene{};
		State* m_pNextState{};

		// Helper functions
		void InitLevel();
		Engine::GameObject* SpawnPlayerA(const glm::vec2& pos, unsigned int controllerIdx = UINT_MAX);
		Engine::GameObject* SpawnPlayerB(const glm::vec2& pos, unsigned int controllerIdx = UINT_MAX);
		Engine::GameObject* SpawnEnemyTank(const glm::vec2& pos);
		Engine::GameObject* SpawnEnemyRecognizer(const glm::vec2& pos);
	};

	class GameOverState : public State
	{
	public:
		// Inherited via State
		virtual void OnEnter() override;
		virtual State* Update() override;
		virtual void OnExit() override;

		void SetGameMode(GameMode* pGameMode) { m_pGameMode = pGameMode; }

	private:
		GameMode* m_pGameMode{};
		State* m_pNextState{};
	};

	class HighScoreMenuState : public State
	{
	public:
		// Inherited via State
		virtual void OnEnter() override;
		virtual State* Update() override;
		virtual void OnExit() override;

		void SetGameMode(GameMode* pGameMode) { m_pGameMode = pGameMode; }

	private:
		GameMode* m_pGameMode{};
		State* m_pNextState{};

		std::vector<std::string> GetHighScores();

	};

}

