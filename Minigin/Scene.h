#pragma once
#include "SceneManager.h"

namespace Engine
{
	class GameObject;
	class Scene final
	{
	public:
		explicit Scene(const std::string& name);
		virtual ~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Init();
		void Update();
		void FixedUpdate();
		void Render() const;
		void RenderDebug() const;
		void OnImGui();

		// Scenegraph
		GameObject* AddChild(GameObject* child);  // Adds the child to the scene
		void RemoveChild(GameObject* child);  // Should delete
		void RemoveChildIndex(size_t index);  // Should delete

		const std::vector<GameObject*>& GetChildren() { return m_Children; };

		const std::string& GetName() { return m_SceneName; };

	private:		
		bool m_IsInitialized{ false };
		const std::string m_SceneName;

		std::vector<GameObject*> m_Children{};
		std::vector<GameObject*> m_ChildrenToAdd{}; 
		
	};
}

