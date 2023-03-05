#pragma once
#include "GameObject.h"

namespace Engine
{
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* pOwner): 
			m_pOwner{ pOwner } {};
		virtual ~BaseComponent() = default;
		virtual void Init() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() const = 0;
		
	
	protected:
		GameObject* GetOwner() const { return m_pOwner; };  // Only needs to be accessed by inheriting components

	private:
		GameObject* const m_pOwner;  // Const pointer so you cant override the pointer (u can still edit the object the pointer is pointing to)
	
	};
}


