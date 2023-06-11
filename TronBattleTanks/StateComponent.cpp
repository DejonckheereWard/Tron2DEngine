#include "StateComponent.h"

StateComponent::~StateComponent()
{
	if (m_pState)
	{
		delete m_pState;
	}
}

void StateComponent::Init()
{
	if (m_pState)
	{
		m_pState->SetOwner(GetOwner());
		m_pState->OnEnter();
	}
}

void StateComponent::Update()
{
	if (m_pState)
	{
		State* newState{ m_pState->Update() };
		if (newState)
		{
			SetState(newState);
		}

	}
}

void StateComponent::SetState(State* pState)
{
	if (m_pState)
		m_pState->OnExit();
	delete m_pState;

	m_pState = pState;

	if (m_pState)
	{
		m_pState->SetOwner(GetOwner());
		m_pState->OnEnter();
	}
}

void StateComponent::Render() const
{
}
