#pragma once

namespace Engine
{
	class GameObject;
}

class State
{
public:
	State() = default;
    virtual ~State() = default;

	State(const State& other) = delete;
	State(State&& other) = delete;
	State& operator=(const State& other) = delete;
	State& operator=(State&& other) = delete;

	virtual void OnEnter() = 0;
	virtual State* Update() = 0;
	virtual void OnExit() = 0;

	void SetOwner(Engine::GameObject* pOwner) { m_pOwner = pOwner; }
	Engine::GameObject* GetOwner() const { return m_pOwner; }

private:
	Engine::GameObject* m_pOwner{};

};

