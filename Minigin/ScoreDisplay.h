#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace Engine { class TextComponent; };
class ScoreDisplay final: public Engine::BaseComponent, public Engine::Observer
{
public:
	ScoreDisplay(Engine::GameObject* parent);
	~ScoreDisplay() = default;

	ScoreDisplay(const ScoreDisplay& other) = delete;
	ScoreDisplay(ScoreDisplay&& other) = delete;
	ScoreDisplay& operator=(const ScoreDisplay& other) = delete;
	ScoreDisplay& operator=(ScoreDisplay&& other) = delete;

	// Inherited via BaseComponent
	virtual void Init() override;
	virtual void Update(float /*deltaTime*/) override {};
	virtual void Render() const override {};

	void SetPrefix(const std::string& prefix) { m_Prefix = prefix; };

	void OnNotify(Engine::GameObject* pGameObject, const std::string& eventName) override;
private:
	Engine::TextComponent* m_pTextComponent;
	std::string m_Prefix{};
};

