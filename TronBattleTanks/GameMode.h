#pragma once

#include "BaseComponent.h"
#include "Observer.h"

namespace Engine
{
	class GameObject;
}


enum class GameModeType
{
	SOLO,
	COOP,
	VERSUS
};


class GameMode : public Engine::BaseComponent, public Engine::Observer
{
public:
	GameMode(Engine::GameObject* pParent) :
		BaseComponent(pParent)
	{}

	// Inherited via BaseComponent
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;

	void SetGameModeType(GameModeType gameModeType) { m_GameModeType = gameModeType; }
	GameModeType GetGameModeType() const { return m_GameModeType; }

	void SetPlayerA(Engine::GameObject* pPlayerA) { m_pPlayerA = pPlayerA; }
	void SetPlayerB(Engine::GameObject* pPlayerB) { m_pPlayerB = pPlayerB; }

	Engine::GameObject* GetPlayerA() const { return m_pPlayerA; }
	Engine::GameObject* GetPlayerB() const { return m_pPlayerB; }

	// Inherited via Observer
	virtual void OnNotify(Engine::GameObject* entity, const std::string& eventName) override;




private:
	GameModeType m_GameModeType{ GameModeType::SOLO };

	Engine::GameObject* m_pPlayerA{ nullptr };
	Engine::GameObject* m_pPlayerB{ nullptr };


};

