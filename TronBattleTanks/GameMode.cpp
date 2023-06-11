#include "GameMode.h"
#include "GameObject.h"


void GameMode::Init()
{
}

void GameMode::Update()
{
}

void GameMode::Render() const
{
}

void GameMode::OnNotify(Engine::GameObject* entity, const std::string& eventName)
{
	if (eventName == "PlayerDeath")
	{
		entity->MarkForDeletion();

	}

}