#pragma once
#include <string>

namespace Engine
{
	class GameObject;
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;
		virtual void OnNotify(GameObject* entity, const std::string& eventName) = 0;
	};
}
