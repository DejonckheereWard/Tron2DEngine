#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"

Engine::RenderComponent::RenderComponent(GameObject* pOwner):
	BaseComponent(pOwner)
{
}

void Engine::RenderComponent::Init()
{
	m_pTransform = GetOwner()->GetComponent<TransformComponent>();
}

void Engine::RenderComponent::Render() const
{
	if(!m_Texture) return;  // Dont do anything if we dont have a texture!

	m_pTransform;
	const glm::vec3& pos{ m_pTransform->GetPosition() };

	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}
