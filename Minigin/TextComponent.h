#pragma once
#include "BaseComponent.h"
#include <string>
#include <memory>
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	class TextComponent: public BaseComponent
	{
	public:
		
		TextComponent() = default;

		// Inherited via BaseComponent
		virtual void Update(GameObject&, float) override {};
		virtual void Render(const GameObject& gameObject) const override;

		void SetText(const std::string& text) { m_Text = text; UpdateTexture(); };
		void SetFont(std::shared_ptr<Font> font) { m_Font = font; };
		void UpdateTexture();

	private:
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_Texture; // Generated using the font
	};
}


