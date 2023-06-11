#pragma once
#include "BaseComponent.h"
#include "glm/vec2.hpp"
#include <vector>

class NavmeshComponent final : public Engine::BaseComponent
{
public:
	NavmeshComponent(Engine::GameObject* pParent) :
		BaseComponent(pParent) {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() const override;
	virtual void RenderDebug() const override;

	glm::vec2 GetNextInPath();

	void SetDestination(const glm::vec2& destination);
	void SetPosition(const glm::vec2& position) { m_Position = position; }

private:
	class NavmeshManager* m_pNavmeshManager = nullptr;

	const float m_UpdateInterval{ 0.5f };  // how often the path is updated
	float m_UpdateTimer = 0.f;  // timer to keep track of when to update the path

	glm::vec2 m_Position{};
	glm::vec2 m_Destination{};

	std::vector<glm::vec2> m_Path{};



};

