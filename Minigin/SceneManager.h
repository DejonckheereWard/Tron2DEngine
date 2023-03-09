#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "GameObject.h"

namespace Engine
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void Init();
		void Update(float deltaTime);
		void Render();

		GameObject* CreateScene(const std::string& name);
		GameObject* GetScene(const std::string& name) const;
		void RemoveScene(const std::string& name);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<GameObject>> m_Scenes{};
	};
}
