
#include "SceneUpdater.h"
#include <utils/OctTree.h>

SceneUpdater::SceneUpdater()
{
}

SceneUpdater::SceneUpdater(std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{
}

SceneUpdater::~SceneUpdater()
{
}

void SceneUpdater::UpdateScene(Timestep ts)
{
	m_Scene->m_MeshLibrary.Clear();


	auto timed_entities = m_Scene->m_Registry.view<TimerComponent>();
	for (auto entity : timed_entities)
	{
		TimerComponent& ttl = timed_entities.get<TimerComponent>(entity);
		ttl.timeToLive -= ts;
		if (ttl.timeToLive < 0.0f)
			m_Scene->m_Registry.destroy(entity);
	}

	auto missiles = m_Scene->m_Registry.view<TargetComponent, TransformComponent, DynamicPropertiesComponent>();
	for (auto missile : missiles)
	{
		TargetComponent& target = missiles.get<TargetComponent>(missile);
		if (m_Scene->m_Registry.valid(target.targetEntity))
		{
			TransformComponent& missileTrf = missiles.get<TransformComponent>(missile);
			DynamicPropertiesComponent& missileVelocity = missiles.get<DynamicPropertiesComponent>(missile);
			TransformComponent& targetLoc = m_Scene->m_Registry.get<TransformComponent>(target.targetEntity);
			DynamicPropertiesComponent& targetVelocity = m_Scene->m_Registry.get<DynamicPropertiesComponent>(target.targetEntity);

			Vec3D dx = targetLoc.location - missileTrf.location;
			Vec3D dv = targetVelocity.velocity - missileVelocity.velocity;
			float dt = sqrt(dx.lengthSquare() / dv.lengthSquare());
			dx += dt * dv;
			float accel = 0.000005f;

			missileVelocity.velocity += accel * ts * dx / dx.length();
		}
		else
		{
			// this always returns false according the documentation: registry.valid(entt::tombstone);
			target.targetEntity = entt::tombstone;
		}
	}

	auto grav_masses = m_Scene->m_Registry.view<TransformComponent, GravitationalMassComponent>();
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent>();
	for (auto entity : view)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(entity);
		DynamicPropertiesComponent& dyn_prop = view.get<DynamicPropertiesComponent>(entity);

		entity_trf.location += ts * dyn_prop.velocity;
		entity_trf.orientation = Rotation(ts*dyn_prop.angular_velocity.length(), dyn_prop.angular_velocity) * entity_trf.orientation;

		// this is not that great, should be separated cus meshes wo dyn props wont be drawn (they dont get into the mesh library)
		MeshIndexComponent& idx = view.get<MeshIndexComponent>(entity);
		m_Scene->m_MeshLibrary.m_MeshTransforms[idx.idx].push_back(entity_trf);

		Vec3D acceleration = {0,0,0};
		for (auto grav_entity : grav_masses)
		{
			if (entity != grav_entity)
			{
				GravitationalMassComponent& grav_mass = grav_masses.get<GravitationalMassComponent>(grav_entity);
				TransformComponent& grav_trf = grav_masses.get<TransformComponent>(grav_entity);
				Vec3D dr = grav_trf.location - entity_trf.location;
				acceleration += grav_mass.gravitational_mass * dr / pow(dr.length(), 3); // a gravitatonal constant scalar could be added here
			}
		}

		dyn_prop.velocity += ts * acceleration;
	}

	/*
	Box3D box; box.center = Vec3D(0, 0, 0); box.radius = Vec3D(100, 100, 100);
	OctTree<Vec3D> octtree(box);

	auto grav_masses = m_Scene->m_Registry.view<TransformComponent, GravitationalMassComponent>();
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent>();
	for (auto entity = view.begin(); entity != view.end(); entity++)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(*entity);
		DynamicPropertiesComponent& dyn_prop = view.get<DynamicPropertiesComponent>(*entity);

		octtree.Insert(entity_trf.location, entity_trf.location);

		entity_trf.location += ts * dyn_prop.velocity;
		entity_trf.orientation = Rotation(ts * dyn_prop.angular_velocity.length(), dyn_prop.angular_velocity) * entity_trf.orientation;

		Vec3D acceleration = { 0,0,0 };
		for (auto grav_entity : grav_masses)
		{
			if (*entity != grav_entity)
			{
				GravitationalMassComponent& grav_mass = grav_masses.get<GravitationalMassComponent>(grav_entity);
				TransformComponent& grav_trf = grav_masses.get<TransformComponent>(grav_entity);
				Vec3D dr = grav_trf.location - entity_trf.location;
				acceleration += grav_mass.gravitational_mass * dr / pow(dr.length(), 3); // a gravitatonal constant scalar could be added here
			}
		}

		dyn_prop.velocity += ts * acceleration;

		// this technically works, but the framerate drops noticeably even in release mode, with just 4 entities
//		auto entity_next = entity;
//		entity_next++;
//		for (entity_next; entity_next != view.end(); entity_next++)
//		{
//			TransformComponent& next_trf = view.get<TransformComponent>(*entity_next);
//			DynamicPropertiesComponent& next_dyn_prop = view.get<DynamicPropertiesComponent>(*entity_next);
//
//			// this is the line that causes the frame rate drop
//			std::cout << next_trf.location - entity_trf.location << "\n";
//		}
	}
	*/

	std::cout << "Scene updated\n";

}
