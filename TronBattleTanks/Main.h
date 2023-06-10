#pragma once

namespace Engine
{
	class GameObject;
	class Scene;
}

void MainScene();

Engine::GameObject* SpawnPlayer(Engine::Scene* pScene);
Engine::GameObject* SpawnEnemy(Engine::Scene* pScene, Engine::GameObject* pTarget);
void CreateLevel(Engine::Scene* pScene);