#pragma once

#include "BaseComponent.h"
#include "State.h"

class StateComponent final: public Engine::BaseComponent
{
public:
	StateComponent(Engine::GameObject* parent, State* initState) :
		BaseComponent(parent),
		m_pState(initState)
	{}
	~StateComponent();

	// Inherited via BaseComponent
	virtual void Init() override;
	virtual void Render() const override;

	virtual void Update() override;

	void SetState(State* pState);
	State* GetState() const { return m_pState; }


private:
	State* m_pState{ nullptr };

};

