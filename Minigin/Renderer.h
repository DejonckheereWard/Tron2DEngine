#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Engine
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float angle, const glm::vec2& center, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE) const;
		void RenderTexture(const Texture2D& texture, const glm::vec4& srcRect, const glm::vec4 dstRect, float angle = 0.f, const glm::vec2& center = {}, SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE) const;

		void RenderLine(const glm::vec2& startPos, const glm::vec2& endPos, const SDL_Color& color = { 255,255,255,255 } ) const;
		void RenderRect(const glm::vec4& rect, const SDL_Color& color = { 255,255,255,255 }) const;
		void RenderPoint(const glm::vec2& pos, float size, const SDL_Color& color = { 255,255,255,255 }) const;

		SDL_Renderer* GetSDLRenderer() const;

		glm::vec2 GetWindowSize() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

	private:
		glm::vec2 m_WindowSize{};
	};
}

