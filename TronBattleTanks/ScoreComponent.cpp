#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(Engine::GameObject* parent):
		Engine::BaseComponent(parent)
{
	m_pSubject = std::make_unique<Engine::Subject>();
}

void ScoreComponent::Init()
{
	m_pSubject->Notify(GetOwner(), "ScoreChanged");
}


int ScoreComponent::GetScore() const
{
	return m_Score;
}

void ScoreComponent::SetScore(int score)
{
	m_Score = score;
	m_pSubject->Notify(GetOwner(), "ScoreChanged");
}

void ScoreComponent::AddScore(int score)
{
	m_Score += score;
	m_pSubject->Notify(GetOwner(), "ScoreChanged");
}

void ScoreComponent::SubtractScore(int score)
{
	m_Score -= score;
	m_pSubject->Notify(GetOwner(), "ScoreChanged");
}
