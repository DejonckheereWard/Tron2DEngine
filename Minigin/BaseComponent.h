#pragma once

namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;
		virtual void Update(GameObject& gameObject, float deltaTime) = 0;
		virtual void Render([[maybe_unused]]const GameObject& gameObject) const {};
	};
}


