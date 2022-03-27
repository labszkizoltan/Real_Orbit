#include "UnitControl.h"

std::shared_ptr<Scene> UnitController::s_Scene = nullptr;
std::shared_ptr<DualOctTree> UnitController::s_MissillesOctTree = nullptr;
std::vector<entt::entity> UnitController::s_UnitVicinity;

UnitController::UnitController()
	: m_Entity(entt::tombstone)
{
}

UnitController::UnitController(entt::entity entity)
	: m_Entity(entity)
{
}

void UnitController::SetMissilleOctTree(std::shared_ptr<DualOctTree> tree)
{
	s_MissillesOctTree = tree;
}

void UnitController::Update()
{
}

void UnitController::Accelerate()
{
}

void UnitController::FendOffMissille()
{
	/*
	s_UnitVicinity.clear();
	TransformComponent trf = 

	Box3D box; box.center = missileTrf.location; box.radius = Vec3D(10, 10, 10);
	missille_vicinity.clear();
	m_CollidersOctTree->GetActiveTree()->QueryRange(box, s_UnitVicinity, 0)
	
	*/
}
