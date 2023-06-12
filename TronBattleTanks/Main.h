#pragma once

#include <glm/vec2.hpp>

namespace Engine
{
	class GameObject;
	class Scene;
}
class GameMode;
class ScoreComponent;


void InitGame();

void MainScene();

Engine::GameObject* SpawnPlayerA(Engine::Scene* pScene, GameMode* pGameMode, const glm::vec2& pos, unsigned int controllerIdx = UINT_MAX);
Engine::GameObject* SpawnPlayerB(Engine::Scene* pScene, GameMode* pGameMode, const glm::vec2& pos, unsigned int controllerIdx = UINT_MAX);
Engine::GameObject* SpawnEnemyTank(Engine::Scene* pScene, GameMode* pGameMode, const glm::vec2& pos);
Engine::GameObject* SpawnEnemyRecognizer(Engine::Scene* pScene, GameMode* pGameMode, const glm::vec2& pos);
void CreateLevel(Engine::Scene* pScene, GameMode* pGameMode);