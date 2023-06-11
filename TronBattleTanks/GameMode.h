#pragma once

#include "BaseComponent.h"
#include "Observer.h"

namespace Engine
{
	class GameObject;
}

class GameMode : public Engine::BaseComponent, public Engine::Observer
{
public:
	GameMode(Engine::GameObject* pParent) :
		BaseComponent(pParent)
	{}

	// Inherited via BaseComponent
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;

	// Inherited via Observer
	virtual void OnNotify(Engine::GameObject* entity, const std::string& eventName) override;

};

