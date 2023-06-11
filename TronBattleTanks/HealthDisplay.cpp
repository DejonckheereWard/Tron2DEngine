#include "HealthDisplay.h"
#include "TextComponent.h"
#include "HealthComponent.h"

HealthDisplay::HealthDisplay(Engine::GameObject* parent):
	Engine::BaseComponent(parent),
	m_pTextComponent{}
{}

void HealthDisplay::Init()
{
	m_pTextComponent = GetOwner()->GetComponent<Engine::TextComponent>();
}

void HealthDisplay::OnNotify(Engine::GameObject* pGameObject, const std::string& eventName)
{
	if(eventName == "HealthChanged")
	{
		m_pTextComponent->SetText(m_Prefix + std::to_string(pGameObject->GetComponent<HealthComponent>()->GetHealth()));
	}

}