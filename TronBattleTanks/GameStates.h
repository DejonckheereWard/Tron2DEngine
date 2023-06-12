#pragma once
#include "State.h"
#include "TextComponent.h"

class GameMode;
namespace States
{
	class MainMenuState: public State
	{
	public:
		// Inherited via State
		virtual void OnEnter() override;
		virtual State* Update() override;
		virtual void OnExit() override;

		void SetGameMode(GameMode* pGameMode) { m_pGameMode = pGameMode; }

	private:
		GameMode* m_pGameMode{};

		Engine::TextComponent* m_pGameModeTextComp{};
	};


	class HighScoreMenuState : public State
	{
	public:
		// Inherited via State
		virtual void OnEnter() override;
		virtual State* Update() override;
		virtual void OnExit() override;

	private:
		GameMode* m_pGameMode{};

	};

}

