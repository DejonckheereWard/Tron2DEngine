#include "WallRenderer.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "glm/vec4.hpp"

void WallRenderer::Init()
{
	if (m_pBackgroundTexture)
	{
		//m_pBackgroundTexture->
	}

}

void WallRenderer::Update()
{

}

void WallRenderer::Render() const
{
}

void WallRenderer::DrawAtPos(const glm::vec2& position, const glm::vec2& size)
{
	const glm::vec2 windowSize{ Engine::Renderer::GetInstance().GetWindowSize() };

	const glm::ivec2 cellIndex{ static_cast<glm::ivec2>(position / size) };  // divides x by x and y by y, stores as ints (ivec)
	const glm::vec2 pos{ static_cast<glm::vec2>(cellIndex)* size };

	const glm::vec2 textureSampleScale{ glm::vec2(2.5f, 2.5f) };

	const glm::vec4 srcRect{(pos* textureSampleScale), (size* textureSampleScale)};
	const glm::vec4 dstRect{ position, size};

	Engine::Renderer::GetInstance().RenderTexture(*m_pBackgroundTexture, srcRect, dstRect);
}
