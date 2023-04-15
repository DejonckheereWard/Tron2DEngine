#include "HealthComponent.h"

void HealthComponent::Init()
{
	m_pSubject->Notify(GetOwner(), "HealthChanged");

}

void HealthComponent::Update(float /*deltaTime*/)
{
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
	if(m_Health > 0)
	{ 	
		m_Health -= damage;
		m_pSubject->Notify(GetOwner(), "HealthChanged");
	}
	if(m_Health <= 0)
	{
		--m_Lives;
		m_Health = 0;

		if(m_Lives > 0)
		{
			m_Health = m_MaxHealth;
			m_pSubject->Notify(GetOwner(), "HealthChanged");
		}

		m_pSubject->Notify(GetOwner(), "PlayerDeath");

	}
}

void HealthComponent::Heal(int heal)
{
	if(m_Health < m_MaxHealth)
	{
		m_Health += heal;

		if(m_Health > m_MaxHealth)
		{
			m_Health = m_MaxHealth;
		}
	}
	m_pSubject->Notify(GetOwner(), "HealthChanged");
}

int HealthComponent::GetLives()
{
	return m_Lives;
}
