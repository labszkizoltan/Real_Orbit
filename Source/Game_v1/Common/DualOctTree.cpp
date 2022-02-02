
#include "DualOctTree.h"
#include <thread>

#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>

DualOctTree::DualOctTree(Box3D box)
	: m_OctTree_1(box), m_OctTree_2(box)
{
	m_ActiveTree = &m_OctTree_1;
	m_BuildTree = &m_OctTree_2;
	m_IsBuilding = false;
	m_WorkerThread = nullptr;
}

DualOctTree::~DualOctTree()
{
	m_WorkerThread->join();
}

OctTree<entt::entity>* DualOctTree::GetActiveTree()
{
	return m_ActiveTree;
}

void DualOctTree::Update(Scene* scene)
{
	// this means that the building has completed
	if (!m_IsBuilding)
	{
		if(m_WorkerThread != nullptr)
			m_WorkerThread->join();
		SwapTrees();
		BuildTree(scene);
	}
}

void DualOctTree::SwapTrees()
{
	OctTree<entt::entity>* temp = m_ActiveTree;
	OctTree<entt::entity>* m_ActiveTree = m_BuildTree;
	OctTree<entt::entity>* m_BuildTree = temp;
}

void DualOctTree::BuildTree(Scene* scene)
{
	m_IsBuilding = true;
	m_BuildTree->Clear();

	m_WorkerThread = std::make_unique<std::thread>(&DualOctTree::AddEntities, this, scene);
}

bool DualOctTree::IsBuilding()
{
	return m_IsBuilding;
}

void DualOctTree::AddEntities(Scene* scene)
{
	auto asteroids = scene->m_Registry.view<TransformComponent, AsteroidComponent>();
	for (auto asteroid : asteroids)
	{
		TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
		m_BuildTree->Insert(asteroidTrf.location, asteroid);
	}

	m_IsBuilding = false;
}
