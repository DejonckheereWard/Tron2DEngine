#include "ScoreDisplay.h"
#include "Observer.h"
#include "TextComponent.h"
#include "ScoreComponent.h"

ScoreDisplay::ScoreDisplay(Engine::GameObject* parent):
	Engine::BaseComponent(parent),
	m_pTextComponent{}
{
}

void ScoreDisplay::Init()
{
	m_pTextComponent = GetOwner()->GetComponent<Engine::TextComponent>();
}

void ScoreDisplay::OnNotify(Engine::GameObject* pGameObject, const std::string& eventName)
{
	if(eventName == "ScoreChanged")
	{
		m_pTextComponent->SetText(m_Prefix + std::to_string(pGameObject->GetComponent<ScoreComponent>()->GetScore()));
	}

}
