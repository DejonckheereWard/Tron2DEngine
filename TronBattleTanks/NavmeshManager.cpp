#include "NavmeshManager.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include <queue>
#include <vector>
#include <unordered_map>


NavmeshManager::~NavmeshManager()
{
	for (NavmeshNode* pNode : m_pNavMeshNodes)
	{
		delete pNode;
	}
	m_pNavMeshNodes.clear();
}

NavmeshNode* NavmeshManager::GetNodeAtPosition(const glm::vec2& position) const
{
	assert(m_IsInitialized && "NavmeshManager::GetPath >> NavmeshManager is not initialized");
	const glm::vec2& cellSize{ m_NavmeshSettings.CellSize };

	for (NavmeshNode* pNode : m_pNavMeshNodes)
	{
		// Positions are central
		if (position.x >= pNode->Position.x - cellSize.x / 2.0f && 
			position.x <= pNode->Position.x + cellSize.x / 2.0f &&
			position.y >= pNode->Position.y - cellSize.y / 2.0f && 
			position.y <= pNode->Position.y + cellSize.y / 2.0f)
		{
			return pNode;
		}
	}
	return nullptr;

}

void NavmeshManager::GenerateNavMesh(const NavmeshSettings& navmeshSettings)
{
	m_NavmeshSettings = navmeshSettings;
	m_IsInitialized = true;

	// Generate all points
	size_t nrOfPoints = static_cast<size_t>(navmeshSettings.GridSize.x) * navmeshSettings.GridSize.y;
	m_pNavMeshNodes.clear();
	m_pNavMeshNodes.reserve(nrOfPoints);

	for (int rowIndex{}; rowIndex < navmeshSettings.GridSize.y; ++rowIndex)
	{
		for (int columnIndex{}; columnIndex < navmeshSettings.GridSize.x; ++columnIndex)
		{
			NavmeshNode* pNavmeshNode = new NavmeshNode();
			pNavmeshNode->Position = navmeshSettings.Position + glm::vec2(columnIndex * navmeshSettings.CellSize.x, rowIndex * navmeshSettings.CellSize.y);
			pNavmeshNode->Position += navmeshSettings.CellSize * 0.5f;
			m_pNavMeshNodes.emplace_back(pNavmeshNode);
		}
	}

	Engine::CollisionManager& collisionManager{ Engine::CollisionManager::GetInstance() };

	// Remove points that are inside a collider
	for (int i{ static_cast<int>(m_pNavMeshNodes.size()) - 1 }; i >= 0; --i)
	{
		if (collisionManager.IsPointInCollider(m_pNavMeshNodes[i]->Position, Engine::CollisionLayer::World))
		{
			delete m_pNavMeshNodes[i];
			m_pNavMeshNodes[i] = nullptr;
			m_pNavMeshNodes.erase(m_pNavMeshNodes.begin() + i);
		}
	}

	// Generate connections
	for (NavmeshNode* pNode : m_pNavMeshNodes)
	{
		pNode->pRight = GetNodeAtPosition(pNode->Position + glm::vec2{ navmeshSettings.CellSize.x, 0 });

		pNode->pLeft = GetNodeAtPosition(pNode->Position + glm::vec2{ -navmeshSettings.CellSize.x, 0 });

		pNode->pUp = GetNodeAtPosition(pNode->Position + glm::vec2{ 0, navmeshSettings.CellSize.y });

		pNode->pDown = GetNodeAtPosition(pNode->Position + glm::vec2{ 0, -navmeshSettings.CellSize.y });

	}

}

std::vector<glm::vec2> NavmeshManager::GetPath(const glm::vec2& startPos, const glm::vec2& endPos) const
{
	// Breadth-first search to find A path
	assert(m_IsInitialized && "NavmeshManager::GetPath >> NavmeshManager is not initialized");

	NavmeshNode* pStartNode{ GetNodeAtPosition(startPos) };
	NavmeshNode* pEndNode{ GetNodeAtPosition(endPos) };
	std::vector<glm::vec2> path{};

	if (!pStartNode || !pEndNode)
	{
		return path;
	}

	if (pStartNode == pEndNode)
	{
		path.emplace_back(endPos);
		return path;
	};

	// Find path from start to end
	std::queue<NavmeshNode*> openList{};
	std::unordered_map<NavmeshNode* , NavmeshNode*> closedList{};  // Key = current node, Value = previous node
	
	openList.push(pStartNode);  // Start with start node (duh)

	while (not openList.empty())
	{
		NavmeshNode* pCurrentNode{ openList.front() };
		openList.pop();

		// Check if we found the correct destination, stop the loop if we did
		if (pCurrentNode == pEndNode)
		{
			break;
		}

		// Fill all the neighbours to the openList
		std::vector<NavmeshNode*> neighbors{
			pCurrentNode->pRight,
			pCurrentNode->pLeft,
			pCurrentNode->pUp,
			pCurrentNode->pDown
		};

		for (NavmeshNode* pNeighbor : neighbors)
		{
			if (closedList.find(pNeighbor) == closedList.end())
			{
				openList.push(pNeighbor);
				closedList[pNeighbor] = pCurrentNode;
			}
		}
	}

	if(closedList.find(pEndNode) == closedList.end())
	{
		path.emplace_back(pEndNode->Position);
		return path;
	}

	// Backtrack from end to start
	NavmeshNode* pCurrentNode{ pEndNode };
	while (pCurrentNode != pStartNode)
	{
		path.emplace_back(pCurrentNode->Position);
		pCurrentNode = closedList[pCurrentNode];
	}

	// Add start position
	path.emplace_back(pStartNode->Position);

	// Reverse the path
	std::reverse(path.begin(), path.end());

	return path;

}

void NavmeshManager::RenderMesh() const
{
	const SDL_Color& color{ SDL_Color{ 255, 0, 0, 255} };
	for (NavmeshNode* pNode : m_pNavMeshNodes)
		{
			// Draw connections
			if (pNode->pRight)
			{
				Engine::Renderer::GetInstance().RenderLine(pNode->Position, pNode->pRight->Position, color);
			}
			if (pNode->pLeft)
			{
				Engine::Renderer::GetInstance().RenderLine(pNode->Position, pNode->pLeft->Position, color);
			}
			if (pNode->pUp)
			{
				Engine::Renderer::GetInstance().RenderLine(pNode->Position, pNode->pUp->Position, color);
			}
			if (pNode->pDown)
			{
				Engine::Renderer::GetInstance().RenderLine(pNode->Position, pNode->pDown->Position, color);
			}

			// Draw node
			Engine::Renderer::GetInstance().RenderPoint(pNode->Position, 2.0f);
	}


}
