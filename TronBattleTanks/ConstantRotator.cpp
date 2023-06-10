#include "ConstantRotator.h"
#include "TransformComponent.h"
#include "glm/gtx/rotate_vector.hpp"


Engine::ConstantRotator::ConstantRotator(GameObject* parent):
	BaseComponent(parent)
{
}

void Engine::ConstantRotator::Init()
{
	m_pTransform = GetOwner()->GetTransform();
	//m_Radius =  glm::distance(glm::vec2(0,0), m_pTransform->GetLocalPosition());
}

void Engine::ConstantRotator::Update()
{
	const float deltaTime{ GameTimer::GetInstance().GetDeltaTime() };
	const glm::vec2 newPosition{ glm::rotate(m_pTransform->GetLocalPosition(), glm::radians(m_AnglePerSec) * deltaTime) };
	m_pTransform->SetLocalPosition(newPosition);
}