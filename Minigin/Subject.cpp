#include "Subject.h"
#include "Observer.h"



void Engine::Subject::AddObserver(Observer* observer)
{
	m_Observers.push_back(observer);
}

void Engine::Subject::RemoveObserver(Observer* observer)
{
	std::erase(m_Observers, observer);
}

void Engine::Subject::Notify(GameObject* actor, unsigned int eventID) const
{
	for(Engine::Observer* observer : m_Observers)
	{
		observer->OnNotify(actor, eventID);
	}
}
