#include "LevelLoader.h"
#include <cassert>
#include <fstream>
#include <sstream>

#include "ResourceManager.h"
#include "CollisionManager.h"
#include "GameObject.h"

#include "TransformComponent.h"
#include "CollisionComponent.h"


#include "WallRenderer.h"
#include "PathRenderer.h"

Engine::GameObject* LevelLoader::LoadLevel(const LevelSettings& levelSettings, Engine::Scene* pScene)
{
	using Engine::GameObject;
	using Engine::CollisionComponent;

	assert(levelSettings.FilePath != "");
	assert(pScene != nullptr);

	// Read file
	std::ifstream levelFile{ Engine::ResourceManager::GetInstance().GetResourcePath(levelSettings.FilePath)};
	if (levelFile.is_open())
	{
		GameObject* pLevel{ pScene->AddChild(new GameObject()) };
		pLevel->GetTransform()->SetLocalPosition(levelSettings.Position);

		std::shared_ptr<Engine::Texture2D> pWallTexture{ Engine::ResourceManager::GetInstance().LoadTexture(levelSettings.WallTexturePath) };
		int rowIdx{};
		std::string inputLine{};
		while (std::getline(levelFile, inputLine))
		{
			int colIdx{};
			std::stringstream stringStream{ inputLine };
			std::string tileValue{};
			while (std::getline(stringStream, tileValue, ','))
			{
				glm::vec2 cellPosition{};
				cellPosition.x = static_cast<float>(colIdx * levelSettings.CellSize);
				cellPosition.y = static_cast<float>(rowIdx * levelSettings.CellSize);

				GameObject* pCell{ pLevel->AddChild(new GameObject()) };
				pCell->GetTransform()->SetLocalPosition(cellPosition);

				int cellValue{ std::stoi(tileValue) };

				if (cellValue == 0)
				{
					WallRenderer* pWallRenderer{ pCell->AddComponent<WallRenderer>() };
					pWallRenderer->SetTexture(pWallTexture);
					pWallRenderer->SetSize(glm::vec2{ levelSettings.CellSize, levelSettings.CellSize });
					CollisionComponent* pCollider{ pCell->AddComponent<CollisionComponent>() };
					pCollider->SetColliderSize(glm::vec2{ levelSettings.CellSize, levelSettings.CellSize });
				}
				else
				{
					PathRenderer* pPathRenderer{ pCell->AddComponent<PathRenderer>() };
					pPathRenderer->SetSize(glm::vec2{ levelSettings.CellSize, levelSettings.CellSize });
				}
				colIdx++;
			}
			rowIdx++;
		}
		return pLevel;
	}
	else
	{
		// File error
	}
	return nullptr;
}

