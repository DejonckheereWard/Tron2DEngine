#pragma once

void MainScene();

Engine::GameObject* SpawnPlayer(Engine::Scene* scene);
Engine::GameObject* SpawnEnemy(Engine::Scene* scene, Engine::GameObject* pTarget);
