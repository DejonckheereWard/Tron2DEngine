#pragma once
#include "BaseComponent.h"
#include "glm/vec2.hpp"

class NavmeshComponent final: public Engine::BaseComponent
{
public:
	NavmeshComponent(Engine::GameObject* pParent) :
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;

	
private:
	class NavmeshManager* m_pNavmeshManager = nullptr;

};

