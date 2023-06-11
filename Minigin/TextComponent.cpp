#include "TextComponent.h"
#include <SDL_ttf.h>
#include <stdexcept>
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include <glm/glm.hpp>

Engine::TextComponent::TextComponent(GameObject* pOwner):
	BaseComponent(pOwner)
{
}

void Engine::TextComponent::Init()
{
	RequireComponent<RenderComponent>();
	m_pRenderComponent = GetOwner()->GetComponent<RenderComponent>();
}

void Engine::TextComponent::Update()
{
	// Only update texture if the text is changed (dirty)
	if(m_TextIsDirty)
	{
		UpdateTexture();
	}
	m_TextIsDirty = false;
}

void Engine::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_TextIsDirty = true;
}

void Engine::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_Font = font;
	m_TextIsDirty = true;
}

void Engine::TextComponent::SetColor(const glm::vec3& color)
{
	assert(m_Color.r <= 1.f && m_Color.g <= 1.f && m_Color.b <= 1.f && "Color values should be between 0 and 1");
	assert(m_Color.r >= 0.f && m_Color.g <= 0.f && m_Color.b <= 0.f && "Color values should be between 0 and 1");
	m_Color = color;
	m_TextIsDirty = true;
}

void Engine::TextComponent::UpdateTexture()
{
	SDL_Color sdlColor{};
	sdlColor.r = static_cast<Uint8>(m_Color.r * 255);
	sdlColor.g = static_cast<Uint8>(m_Color.g * 255);
	sdlColor.b = static_cast<Uint8>(m_Color.b * 255);

	if(m_Text == "")
	{
		m_Texture = nullptr;
		m_pRenderComponent->SetTexture(m_Texture);
		return;
	}

	const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), sdlColor);
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
	m_pRenderComponent->SetTexture(m_Texture);
}
