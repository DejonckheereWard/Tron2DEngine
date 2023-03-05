#include "TransformComponent.h"

#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#pragma warning(pop)



Engine::TransformComponent::TransformComponent(GameObject* pOwner):
	BaseComponent(pOwner)
{
}

void Engine::TransformComponent::SetLocalPosition(const glm::vec2& pos)
{
	m_LocalPosition = pos;
	
	SetDirty();
}

void Engine::TransformComponent::SetLocalPosition(float x, float y)
{
	SetLocalPosition({x, y});
}

void Engine::TransformComponent::SetLocalRotation(float angle)
{
	// Sets rotation in degrees
	m_LocalRotation = angle;
	
	SetDirty();
}

void Engine::TransformComponent::SetLocalScale(const glm::vec2& scale)
{
	m_LocalScale = scale;
	SetDirty();
}

void Engine::TransformComponent::SetLocalScale(float x, float y)
{
	SetLocalScale(glm::vec2{x, y});
}

void Engine::TransformComponent::SetDirty()
{

	auto children{ GetOwner()->GetChildren() };
	for(GameObject* child : children)
	{
		child->GetTransform()->SetDirty();
	}

	m_LocalTransformIsDirty = true;
	m_WorldTransformIsDirty = true;
}

const glm::vec2& Engine::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

float Engine::TransformComponent::GetLocalRotation() const
{
	return m_LocalRotation;
}

const glm::vec2& Engine::TransformComponent::GetLocalScale() const
{
	return m_LocalScale;
}

const glm::vec2& Engine::TransformComponent::GetPosition()
{
	GetWorldTransform();  // Makes sure its updated
	return m_WorldPosition;
}

const float Engine::TransformComponent::GetRotation()
{
	GetWorldTransform();  // Makes sure its updated
	return m_WorldRotation;
}

const glm::vec2& Engine::TransformComponent::GetScale()
{
	GetWorldTransform();  // Makes sure its updated
	return m_WorldScale;
}

const glm::mat4& Engine::TransformComponent::GetWorldTransform()
{
	if(m_WorldTransformIsDirty)
	{
		UpdateWorldTransform();
		m_WorldTransformIsDirty = false;
	}

	return m_WorldTransform;
}

const glm::mat4& Engine::TransformComponent::GetLocalTransform()
{
	if(m_LocalTransformIsDirty)
	{
		UpdateLocalTransform();
		m_LocalTransformIsDirty = false;
	}
	return m_LocalTransform;
}

void Engine::TransformComponent::UpdateLocalTransform()
{
	m_LocalTransform = glm::mat4(1.0);
	m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(m_LocalPosition, 0.f));
	m_LocalTransform = glm::rotate(m_LocalTransform, glm::radians(m_LocalRotation), glm::vec3(0.f, 0.f, 1.0f));  // S
	m_LocalTransform = glm::scale(m_LocalTransform, glm::vec3(m_LocalScale, 0.f));
}

void Engine::TransformComponent::UpdateWorldTransform()
{
	GameObject* owner{ GetOwner()->GetParent() };

	if(owner)
	{
		m_WorldTransform = owner->GetTransform()->GetWorldTransform();
		auto localTransform = GetLocalTransform();
		m_WorldTransform *= localTransform;
	}
	else
	{
		m_WorldTransform = GetLocalTransform();
	}

	m_WorldPosition = glm::vec2{ m_WorldTransform[3][0], m_WorldTransform[3][1] };
	m_WorldScale = { glm::length(m_WorldTransform[0]), glm::length(m_WorldTransform[1]) };
	m_WorldRotation =  glm::degrees(glm::eulerAngles(glm::quat_cast(m_WorldTransform)).z);

}
