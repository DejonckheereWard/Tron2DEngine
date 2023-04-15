#pragma once
#include <vector>
#include <memory>
#include <string>

namespace Engine
{
	class Observer;
	class GameObject;
	class Subject final
	{
	public:
		Subject() = default;
		~Subject() = default;

		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

		void AddObserver(Observer* observer);;
		void RemoveObserver(Observer* observer);;

		void Notify(GameObject* actor, const std::string& eventName) const;

	private:
		std::vector<Observer*> m_Observers{};
	};
}

