#pragma once
#include <memory>
#include <variant>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <stdexcept>

namespace Engine
{
	class Texture2D;
	class BaseComponent;
	class TransformComponent;
	class MissingComponent {};


	template<typename TComponent>
	concept ComponentType = std::is_base_of<BaseComponent, TComponent>::value;

	class GameObject final
	{
	public:
		void Init();
		void Update(float deltaTime);
		void Render() const;
		void OnImGui();

		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// Scenegraph
		GameObject* GetParent() { return m_Parent; };
		void SetParent(GameObject* parent, bool keepWorldPosition = false);  // Bit like "move child to new parent"
		void AddChild(GameObject* child, bool keepWorldPosition = false);  // Adds the child to this gameobject, making this gameobject the owner
		void RemoveChild(GameObject* child);  // Should delete


		//const std::vector<std::unique_ptr<GameObject>>& GetChildren() { return m_Children; };
		const std::vector<GameObject*>& GetChildren() { return m_Children; };


		// Components
		template <ComponentType TComponent> TComponent* AddComponent();
		template <ComponentType TComponent> TComponent* GetComponent();
		template <ComponentType TComponent> void RemoveComponent();

		TransformComponent* GetTransform() { return m_pTransform; };

	private:
		std::unordered_map<std::type_index, BaseComponent*> m_Components{};

		TransformComponent* const m_pTransform;

		GameObject* m_Parent{ nullptr };
		//std::vector<std::unique_ptr<GameObject>> m_Children{};
		std::vector<GameObject*> m_Children{};


		void AddToChildCollection(GameObject* child);
		void RemoveFromChildCollection(GameObject* child);
	};


	template<ComponentType TComponent>
	inline TComponent* GameObject::AddComponent()
	{
		static_assert(std::is_base_of<BaseComponent, TComponent>::value, "Component must inherit from BaseComponent");

		const std::type_index componentTypeID{ std::type_index(typeid(TComponent)) };  // Get Typeid (expensive)
		if(m_Components.contains(componentTypeID))
		{
			// Component already exists, giving existing
			return GetComponent<TComponent>();
		}

		// Create component if it doesnt exist yet
		TComponent* component{ new TComponent(this) };  // Create new component and give current gameobject as paramater (pOwner)
		m_Components[componentTypeID] = component;
		return component;
	}
	template<ComponentType TComponent>
	inline TComponent* GameObject::GetComponent()
	{
		// Find component in map using its typeid (typeid is expensive, avoid in hot code path)

		try
		{
			return static_cast<TComponent*>(m_Components.at(std::type_index(typeid(TComponent))));
		}
		catch(std::out_of_range&)
		{
			throw MissingComponent();
		}
	}
	template<ComponentType TComponent>
	inline void GameObject::RemoveComponent()
	{
		const std::type_index componentTypeID{ std::type_index(typeid(TComponent)) };
		delete m_Components.at(componentTypeID);
		m_Components.erase(componentTypeID);
	}
}