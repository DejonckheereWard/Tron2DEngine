#pragma once
#include "GameObject.h"

namespace dae
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
		GameObject* const m_pOwner;
	
	};
}


