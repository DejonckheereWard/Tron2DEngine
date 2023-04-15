#include "HealthComponent.h"

void HealthComponent::Init()
{
	m_pSubject->Notify(GetOwner(), "HealthChanged");

}

void HealthComponent::Update(float /*deltaTime*/)
{
	if(m_Health <= 0)
	{
		m_pSubject->Notify(GetOwner(), "Death");
	}

}

void HealthComponent::Render() const
{
}

void HealthComponent::SetHealth(int health)
{
	m_Health = health;
	m_pSubject->Notify(GetOwner(), "HealthChanged");
}

void HealthComponent::SetMaxHealth(int maxHealth)
{
	m_MaxHealth = maxHealth;
	m_pSubject->Notify(GetOwner(), "HealthChanged");
}

int HealthComponent::GetHealth() const
{
	return m_Health;
}

int HealthComponent::GetMaxHealth() const
{
	return m_MaxHealth;
}

float HealthComponent::GetHealthPercentage() const
{
	return float(m_Health) / float(m_MaxHealth);
}

void HealthComponent::TakeDamage(int damage)
{
	m_Health -= damage;
	m_pSubject->Notify(GetOwner(), "HealthChanged");
}

void HealthComponent::Heal(int heal)
{
	m_Health += heal;
	m_pSubject->Notify(GetOwner(), "HealthChanged");
}
