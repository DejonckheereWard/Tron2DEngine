#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace Engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Init();
		void Update(float deltaTime);
		void Render();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
