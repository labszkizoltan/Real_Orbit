
#ifndef ENTITY_BOUND_PLAYER_H
#define ENTITY_BOUND_PLAYER_H

#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>
#include <core/scene/Scene.h>

class EntityBoundPlayer
{
public:
	EntityBoundPlayer() = default;
	EntityBoundPlayer(entt::entity entity);
	~EntityBoundPlayer() = default;

	void SetEntity(entt::entity entity);
	void Update(Timestep ts, Vec3D acceleration);
	void DrawStatsOnScreen();

	void TakePickUp(std::shared_ptr<Scene> scene, float range, float amount);
	void FillReserves(Timestep ts);

private:
	Vec3D CalculateColour(float ratio);

public:
	entt::entity m_Entity = entt::tombstone;
};

#endif // ENTITY_BOUND_PLAYER_H
