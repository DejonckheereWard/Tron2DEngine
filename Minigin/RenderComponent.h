#pragma once
#include "BaseComponent.h"
#include "glm/vec2.hpp"

namespace Engine
{
	class Texture2D;
	class TransformComponent;
	class RenderComponent: public BaseComponent
	{

	public:
		RenderComponent(GameObject* pOwner);

		// Inherited via BaseComponent
		virtual void Init() override;
		virtual void Update() override {};
		virtual void Render() const override;

		virtual void SetTexture(std::shared_ptr<Texture2D> pTexture) { m_Texture = pTexture; };
		virtual void SetTextureOffset(const glm::vec2& offset) { m_TextureOffset = offset; }; 
		virtual void SetTextureRotation(float rotation) { m_TextureRotation = rotation; }; // In degrees

	protected:
		std::shared_ptr<Texture2D> m_Texture{};

	private:
		TransformComponent* m_pTransform{};
		glm::vec2 m_TextureOffset{};
		float m_TextureRotation{};  // In degrees
	};
}

