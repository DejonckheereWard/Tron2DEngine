#include "LivesDisplay.h"
#include "TextComponent.h"
#include "HealthComponent.h"

LivesDisplay::LivesDisplay(Engine::GameObject* parent):
	Engine::BaseComponent(parent),
	m_pTextComponent{}
{
}

void LivesDisplay::Init()
{
	m_pTextComponent = GetOwner()->GetComponent<Engine::TextComponent>();
	m_pTextComponent->SetText(m_Prefix + "Lives: 3");
}

void LivesDisplay::OnNotify(Engine::GameObject* pGameObject, const std::string& eventName)
{
	if(eventName == "PlayerDeath")
	{
		m_pTextComponent->SetText(m_Prefix + "Lives: " + std::to_string(pGameObject->GetComponent<HealthComponent>()->GetExtraLives()));
	}
}
