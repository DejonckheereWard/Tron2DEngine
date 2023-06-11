#pragma once

namespace Engine
{
	class GameObject;
	class Scene;
}
class GameMode;

void MainScene();

Engine::GameObject* SpawnPlayer(Engine::Scene* pScene, GameMode* pGameMode);
Engine::GameObject* SpawnEnemy(Engine::Scene* pScene, Engine::GameObject* pTarget, GameMode* pGameMode);
void CreateLevel(Engine::Scene* pScene, GameMode* pGameMode);