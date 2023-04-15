#pragma once

namespace Engine
{
	class GameObject;
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;
		virtual void OnNotify(GameObject* entity, unsigned int eventID) = 0;
	};
}
