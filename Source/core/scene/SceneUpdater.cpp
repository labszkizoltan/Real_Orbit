
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
	/*
	auto grav_masses = m_Scene->m_Registry.view<TransformComponent, GravitationalMassComponent>();
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent>();
	for (auto entity : view)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(entity);
		DynamicPropertiesComponent& dyn_prop = view.get<DynamicPropertiesComponent>(entity);

		entity_trf.location += ts * dyn_prop.velocity;
		entity_trf.orientation = Rotation(ts*dyn_prop.angular_velocity.length(), dyn_prop.angular_velocity) * entity_trf.orientation;

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
	*/

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
