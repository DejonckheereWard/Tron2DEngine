#pragma once
#include "BaseComponent.h"


namespace dae
{
	class UIComponent: public BaseComponent
	{
		// Inherited via BaseComponent
	public:
		UIComponent() = default;
		virtual void Update(GameObject& gameObject, float deltaTime) override;
		virtual void Render(const GameObject& gameObject) const override;
		
	};
}

