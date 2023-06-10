#pragma once

void MainScene();

Engine::GameObject* SpawnPlayer(Engine::Scene* pScene);
Engine::GameObject* SpawnEnemy(Engine::Scene* pScene, Engine::GameObject* pTarget);
void CreateLevel(Engine::Scene* pScene);