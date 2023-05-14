#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"

Engine::RenderComponent::RenderComponent(GameObject* pOwner):
	BaseComponent(pOwner)
{
}

void Engine::RenderComponent::Init()
{
	m_pTransform = GetOwner()->GetTransform();
}

void Engine::RenderComponent::Render() const
{
	if(!m_Texture) 
		return;  // Dont do anything if we dont have a texture!

	const glm::vec2& pos{ m_pTransform->GetPosition() };

	// -rotation because SDL rotates clockwise while in the engine, positive rotations are counter clockwise
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, -m_pTransform->GetRotation(), m_TextureOffset );
}
