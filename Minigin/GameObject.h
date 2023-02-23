#pragma once
#include <memory>
#include "Transform.h"
#include <variant>
#include <map>

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
	private:
		Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};

		std::map<int, BaseComponent*> m_Components;

	};
	
	template<typename TComponent>
	inline TComponent* GameObject::AddComponent()
	{
		return m_Components.insert(std::make_pair(typeid(TComponent), new TComponent));
	}
	template<typename TComponent>
	inline TComponent* GameObject::GetComponent()
	{
		return m_Components.at(typeid(TComponent));
	}
	template<typename TComponent>
	inline void GameObject::RemoveComponent()
	{
		delete m_Components.at(typeid(TComponent));
		m_Components.erase(typeid(TComponent));
	}
}
