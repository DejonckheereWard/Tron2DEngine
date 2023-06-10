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
	const glm::vec4 srcRect{ (position * textureSampleScale), (m_Size * textureSampleScale) };
	const glm::vec4 dstRect{ position, m_Size };

	Engine::Renderer::GetInstance().RenderTexture(*m_pTexture, srcRect, dstRect);
}