#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace Engine { class TextComponent; };
class LivesDisplay final: public Engine::BaseComponent, public Engine::Observer
{
public:
	LivesDisplay(Engine::GameObject* parent);
	~LivesDisplay() = default;

	LivesDisplay(const LivesDisplay& other) = delete;
	LivesDisplay(LivesDisplay&& other) = delete;
	LivesDisplay& operator=(const LivesDisplay& other) = delete;
	LivesDisplay& operator=(LivesDisplay&& other) = delete;

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

