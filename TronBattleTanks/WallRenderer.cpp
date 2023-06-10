#include "WallRenderer.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "glm/vec4.hpp"

void WallRenderer::Init()
{
}

void WallRenderer::Update()
{

}

void WallRenderer::Render() const
{
	const glm::vec2 windowSize{ Engine::Renderer::GetInstance().GetWindowSize() };
	const glm::vec2 position{ GetTransform()->GetLocalPosition() };

	const glm::vec2 textureSampleScale{ glm::vec2(2.5f, 2.5f) };
	const glm::vec4 srcRect{ (position * textureSampleScale), (m_Size * textureSampleScale) };  // Use local position for texture sample
	const glm::vec4 dstRect{  GetTransform()->GetPosition(), m_Size };  // Use world position for texture position

	Engine::Renderer::GetInstance().RenderTexture(*m_pTexture, srcRect, dstRect);
}