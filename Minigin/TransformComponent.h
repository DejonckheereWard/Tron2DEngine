#pragma once
#include "BaseComponent.h"
#include <glm/glm.hpp>

namespace Engine
{
	class TransformComponent: public BaseComponent
	{
	public:
		TransformComponent(GameObject* pOwner);

		// Inherited via BaseComponent
		virtual void Init() override {};
		virtual void Update(float) override {};
		virtual void Render() const override {};

		void SetLocalPosition(const glm::vec2& pos);
		void SetLocalPosition(float x, float y);

		void SetLocalRotation(float angle);

		void SetLocalScale(const glm::vec2& scale);
		void SetLocalScale(float x, float y);

		void SetDirty();

		const glm::vec2& GetLocalPosition() const;
		float GetLocalRotation() const;
		const glm::vec2& GetLocalScale() const;

		const glm::vec2& GetPosition();
		const float GetRotation();
		const glm::vec2& GetScale();

		const glm::mat4& GetWorldTransform();
		const glm::mat4& GetLocalTransform();

	private:
		glm::vec2 m_LocalPosition{};
		float m_LocalRotation{};
		glm::vec2 m_LocalScale{ 1.0f, 1.0f};
		
		
		bool m_LocalTransformIsDirty{ true };
		glm::mat4 m_LocalTransform{};
		void UpdateLocalTransform();
		
		bool m_WorldTransformIsDirty{ true };
		glm::mat4 m_WorldTransform{};
		void UpdateWorldTransform();

		glm::vec2 m_WorldPosition{};
		float m_WorldRotation{};
		glm::vec2 m_WorldScale{ 1.0f, 1.0f};

	};
}

