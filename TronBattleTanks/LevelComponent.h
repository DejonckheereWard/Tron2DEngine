#pragma once
#include "BaseComponent.h"
#include <glm/vec2.hpp>
#include "ResourceManager.h"

enum class CellType {
	Wall,
	Path,
	Finish
};

struct Cell
{
	size_t CellIndex;
	CellType CellType{ CellType::Wall };
	glm::vec2 Position;
	glm::vec2 Size;
};

class WallRenderer;
class LevelComponent final : public Engine::BaseComponent
{

public:
	LevelComponent(Engine::GameObject* pParent, int levelSize = 500) :
		Engine::BaseComponent(pParent),
		m_LevelSize{ levelSize }
	{}

	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Render() const override;

	void ReloadLevel();  // Loads / reloads level from scratch
	void SetLevelSize(int levelSize) { m_LevelSize = levelSize; }
	void SetLevelFile(const std::string& filePath) { m_LevelFilePath = Engine::ResourceManager::GetInstance().GetResourcePath(filePath); };

	const Cell& GetCellAtPosition(glm::vec2 position) const;  // World position

private:
	WallRenderer* m_pWallRenderer{};
	std::string m_LevelFilePath{};
	
	std::vector<Cell> m_Cells{};

	int m_LevelSize{};
	glm::vec2 m_CellSize{};  // Based on the levelsize & nr of cells per row
	int m_NrOfRows{};
	int m_NrOfColumns{};
};

