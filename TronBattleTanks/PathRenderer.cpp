#include "PathRenderer.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "glm/vec4.hpp"

void PathRenderer::Init()
{
}

void PathRenderer::Update()
{
}

void PathRenderer::Render() const
{
	const glm::vec2 windowSize{ Engine::Renderer::GetInstance().GetWindowSize() };
	const glm::vec2 position{ GetTransform()->GetLocalPosition() };

	const glm::vec2 textureSampleScale{ glm::vec2(2.5f, 2.5f) };
	const glm::vec4 dstRect{ GetTransform()->GetPosition(), m_Size };  // Use world position for texture position

	Engine::Renderer::GetInstance().RenderRect(dstRect, SDL_Color{ 0, 0, 0, 255 });

	//Engine::Renderer::GetInstance().RenderTexture(*m_pTexture, srcRect, dstRect);

}
