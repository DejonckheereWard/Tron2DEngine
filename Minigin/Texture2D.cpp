#include <SDL.h>
#include "Texture2D.h"

Engine::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 Engine::Texture2D::GetSize() const
{
	glm::ivec2 size{};
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &size.x, &size.y);
	return size;
}

SDL_Texture* Engine::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

Engine::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_texture = texture;
}
