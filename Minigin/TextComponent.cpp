#include "TextComponent.h"
#include <SDL_ttf.h>
#include <stdexcept>
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"

void dae::TextComponent::Render(const GameObject& gameObject) const
{
	if(m_Texture != nullptr)
	{
		glm::vec3 position{ gameObject.GetTransform().GetPosition() };
		Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y);
	}
}

void dae::TextComponent::UpdateTexture()
{
	const SDL_Color color = { 255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
	if(surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if(texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_Texture = std::make_shared<Texture2D>(texture);
}
 