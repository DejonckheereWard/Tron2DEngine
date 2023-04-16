#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "Texture2D.h"

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

	m_pTransform;
	const glm::vec2& pos{ m_pTransform->GetPosition() };

	glm::ivec2 size{ m_Texture->GetSize() };
	size *= m_pTransform->GetScale();


	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, (float)size.x, (float)size.y);
}
