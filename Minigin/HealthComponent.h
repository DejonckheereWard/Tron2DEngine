#pragma once

#include "BaseComponent.h"
#include "Subject.h"

class HealthComponent: public Engine::BaseComponent
{
public:
	HealthComponent(Engine::GameObject* parent):
		BaseComponent(parent)
	{
		m_pSubject = std::make_unique<Engine::Subject>();
	};

	// Inherited via BaseComponent
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	void SetHealth(int health);
	void SetMaxHealth(int maxHealth);
	int GetHealth() const;
	int GetMaxHealth() const;
	float GetHealthPercentage() const;

	void TakeDamage(int damage);
	void Heal(int heal);

	Engine::Subject* GetSubject() { return m_pSubject.get(); };
private:

	int m_Health{ 100 };
	int m_MaxHealth{ 100 };

	std::unique_ptr<Engine::Subject> m_pSubject{};

};

