#include "HealthDisplay.h"
#include "TextComponent.h"
#include "HealthComponent.h"

HealthDisplay::HealthDisplay(Engine::GameObject* parent):
	Engine::BaseComponent(parent)
{
	m_pTextComponent = GetOwner()->GetComponent<Engine::TextComponent>();
}

void HealthDisplay::OnNotify(Engine::GameObject* pGameObject, const std::string& eventName)
{
	if(eventName == "HealthChanged")
	{
		m_pTextComponent->SetText(m_Prefix + "Health: " + std::to_string(pGameObject->GetComponent<HealthComponent>()->GetHealth()));
	}

}

void HealthDisplay::Init()
{
}

void HealthDisplay::Update(float /*deltaTime*/)
{
}

void HealthDisplay::Render() const
{
}
