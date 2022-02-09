
#include "DualOctTree.h"
#include <thread>
#include <functional>

#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>

DualOctTree::DualOctTree()
	: m_OctTree_1(Box3D()), m_OctTree_2(Box3D())
{
	m_ActiveTree = &m_OctTree_1;
	m_BuildTree = &m_OctTree_2;
	m_IsBuilding = false;
	m_WorkerThread = nullptr;
	m_EntityAdder = [this](Scene* scene) {
		// add only the asteroids
		auto asteroids = scene->m_Registry.view<TransformComponent, AsteroidComponent>();
		for (auto asteroid : asteroids)
		{
			TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
			// if (scene->m_Registry.valid(asteroid))
				m_BuildTree->Insert(asteroidTrf.location, asteroid);
		}
		m_IsBuilding = false;
	};
}

DualOctTree::DualOctTree(Box3D box)
	: m_OctTree_1(box), m_OctTree_2(box)
{
	m_ActiveTree = &m_OctTree_1;
	m_BuildTree = &m_OctTree_2;
	m_IsBuilding = false;
	m_WorkerThread = nullptr;
	m_EntityAdder = [this](Scene* scene) {
		// add only the asteroids
		auto asteroids = scene->m_Registry.view<TransformComponent, AsteroidComponent>();
		for (auto asteroid : asteroids)
		{
			TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
			// if (scene->m_Registry.valid(asteroid))
				m_BuildTree->Insert(asteroidTrf.location, asteroid);
		}
		m_IsBuilding = false;
	};
}

DualOctTree::~DualOctTree()
{
	if(m_WorkerThread != nullptr)
	 	m_WorkerThread->join();
}

void DualOctTree::SetEntityAdderFunction(EntityAdderFunction function)
{
	m_EntityAdder = function;
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
	m_ActiveTree = m_BuildTree;
	m_BuildTree = temp;
}

void DualOctTree::BuildTree(Scene* scene)
{
	m_IsBuilding = true;
	m_BuildTree->Clear();

	m_WorkerThread = std::make_unique<std::thread>(m_EntityAdder, scene);
	// m_WorkerThread = std::make_unique<std::thread>(&DualOctTree::AddEntities, this, scene);
}

bool DualOctTree::IsBuilding()
{
	return m_IsBuilding;
}

void DualOctTree::AddEntities(Scene* scene)
{
	// auto start = std::chrono::high_resolution_clock::now(); 
	
	// add only the asteroids
	auto asteroids = scene->m_Registry.view<TransformComponent, AsteroidComponent>();
	for (auto asteroid : asteroids)
	{
		TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
		if (scene->m_Registry.valid(asteroid))
			m_BuildTree->Insert(asteroidTrf.location, asteroid);
	}

	// add everything with a transform component
	/*
	auto asteroids = scene->m_Registry.view<TransformComponent>();
	for (auto asteroid : asteroids)
	{
		TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
		if (scene->m_Registry.valid(asteroid))
			m_BuildTree->Insert(asteroidTrf.location, asteroid);
	}
	*/

	/*
	auto finish = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
	std::cout << "DualOctTree::AddEntities took (microsec):" << duration.count() << "\n";
	*/

	// this function builds the tree in ~500-1000 microsec when the asteroid count
	// is maxed out, around 1200-1400

	m_IsBuilding = false;
}
