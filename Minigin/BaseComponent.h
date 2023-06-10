#pragma once
#include "GameObject.h"
#include <iostream>
#include "GameTimer.h"

using Engine::GameTimer;

namespace Engine
{
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* pOwner): 
			m_pOwner{ pOwner } {};
		virtual ~BaseComponent() = default;
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Render() const = 0;
		virtual void RenderDebug() const {};
		virtual void FixedUpdate() {};
		virtual void OnImGui() {};  // Not const because the UI can have sliders, buttons etc. that change the values
		
		TransformComponent* GetTransform() const { return m_pOwner->GetTransform(); }
		GameObject* GetOwner() const { return m_pOwner; }

	
	protected:
		template <ComponentType TComponent>
		void RequireComponent() const;

	private:
		GameObject* const m_pOwner;  // Const pointer so you cant override the pointer (u can still edit the object the pointer is pointing to)
	
	};

	template<ComponentType TComponent>
	inline void BaseComponent::RequireComponent() const
	{
		if(!m_pOwner->HasComponent<TComponent>())
		{
			// Get type of this
			std::string typeName = typeid(*this).name();

			std::cout << "ERROR: required component (" << typeid(TComponent).name() << ") not found for (" << typeName << ")" << std::endl;
		}
	}
}


