#pragma once
#include "BaseComponent.h"
#include <glm/vec2.hpp>

class PathRenderer final:public Engine::BaseComponent
{
public:
	PathRenderer(Engine::GameObject* pParent) :
		Engine::BaseComponent(pParent) {}


	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;

	void SetSize(const glm::vec2& size) { m_Size = size; }

private:
	glm::vec2 m_Size{};

};

