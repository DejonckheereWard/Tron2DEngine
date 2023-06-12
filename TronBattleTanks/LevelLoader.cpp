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

Engine::GameObject* LevelLoader::LoadLevel(const LevelSettings& levelSettings, Engine::Scene* pScene, glm::ivec2& outGridSize, std::unordered_map<std::string, std::vector<glm::vec2>>& outObjectPositions)
{
	using Engine::GameObject;
	using Engine::CollisionComponent;

	assert(levelSettings.FilePath != "");
	assert(pScene != nullptr);

	// Read file
	std::ifstream levelFile{ Engine::ResourceManager::GetInstance().GetResourcePath(levelSettings.FilePath)};
	if (levelFile.is_open())
	{
		// Reverse read in lines
		std::vector<std::string> lines{};
		std::string line{};
		while (std::getline(levelFile, line))
		{
			lines.emplace_back(line);
		}
		std::reverse(lines.begin(), lines.end());



		GameObject* pLevel{ pScene->AddChild(new GameObject()) };
		pLevel->GetTransform()->SetLocalPosition(levelSettings.Position);

		std::shared_ptr<Engine::Texture2D> pWallTexture{ Engine::ResourceManager::GetInstance().LoadTexture(levelSettings.WallTexturePath) };
		int rowIdx{};
		int colIdx{};

		for(const std::string& inputLine: lines)
		{
			colIdx = 0;
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
					//PathRenderer* pPathRenderer{ pCell->AddComponent<PathRenderer>() };
					//pPathRenderer->SetSize(glm::vec2{ levelSettings.CellSize, levelSettings.CellSize });
					//
					const glm::vec2 cellCenterPosition{ cellPosition };
					if(cellValue == 1)
					{
						outObjectPositions["Player"].push_back(cellCenterPosition);
					}
					else if (cellValue == 3)
					{
						outObjectPositions["EnemyTank"].push_back(cellCenterPosition);
					}
					else if (cellValue == 4)
					{
						outObjectPositions["EnemyRecognizer"].push_back(cellCenterPosition);
					}


				}
				colIdx++;
			}
			rowIdx++;
		}
		outGridSize.x = colIdx;
		outGridSize.y = rowIdx;
		return pLevel;
	}
	else
	{
		// File error
	}
	return nullptr;
}

