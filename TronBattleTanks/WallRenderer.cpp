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

void WallRenderer::Update(float /*deltaTime*/)
{

}

void WallRenderer::Render() const
{
	const glm::vec2 cellSize{ 32, 32 };
	const glm::vec2 windowSize{ Engine::Renderer::GetInstance().GetWindowSize() };
	const glm::vec2 position{ GetTransform()->GetLocalPosition() };

	const glm::ivec2 cellIndex{ static_cast<glm::ivec2>(position / cellSize) };  // divides x by x and y by y, stores as ints (ivec)

	const glm::vec2 pos{ static_cast<glm::vec2>(cellIndex) * cellSize };

	const glm::vec2 textureSampleScale{ glm::vec2(2.5f, 2.5f) };

	const glm::vec4 srcRect{(pos * textureSampleScale), (cellSize* textureSampleScale)};
	const glm::vec4 dstRect{pos, cellSize};

	Engine::Renderer::GetInstance().RenderTexture(*m_pBackgroundTexture, srcRect, dstRect);


}
