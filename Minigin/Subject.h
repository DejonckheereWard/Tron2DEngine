#pragma once
#include <vector>
#include <memory>

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

	protected:
		void Notify(GameObject* actor, unsigned int eventID) const;
	private:
		std::vector<Observer*> m_Observers{};
	};
}

