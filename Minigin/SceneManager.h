#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <vector>

namespace Engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		void AddScene(Scene* scene);
		Scene* GetScene(const std::string& name) const;
		void RemoveScene(const std::string& name);

		void Init();
		void Update(float deltaTime);
		void Render() const;
		void OnImGui();


	private:
		friend class Singleton<SceneManager>;

		// Destructor AND Constructor need to be defined in the CPP file for smart pointers to accept forward declarations
		~SceneManager();
		SceneManager();
		std::vector<Scene*> m_Scenes{};
		//std::vector<std::unique_ptr<Scene>> m_Testing{};
		std::unique_ptr<Scene> m_Test;

	};
}
