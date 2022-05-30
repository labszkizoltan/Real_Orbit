
#include "UnitControl.h"
#include "Game_v1/Components/GameComponents.h"
#include <Game_v1/Layers/04_MarsMission_layer.h>


//std::shared_ptr<Scene> UnitController::s_Scene = nullptr;
//std::shared_ptr<DualOctTree> UnitController::s_MissillesOctTree = nullptr;
// std::vector<entt::entity> UnitController::s_UnitVicinity;

/*
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
	// s_MissillesOctTree = tree;
	// m_MissillesOctTree = tree;
}
*/

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

void UnitController::MoveUnits(Timestep ts)
{
	const float maxVel = 0.00001f;
	const float accel = 0.000002f;
	const float waypointRemovalRangeSqr = 10*10;
	const float ccRemovelSpeedSqr = 0.0000001f;
	auto controlledUnits = m_Layer->m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, ControllComponent>();
	for (auto unit : controlledUnits)
	{
		TransformComponent& trf = controlledUnits.get<TransformComponent>(unit);
		DynamicPropertiesComponent& vel = controlledUnits.get<DynamicPropertiesComponent>(unit);
		ControllComponent& wp = controlledUnits.get<ControllComponent>(unit);
		
		if (wp.waypoints.size() > 0)
			if ((wp.waypoints[wp.waypoints.size() - 1] - trf.location).lengthSquare() < waypointRemovalRangeSqr)
				wp.waypoints.pop_back();


		// when there are no more waypoints, stop, or even remove the ControllComponent
		if (wp.waypoints.size() == 0)
		{
			if(vel.velocity.lengthSquare() > 0.0f)
				vel.velocity *= (vel.velocity.length() - ts * accel) / vel.velocity.length();
			
			if (vel.velocity.lengthSquare() < ccRemovelSpeedSqr)
				m_Layer->m_Scene->m_Registry.remove<ControllComponent>(unit);
		}
		else
		{
			Vec3D nextWayPoint = wp.waypoints[wp.waypoints.size() - 1];
			Vec3D dx = nextWayPoint - trf.location;

			Vec3D v_ortho = vel.velocity - dx * (dx * vel.velocity)/dx.lengthSquare();
			v_ortho.normalize();

			float pathLen = dx.length();
			float stopLen = vel.velocity * vel.velocity / (2 * accel);// +dx.length();

			Vec3D ve = dx * (1 - stopLen / pathLen); // an extra velocity addon, to close the distance from the target
			Vec3D dv = ve - vel.velocity;

			vel.velocity += (accel * ts / dv.length()) * dv;
			vel.velocity += -ts * accel * v_ortho;
		}
	}

}

// previous trial versions of MoveUnits()
/*
void UnitController::MoveUnits(Timestep ts)
{
	const float maxVel = 0.00001f;
	const float accel = 0.000002f;
	const float waypointRemovalRangeSqr = 4*4;
	auto controlledUnits = m_Layer->m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, ControllComponent>();
	for (auto unit : controlledUnits)
	{
		TransformComponent& trf = controlledUnits.get<TransformComponent>(unit);
		DynamicPropertiesComponent& vel = controlledUnits.get<DynamicPropertiesComponent>(unit);
		ControllComponent& wp = controlledUnits.get<ControllComponent>(unit);

		if (wp.waypoints.size() > 0)
			if ((wp.waypoints[wp.waypoints.size() - 1] - trf.location).lengthSquare() < waypointRemovalRangeSqr)
				wp.waypoints.pop_back();


		// when there are no more waypoints, stop
		if (wp.waypoints.size() == 0)
		{
			if(vel.velocity.lengthSquare() > 0.0f)
				vel.velocity *= (vel.velocity.length() - ts * accel) / vel.velocity.length();
			// vel.velocity += -1.0f * ts * accel * vel.velocity / vel.velocity.length();
		}
		else if (wp.waypoints.size() == 1)
		{
			Vec3D nextWayPoint = wp.waypoints[0];
			Vec3D dx = nextWayPoint - trf.location;
			float pathLen = dx.length();
			dx.normalize();

			Vec3D v_ortho = vel.velocity - dx * (dx * vel.velocity);
			v_ortho.normalize();
			Vec3D dv = Vec3D();

			float stopLen = vel.velocity * vel.velocity / (2 * accel);

			if ((stopLen * 1.1f) > pathLen)
				dv = vel.velocity;
			else
				dv = -1 * vel.velocity;

			dv.normalize();
			dv = dv - v_ortho + dx;
			dv.normalize();
			vel.velocity += accel * ts * dv;

//			if (dx * vel.velocity > 0)
//				vel.velocity += 0.75f * ts * accel * dx;
//			else
//				vel.velocity += ts * accel * dx;
		}
		else
		{
			Vec3D nextWayPoint = wp.waypoints[wp.waypoints.size() - 1];
			Vec3D nextNextWayPoint = wp.waypoints[wp.waypoints.size() - 2];
			Vec3D dw = nextNextWayPoint - nextWayPoint;
			Vec3D dx = nextWayPoint - trf.location;

			// float pathLen = wp.GetPathLength();
			// float pathLen = dw.length()+dx.length();
			float pathLen = dx.length();
			float stopLen = vel.velocity * vel.velocity / (2 * accel);// +dx.length();

			dx.normalize();
			Vec3D v_ortho = vel.velocity - dx * (dx * vel.velocity);
			v_ortho.normalize();
			Vec3D dv = Vec3D();

			if ((stopLen * 1.1f) > pathLen)
				// dv = -1 * vel.velocity;
				dv = -1 * dx;
			else
				// dv = vel.velocity;
				dv = dx;

			dv.normalize();
//			dv = dv - v_ortho + dx;
			dv = dv - 0.1f*v_ortho;
			dv.normalize();
			vel.velocity += accel * ts * dv;

//			if (dx * vel.velocity > 0)
//				vel.velocity += 0.75f * ts * accel * dx / dx.length();
//			else
//				vel.velocity += ts * accel * dx / dx.length();
		}
	}

}
*/

entt::entity UnitController::GetClosestTargetFromTeam(const Vec3D& acquisitionLocation, const float acquisitionRange, const int teamNumber)
{
	static std::vector<entt::entity> entities; entities.clear();
	Box3D box(acquisitionLocation, Vec3D(acquisitionRange, acquisitionRange, acquisitionRange));
	m_Layer->m_CollidersOctTree->GetActiveTree()->QueryRange(box, entities, 0);

	entt::entity result = entt::null;
	float closestDistSqr = acquisitionRange * acquisitionRange;
	for (auto entity : entities)
	{
		if (!m_Layer->m_Scene->m_Registry.valid(entity))
			continue;

		// if the entities team component doesnt match the requested team number, than dont do anything
		if (teamNumber == 0 && !m_Layer->m_Scene->m_Registry.all_of<TeamComponent_0>(entity)) { continue; }
		if (teamNumber == 1 && !m_Layer->m_Scene->m_Registry.all_of<TeamComponent_1>(entity)) { continue; }
		if (teamNumber == 2 && !m_Layer->m_Scene->m_Registry.all_of<TeamComponent_2>(entity)) { continue; }
		if (teamNumber == 3 && !m_Layer->m_Scene->m_Registry.all_of<TeamComponent_3>(entity)) { continue; }
		if (teamNumber == 4 && !m_Layer->m_Scene->m_Registry.all_of<TeamComponent_4>(entity)) { continue; }
		if (teamNumber == 5 && !m_Layer->m_Scene->m_Registry.all_of<TeamComponent_5>(entity)) { continue; }
		if (teamNumber == 6 && !m_Layer->m_Scene->m_Registry.all_of<TeamComponent_6>(entity)) { continue; }
		if (teamNumber == 7 && !m_Layer->m_Scene->m_Registry.all_of<TeamComponent_7>(entity)) { continue; }

		TransformComponent& entity_trf = m_Layer->m_Scene->m_Registry.get<TransformComponent>(entity);
		Vec3D dx = entity_trf.location - acquisitionLocation;

		if (dx.lengthSquare() < closestDistSqr)
		{
			closestDistSqr = dx.lengthSquare();
			result = entity;
		}
	}

	return result;
}
