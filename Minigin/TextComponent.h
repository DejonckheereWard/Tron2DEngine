#pragma once
#include "BaseComponent.h"
#include <string>
#include <memory>
#include "Font.h"
#include "Texture2D.h"
#include "glm/vec3.hpp"

namespace Engine
{
	class GameObject;
	class TextComponent: public BaseComponent
	{
	public:

		TextComponent(GameObject* pOwner);

		// Inherited via BaseComponent
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() const override {};

	public:
		// Component specific
		void SetText(const std::string& text);
		void SetFont(std::shared_ptr<Font> font);
		void SetColor(const glm::vec3& color);

	private:
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_Texture;
		glm::vec3 m_Color{ 1.0f, 1.0f, 1.0f };
		bool m_TextIsDirty{ true };

		// Components used
		class RenderComponent* m_pRenderComponent{ nullptr };
		

		void UpdateTexture();
	};
}


