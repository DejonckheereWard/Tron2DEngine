#include "NavmeshComponent.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "GameObject.h"
#include "NavmeshManager.h"

void NavmeshComponent::Init()
{
	m_pNavmeshManager = &NavmeshManager::GetInstance();
}

void NavmeshComponent::Update()
{
}

void NavmeshComponent::Render() const
{
	m_pNavmeshManager->RenderMesh();
}

//void NavmeshComponent::GenerateNavmesh()
//{
//    Engine::CollisionManager& collisionManager{	Engine::CollisionManager::GetInstance() };
//
//}
