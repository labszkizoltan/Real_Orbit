
#ifndef DUAL_OCT_TREE_H
#define DUAL_OCT_TREE_H

#include <utils/OctTree.h>
#include <core/scene/Scene.h>
#include <thread>

// typedef void(*EntityAdderFunction)(Scene* scene);
using EntityAdderFunction = std::function<void(Scene*)>;
// using EntitySelecterFunction = std::function<entt::basic_view<entt::entity, >(Scene*)>;

class DualOctTree
{
public:
	DualOctTree();
	DualOctTree(Box3D box);
	~DualOctTree();

	void SetEntityAdderFunction(EntityAdderFunction function);
	OctTree<entt::entity>* GetActiveTree();

	void Update(Scene* scene);
	void SwapTrees();

	template<class... Components>
	void Update(Scene* scene);

	void BuildTree(Scene* scene);
	bool IsBuilding();

private:
	void AddEntities(Scene* scene);

public:
//private:
	OctTree<entt::entity> m_OctTree_1;
	OctTree<entt::entity> m_OctTree_2;
	OctTree<entt::entity>* m_ActiveTree = nullptr;
	OctTree<entt::entity>* m_BuildTree = nullptr;
	bool m_IsBuilding = false;
	std::unique_ptr<std::thread> m_WorkerThread = nullptr;
	EntityAdderFunction m_EntityAdder;
//	EntitySelecterFunction m_EntitySelecter;
//https://stackoverflow.com/questions/30560949/how-to-call-join-method-of-one-thread-in-another-function-in-c

};



template<class... Components>
void DualOctTree::Update(Scene* scene)
{
	// this means that the building has completed
	if (!m_IsBuilding)
	{
		if (m_WorkerThread != nullptr)
			m_WorkerThread->join();
		SwapTrees();
		
		// BuildTree(scene):
		{
			m_IsBuilding = true;
			m_BuildTree->Clear();

			m_WorkerThread = std::make_unique<std::thread>([scene, this]() {
				auto entities = scene->m_Registry.view<TransformComponent, Components...>();
				for (auto entity : entities)
				{
					TransformComponent& entityTrf = entities.get<TransformComponent>(entity);
					// if (scene->m_Registry.valid(asteroid))
					m_BuildTree->Insert(entityTrf.location, entity);
				}
				m_IsBuilding = false;
			});
		}
	}
}




#endif DUAL_OCT_TREE_H
