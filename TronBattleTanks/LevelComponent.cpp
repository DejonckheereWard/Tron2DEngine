#include "LevelComponent.h"
#include <fstream>
#include <sstream>
#include <algorithm>

#include "SceneManager.h"
#include "WallRenderer.h"

#include "TransformComponent.h"

void LevelComponent::Init()
{
	RequireComponent<WallRenderer>();
	m_pWallRenderer = GetOwner()->GetComponent<WallRenderer>();

	// Load in level file
	ReloadLevel();


}

void LevelComponent::Update(float /*deltaTime*/)
{
}


void LevelComponent::Render() const
{
	assert(m_pWallRenderer != nullptr && "Missing wallrenderer");
	const glm::vec2& position{ GetTransform()->GetPosition()};

	for (const Cell& cell : m_Cells)
	{	
		switch (cell.CellType)
		{
		case CellType::Wall:
			m_pWallRenderer->DrawAtPos(position + cell.Position, cell.Size);
			break;
		default:
			break;
		}
	}

}

void LevelComponent::ReloadLevel()
{
	assert(m_LevelFilePath != "");
	assert(m_LevelSize > 1);

	// Read file
	std::ifstream levelFile{ m_LevelFilePath };
	if (levelFile.is_open())
	{
		int nrOfRows{};
		int nrOfCells{};

		std::string inputLine{};
		while (std::getline(levelFile, inputLine))
		{
			std::stringstream stringStrean{inputLine};
			std::string tileValue{};
			while (std::getline(stringStrean, tileValue, ','))
			{
				int cellValue{ std::stoi(tileValue) };
				Cell newCell{};
				if (cellValue == 0)
					newCell.CellType = CellType::Wall;
				else if (cellValue == 2)
					newCell.CellType = CellType::Path;
				else
					newCell.CellType = CellType::Path;  // Default

				newCell.CellIndex = m_Cells.size();
				newCell.Position = { 0, 0 };  // Init to 0
				newCell.Size = { 0, 0 }; // Init to 0

				m_Cells.emplace_back(newCell);
				nrOfCells++;
			}
			nrOfRows++;
		}

		m_NrOfColumns = nrOfCells / nrOfRows;
		m_NrOfRows = nrOfRows;

		// Finished loading
		// Calculate cellsize
		// Go over each cell and set position and cellsize

		m_CellSize =  { m_LevelSize / m_NrOfColumns, m_LevelSize / m_NrOfRows };
		for (int rowIdx = 0; rowIdx < m_NrOfRows; rowIdx++)
		{
			for (int colIdx = 0; colIdx < m_NrOfColumns; colIdx++)
			{
				Cell& cell{ m_Cells[rowIdx * m_NrOfColumns + colIdx] };
				glm::vec2 position{ colIdx* m_CellSize.x, rowIdx * m_CellSize.y };
				cell.Position = position;
				cell.Size = m_CellSize;
			}
		}
	}
	else
	{
		// File error
	}




}

const Cell& LevelComponent::GetCellAtPosition(glm::vec2 position) const
{
	// Convert to local position
	position -= GetTransform()->GetLocalPosition();

	position.x /= m_CellSize.x;
	position.y /= m_CellSize.y;

	int colIdx{ static_cast<int>(position.x) };
	int rowIdx{ static_cast<int>(position.y) };

	colIdx = std::clamp(colIdx, 0, m_NrOfColumns - 1);
	rowIdx = std::clamp(rowIdx, 0, m_NrOfRows - 1);

	int cellIndex{ colIdx + (rowIdx * m_NrOfColumns) };

	assert(cellIndex >= 0 && cellIndex < m_Cells.size() && "Cell index out of bounds");

	return m_Cells[cellIndex];


	

}
