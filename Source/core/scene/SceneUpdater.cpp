
#include "SceneUpdater.h"


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



}
