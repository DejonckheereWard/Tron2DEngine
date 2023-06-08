#pragma once
#include "GameObject.h"
#include "BaseComponent.h"
#include "RenderComponent.h"

class Engine::Texture2D;
class WallRenderer final : public Engine::BaseComponent
{
public:
	WallRenderer(Engine::GameObject* pParent):
		Engine::BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	void SetBackgroundTexture(std::shared_ptr<Engine::Texture2D> pTexture) { m_pBackgroundTexture = pTexture; }

private:
	std::shared_ptr<Engine::Texture2D> m_pBackgroundTexture{};  // Wall texture to sample from


};
