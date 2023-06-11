#pragma once
#include <string>
#include <glm/vec2.hpp>

#include "Scene.h"

struct LevelSettings
{
	std::string FilePath{};
	glm::vec2 Position{ 0, 0 };
	int CellSize{ 0 };

	std::string WallTexturePath{};

};


class LevelLoader final
{
	enum class CellType {
		Wall,
		Path,
		Diamond
	};

public:
	static Engine::GameObject* LoadLevel(const LevelSettings& levelSettings, Engine::Scene* pScene, glm::ivec2& outGridSize);


};

