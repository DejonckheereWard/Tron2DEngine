#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace Engine { class TextComponent; };
class HealthDisplay final: public Engine::BaseComponent, public Engine::Observer
{
public:
	HealthDisplay(Engine::GameObject* parent);
	~HealthDisplay() = default;

	HealthDisplay(const HealthDisplay& other) = delete;
	HealthDisplay(HealthDisplay&& other) = delete;
	HealthDisplay& operator=(const HealthDisplay& other) = delete;
	HealthDisplay& operator=(HealthDisplay&& other) = delete;

	// Inherited via BaseComponent
	virtual void Init() override;
	virtual void Update() override {};
	virtual void Render() const override {};

	void OnNotify(Engine::GameObject* pGameObject, const std::string& eventName) override;
	void SetPrefix(const std::string& prefix) { m_Prefix = prefix; };

private:
	Engine::TextComponent* m_pTextComponent;
	std::string m_Prefix{};


};

