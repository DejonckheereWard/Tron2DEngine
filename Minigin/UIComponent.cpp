#include "UIComponent.h"
#include "GameObject.h"
#include "FPSComponent.h"
#include "TextComponent.h"


void dae::UIComponent::Update(GameObject& gameObject, float deltaTime)
{
	deltaTime;
	int fps{ gameObject.GetComponent<FPSComponent>()->m_FPS };
	std::string fpsText{ std::to_string(fps) };	
	gameObject.GetComponent<TextComponent>()->SetText(fpsText);

}

void dae::UIComponent::Render([[maybe_unused]]const GameObject& gameObject) const
{
}
