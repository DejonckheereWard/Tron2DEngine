#include "ConstantRotator.h"
#include "TransformComponent.h"


Engine::ConstantRotator::ConstantRotator(GameObject* parent):
	BaseComponent(parent)
{
}

void Engine::ConstantRotator::Init()
{
	m_pTransform = GetOwner()->GetTransform();
}

void Engine::ConstantRotator::Update(float deltaTime)
{
	m_pTransform->SetLocalRotation(m_pTransform->GetLocalRotation() + m_AnglePerSec * deltaTime);
}