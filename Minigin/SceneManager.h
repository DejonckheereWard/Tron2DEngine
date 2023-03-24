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
		Scene* CreateScene(const std::string& name);
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

		std::vector<std::unique_ptr<Scene>> m_Scenes;  // Dont initalize 

	};
}
