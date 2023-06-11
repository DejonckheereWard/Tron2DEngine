#pragma once
#include "State.h"

class NavmeshComponent;
class NPCControlComponent;

namespace States
{
	/* ------- RoamingState ------- */
	class RoamingState final : public State
	{
	public:
		RoamingState() = default;
		virtual ~RoamingState() = default;

		RoamingState(const RoamingState& other) = delete;
		RoamingState(RoamingState&& other) = delete;
		RoamingState& operator=(const RoamingState& other) = delete;
		RoamingState& operator=(RoamingState&& other) = delete;

		virtual void OnEnter() override;
		virtual State* Update() override;
		virtual void OnExit() override;

	private:
		NavmeshComponent* m_pNavmeshComponent{};
		NPCControlComponent* m_pNPCControlComponent{};



	};

	/* ------- ChasingState ------- */
	class ChasingState final : public State
	{
	public:
		ChasingState() = default;
		virtual ~ChasingState() = default;

		ChasingState(const ChasingState& other) = delete;
		ChasingState(ChasingState&& other) = delete;
		ChasingState& operator=(const ChasingState& other) = delete;
		ChasingState& operator=(ChasingState&& other) = delete;

		virtual void OnEnter() override;
		virtual State* Update() override;
		virtual void OnExit() override;

	private:
		NavmeshComponent* m_pNavmeshComponent{};
		NPCControlComponent* m_pNPCControlComponent{};
	};
}

