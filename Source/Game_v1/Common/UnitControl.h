
#ifndef UNIT_CONTROL_H
#define UNIT_CONTROL_H

#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <Game_v1/Common/DualOctTree.h>

// I'd like to make a class that controls the NPC units, like moving,
// firing at enemy targets or missilles

class UnitController
{
	UnitController();
	UnitController(entt::entity entity);
	~UnitController() = default;

	void SetMissilleOctTree(std::shared_ptr<DualOctTree> tree);

	void Update();

private:
	void Accelerate();
	void FendOffMissille();
	// void AvertMissille();


private:
	entt::entity m_Entity;

	static std::shared_ptr<Scene> s_Scene;
	static std::shared_ptr<DualOctTree> s_MissillesOctTree;
	static std::vector<entt::entity> s_UnitVicinity;
};






#endif // UNIT_CONTROL_H
