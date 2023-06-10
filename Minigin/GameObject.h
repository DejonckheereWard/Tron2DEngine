#pragma once
#include <memory>
#include <variant>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <stdexcept>
#include "GameTimer.h"

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
		void Update();
		void FixedUpdate();
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
		GameObject* AddChild(GameObject* child, bool keepWorldPosition = false);  // Adds the child to this gameobject, making this gameobject the owner
		void RemoveChild(GameObject* child);  // Should delete


		//const std::vector<std::unique_ptr<GameObject>>& GetChildren() { return m_Children; };
		const std::vector<GameObject*>& GetChildren() { return m_Children; };


		// Components
		template <ComponentType TComponent, typename... Args> TComponent* AddComponent(Args... args);
		template <ComponentType TComponent> TComponent* GetComponent();
		template <ComponentType TComponent> bool HasComponent() const;
		template <ComponentType TComponent> void RemoveComponent();

		TransformComponent* GetTransform() { return m_pTransform; };

	private:
		std::unordered_map<std::type_index, BaseComponent*> m_Components{};

		TransformComponent* const m_pTransform;

		GameObject* m_Parent{ nullptr };
		std::vector<GameObject*> m_Children{};


		void AddToChildCollection(GameObject* child);
		void RemoveFromChildCollection(GameObject* child);
	};

	template<ComponentType TComponent, typename... Args>
	inline TComponent* GameObject::AddComponent(Args... args)
	{
		static_assert(std::is_base_of<BaseComponent, TComponent>::value, "Component must inherit from BaseComponent");

		const std::type_index componentTypeID{ std::type_index(typeid(TComponent)) };  // Get Typeid (expensive)
		if(m_Components.contains(componentTypeID))
		{
			// Component already exists, giving existing
			return GetComponent<TComponent>();
		}

		// Create component if it doesnt exist yet
		TComponent* component{ new TComponent(this, args...) };  // Create new component and give current gameobject as paramater (pOwner)
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
	inline bool GameObject::HasComponent() const
	{
		return m_Components.count(std::type_index(typeid(TComponent))) > 0;
	}

	template<ComponentType TComponent>
	inline void GameObject::RemoveComponent()
	{
		const std::type_index componentTypeID{ std::type_index(typeid(TComponent)) };
		delete m_Components.at(componentTypeID);
		m_Components.erase(componentTypeID);
	}
}