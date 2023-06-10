#pragma once
#include "GameObject.h"
#include "BaseComponent.h"
#include "RenderComponent.h"
#include <glm/vec2.hpp>

class Engine::Texture2D;
class WallRenderer final : public Engine::BaseComponent
{
public:
	WallRenderer(Engine::GameObject* pParent):
		Engine::BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;

	void SetTexture(const std::shared_ptr<Engine::Texture2D>& pTexture) { m_pTexture = pTexture; }
	void SetSize(const glm::vec2& size) { m_Size = size; }

private:
	std::shared_ptr<Engine::Texture2D> m_pTexture{};  // Wall texture to sample from
	glm::vec2 m_Size{};

};

