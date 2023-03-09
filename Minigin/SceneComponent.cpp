#include "SceneComponent.h"

unsigned int Engine::SceneComponent::m_SceneCounter = 0;

Engine::SceneComponent::SceneComponent(GameObject* pOwner):
	BaseComponent(pOwner)
{
	++m_SceneCounter;
}

Engine::SceneComponent::~SceneComponent()
{
	--m_SceneCounter;
}
