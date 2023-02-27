#pragma once
#include <memory>
#include "Transform.h"
#include <variant>
#include <unordered_map>
#include <typeindex>
#include <string>

namespace dae
{
	class Texture2D;
	class BaseComponent;

	// todo: this should become final.
	class GameObject
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// Components
		template <typename TComponent> TComponent* AddComponent();
		template <typename TComponent> TComponent* GetComponent();
		template <typename TComponent> void RemoveComponent();

		// Getters
		const Transform& GetTransform() const { return m_Transform; }

	private:
		Transform m_Transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};

		std::unordered_map<std::type_index, BaseComponent*> m_Components;

	};
	
	template<typename TComponent>
	inline TComponent* GameObject::AddComponent()
	{
		TComponent* component{ new TComponent() };
		//m_Components.insert(std::make_pair(typeid(TComponent), static_cast<BaseComponent*>(component)));
		m_Components[std::type_index(typeid(TComponent))] = component;
		return component;
	}
	template<typename TComponent>
	inline TComponent* GameObject::GetComponent()
	{
		return static_cast<TComponent*>(m_Components.at(std::type_index(typeid(TComponent))));
	}
	template<typename TComponent>
	inline void GameObject::RemoveComponent()
	{
		delete m_Components.at(std::type_index(typeid(TComponent)));
		m_Components.erase(std::type_index(typeid(TComponent)));
	}
}