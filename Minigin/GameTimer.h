#pragma once
#include "Singleton.h"
namespace Engine
{
	// Keeps track of the time passed since the game started + allows deltaTime to be retrieved
	class GameTimer final: public Singleton<GameTimer>
	{
	public:
		void SetFixedDeltaTime(float fixedDeltaTime) { m_FixedDeltaTime = fixedDeltaTime; }
		void UpdateDeltaTime(float deltaTime);

		float GetFixedDeltaTime() const { return m_FixedDeltaTime; }
		float GetDeltaTime() const { return m_DeltaTime; }
		float GetTotalTime() const { return m_TotalTime; }
	private:
		friend class Singleton<GameTimer>;
		GameTimer() = default;
		virtual ~GameTimer() = default;

		float m_FixedDeltaTime{ 0.02f };

		float m_DeltaTime{};
		float m_TotalTime{};
	};
}

