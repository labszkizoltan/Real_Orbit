
#ifndef UNIT_CONTROL_H
#define UNIT_CONTROL_H

// #include <core/Timestep.h>
#include <core/scene/Scene.h>
#include <core/scene/Entity.h>
#include <Game_v1/Common/DualOctTree.h>
// #include <Game_v1/Layers/04_MarsMission_layer.h>

// I'd like to make a class that controls the NPC units, like moving,
// firing at enemy targets or missilles

class MarsMission_layer;

class UnitController
{
public:
	UnitController() = default;
	~UnitController() = default;

	void Update();

// private:
	void Accelerate();
	void FendOffMissille();
	void MoveUnits(Timestep ts);
	void FireControl(Timestep ts);
	void UpdateWeaponControls(Timestep ts);

	// void AvertMissille();

	entt::entity GetClosestTargetFromTeam(const Vec3D& acquisitionLocation, const float acquisitionRange, const int teamNumber);


public:
	MarsMission_layer* m_Layer = nullptr;

};






#endif // UNIT_CONTROL_H
