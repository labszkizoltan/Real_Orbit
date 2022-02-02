
#ifndef DUAL_OCT_TREE_H
#define DUAL_OCT_TREE_H

#include <utils/OctTree.h>
#include <core/scene/Scene.h>
#include <thread>

class DualOctTree
{
public:
	DualOctTree() = default;
	DualOctTree(Box3D box);
	~DualOctTree();

	OctTree<entt::entity>* GetActiveTree();

	void Update(Scene* scene);
	void SwapTrees();

	void BuildTree(Scene* scene);
	bool IsBuilding();

private:
	void AddEntities(Scene* scene);

private:
	OctTree<entt::entity> m_OctTree_1;
	OctTree<entt::entity> m_OctTree_2;
	OctTree<entt::entity>* m_ActiveTree = nullptr;
	OctTree<entt::entity>* m_BuildTree = nullptr;
	bool m_IsBuilding = false;
	std::unique_ptr<std::thread> m_WorkerThread = nullptr;
//https://stackoverflow.com/questions/30560949/how-to-call-join-method-of-one-thread-in-another-function-in-c

};

#endif DUAL_OCT_TREE_H
