#pragma once
#include "BaseComponent.h"

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
		virtual void Update(float) override {};
		virtual void Render() const override;

		virtual void SetTexture(std::shared_ptr<Texture2D> pTexture) { m_Texture = pTexture; };

	protected:
		std::shared_ptr<Texture2D> m_Texture{};
		//Texture2D* m_Texture{};

	private:
		TransformComponent* m_pTransform{};
	};
}

