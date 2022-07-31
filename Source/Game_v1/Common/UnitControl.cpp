
#include "UnitControl.h"
#include "Game_v1/Components/GameComponents.h"
#include <Game_v1/Layers/04_MarsMission_layer.h>
#include <Game_v1/Common/EntityManager.h>
#include <utils/RandomGeneration.h>


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
	auto controlledUnits = m_Layer->m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MovementControllComponent>();
	for (auto unit : controlledUnits)
	{
		TransformComponent& trf = controlledUnits.get<TransformComponent>(unit);
		DynamicPropertiesComponent& vel = controlledUnits.get<DynamicPropertiesComponent>(unit);
		MovementControllComponent& wp = controlledUnits.get<MovementControllComponent>(unit);
		
		if (wp.waypoints.size() > 0)
			if ((wp.waypoints[wp.waypoints.size() - 1] - trf.location).lengthSquare() < waypointRemovalRangeSqr)
				wp.waypoints.pop_back();


		// when there are no more waypoints, stop, or even remove the MovementControllComponent
		if (wp.waypoints.size() == 0)
		{
			if(vel.velocity.lengthSquare() > 0.0f)
				vel.velocity *= (vel.velocity.length() - ts * accel) / vel.velocity.length();
			
			if (vel.velocity.lengthSquare() < ccRemovelSpeedSqr)
				m_Layer->m_Scene->m_Registry.remove<MovementControllComponent>(unit);
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

		// // turn the nuit into direction
		// const float turn_rate = 0.01f;
		// Vec3D rot_axis = CrossProduct(trf.orientation.f3, vel.velocity);
		// trf.orientation = trf.orientation * Rotation(std::min(turn_rate * ts, Vec3D::angle(trf.orientation.f3, vel.velocity)), rot_axis);
	}
}

void UnitController::FireControl(Timestep ts)
{
	static int blueIdx = m_Layer->m_Scene->GetMeshLibrary().m_NameIndexLookup["BlueSphere"];
	static int yellowIdx = m_Layer->m_Scene->GetMeshLibrary().m_NameIndexLookup["YellowSphere"];

	auto team0_ships = m_Layer->m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, WeaponControllComponent, TeamComponent_0>();
	auto team1_ships = m_Layer->m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, WeaponControllComponent, TeamComponent_1>();

	const float bulletSpeed = 0.6f; // 0.2f

	static std::vector<entt::entity> outer_vicinity;
	static std::vector<entt::entity> ship_vicinity;

	// team green fire missilles
	const int missille_fire_range = 1500;
	const int outer_missille_fire_range = 2000;
	for (auto ship : team0_ships)
	{
		TransformComponent& shipTrf = team0_ships.get<TransformComponent>(ship);
		DynamicPropertiesComponent& shipVel = team0_ships.get<DynamicPropertiesComponent>(ship);
		WeaponControllComponent& wcc = team0_ships.get<WeaponControllComponent>(ship);
		Box3D box; box.center = shipTrf.location; box.radius = Vec3D(missille_fire_range, missille_fire_range, missille_fire_range);
		ship_vicinity.clear(); outer_vicinity.clear();
		m_Layer->m_Team1_Tree->GetActiveTree()->QueryRange(box, ship_vicinity, 0);
		box.radius = Vec3D(outer_missille_fire_range, outer_missille_fire_range, outer_missille_fire_range);
		m_Layer->m_Team1_Tree->GetActiveTree()->QueryRange(box, outer_vicinity, 0);

		if (ship_vicinity.size() > 0)
		{
			int tgt_idx = std::rand() % outer_vicinity.size();
			if(wcc.missilleShotsRemaining > 0) // && m_Layer->m_Scene->m_Registry.valid(ship_vicinity[tgt_idx]))
			{
				m_Layer->m_EntityManager.LaunchMissile(blueIdx, shipTrf, shipVel, outer_vicinity[tgt_idx]);
				wcc.missilleShotsRemaining--;
			}
		}
	}

	// team red fire missilles
	for (auto ship : team1_ships)
	{
		TransformComponent& shipTrf = team1_ships.get<TransformComponent>(ship);
		DynamicPropertiesComponent& shipVel = team1_ships.get<DynamicPropertiesComponent>(ship);
		WeaponControllComponent& wcc = team1_ships.get<WeaponControllComponent>(ship);
		Box3D box; box.center = shipTrf.location; box.radius = Vec3D(missille_fire_range, missille_fire_range, missille_fire_range);
		ship_vicinity.clear(); outer_vicinity.clear();
		m_Layer->m_Team0_Tree->GetActiveTree()->QueryRange(box, ship_vicinity, 0);
		box.radius = Vec3D(outer_missille_fire_range, outer_missille_fire_range, outer_missille_fire_range);
		m_Layer->m_Team0_Tree->GetActiveTree()->QueryRange(box, outer_vicinity, 0);

		if (ship_vicinity.size() > 0)
		{
			int tgt_idx = std::rand() % outer_vicinity.size();
			if (wcc.missilleShotsRemaining > 0) // && m_Layer->m_Scene->m_Registry.valid(ship_vicinity[tgt_idx]))
			{
				m_Layer->m_EntityManager.LaunchMissile(yellowIdx, shipTrf, shipVel, outer_vicinity[tgt_idx]);
				wcc.missilleShotsRemaining--;
			}
		}
	}

	// team green fire bullets
	const int bullet_fire_range = 1000;
	for (auto ship : team0_ships)
	{
		TransformComponent& shipTrf = team0_ships.get<TransformComponent>(ship);
		DynamicPropertiesComponent& shipVel = team0_ships.get<DynamicPropertiesComponent>(ship);
		WeaponControllComponent& wcc = team0_ships.get<WeaponControllComponent>(ship);
		Box3D box; box.center = shipTrf.location; box.radius = Vec3D(bullet_fire_range, bullet_fire_range, bullet_fire_range);
		ship_vicinity.clear();
		m_Layer->m_Team1_Tree->GetActiveTree()->QueryRange(box, ship_vicinity, 0);

		if (ship_vicinity.size() > 0)
		{
			if (!m_Layer->m_Scene->m_Registry.valid(ship_vicinity[0]))
				continue;

			if (wcc.gunShotsRemaining > 0)
			{
				TransformComponent& targetTrf = team1_ships.get<TransformComponent>(ship_vicinity[0]);
				DynamicPropertiesComponent& targetVel = team1_ships.get<DynamicPropertiesComponent>(ship_vicinity[0]);
				float t0 = (targetTrf.location - shipTrf.location).length() / bulletSpeed;
				Vec3D futureLoc = targetTrf.location + t0 * targetVel.velocity;
				// I think this is not exact, because the firing ships velocity is not considered,
				// but close enough when the bullet speed is well over the ships velocity
				Vec3D bulletVel = futureLoc - shipTrf.location; bulletVel.normalize();

				TransformComponent bulletStartLoc = TransformComponent();
				bulletStartLoc.location = shipTrf.location + (bulletVel * 1.5f * shipTrf.scale);
				m_Layer->m_EntityManager.ShootBullett(bulletStartLoc, bulletVel* bulletSpeed, false);
				wcc.gunShotsRemaining--;
			}
		}
	}

	// team red fire bullets
	for (auto ship : team1_ships)
	{
		TransformComponent& shipTrf = team1_ships.get<TransformComponent>(ship);
		DynamicPropertiesComponent& shipVel = team1_ships.get<DynamicPropertiesComponent>(ship);
		WeaponControllComponent& wcc = team1_ships.get<WeaponControllComponent>(ship);
		Box3D box; box.center = shipTrf.location; box.radius = Vec3D(bullet_fire_range, bullet_fire_range, bullet_fire_range);
		ship_vicinity.clear();
		m_Layer->m_Team0_Tree->GetActiveTree()->QueryRange(box, ship_vicinity, 0);

		if (ship_vicinity.size() > 0)
		{
			if (!m_Layer->m_Scene->m_Registry.valid(ship_vicinity[0]))
				continue;

			if (wcc.gunShotsRemaining > 0)
			{
				TransformComponent& targetTrf = team0_ships.get<TransformComponent>(ship_vicinity[0]);
				DynamicPropertiesComponent& targetVel = team0_ships.get<DynamicPropertiesComponent>(ship_vicinity[0]);
				float t0 = (targetTrf.location - shipTrf.location).length() / bulletSpeed;
				Vec3D futureLoc = targetTrf.location + t0 * targetVel.velocity;
				// I think this is not exact, because the firing ships velocity is not considered,
				// but close enough when the bullet speed is well over the ships velocity
				Vec3D bulletVel = futureLoc - shipTrf.location; bulletVel.normalize();

				TransformComponent bulletStartLoc = TransformComponent();
				bulletStartLoc.location = shipTrf.location + (bulletVel * 1.5f * shipTrf.scale);
				m_Layer->m_EntityManager.ShootBullett(bulletStartLoc, bulletVel * bulletSpeed, false);
				wcc.gunShotsRemaining--;
			}
		}
	}

}

void UnitController::UpdateWeaponControls(Timestep ts)
{
	auto weponEntities = m_Layer->m_Scene->m_Registry.view<WeaponControllComponent>();
	for (auto entity : weponEntities)
	{
		WeaponControllComponent& wcc = weponEntities.get<WeaponControllComponent>(entity);

		// update bullets
		if (wcc.gunShotsRemaining <= 0)
			wcc.gunShotTimerRemaining -= ts;

		if (wcc.gunShotTimerRemaining <= 0)
		{
			wcc.gunShotTimerRemaining = wcc.gunShotTimer + RORNG::runif()*10.0f;
			wcc.gunShotsRemaining = wcc.gunShots;
		}

		// update missilles
		if (wcc.missilleShotsRemaining <= 0)
			wcc.missilleShotTimerRemaining -= ts;

		if (wcc.missilleShotTimerRemaining <= 0)
		{
			wcc.missilleShotTimerRemaining = wcc.missilleShotTimer;
			wcc.missilleShotsRemaining = wcc.missilleShots;
		}

	}

}




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
