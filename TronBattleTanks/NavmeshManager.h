#pragma once
#include "Singleton.h"
#include <glm/vec2.hpp>
#include <vector>

struct NavmeshSettings
{
	glm::vec2 Position;  // bottom left corner of the grid
	glm::vec2 CellSize;  // width, height per cell
	glm::ivec2 GridSize;  // rows, columns
};

struct NavmeshNode
{
	glm::vec2 Position;

	// Neighbours
	NavmeshNode* pUp{ nullptr };
	NavmeshNode* pDown{ nullptr };
	NavmeshNode* pRight{ nullptr };
	NavmeshNode* pLeft{ nullptr };
};


class NavmeshManager final : public Engine::Singleton<NavmeshManager>
{
public:
	~NavmeshManager();

	NavmeshNode* GetNodeAtPosition(const glm::vec2& position) const;

	void GenerateNavMesh(const NavmeshSettings& navmeshSettings);

	std::vector<glm::vec2> GetPath(const glm::vec2& startPos, const glm::vec2& endPos) const;

	void RenderMesh() const;

private:
	friend class Singleton<NavmeshManager>;
	NavmeshManager() = default;

	bool m_IsInitialized{ false };

	std::vector<NavmeshNode*> m_pNavMeshNodes;
	NavmeshSettings m_NavmeshSettings;

};

