
#include "EntityManager.h"

EntityManager::EntityManager(Scene* scene)
	: m_Scene(scene) {}

void EntityManager::SetScene(Scene* scene)
{
	m_Scene = scene;
}

void EntityManager::EmitMesh(int meshIdx, TransformComponent transform)
{
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 0.001f;
	dynProps.velocity = 0.05f * transform.orientation.f3;
	dynProps.angular_velocity = Vec3D();
	transform.scale = 0.1f; // 0.02f;
	transform.location += 0.1 * (transform.orientation.f3 - transform.orientation.f2);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(meshIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(10000.0f)); // provide ttl in mili seconds
	newEntity.AddComponent<HitPointComponent>(1.0f);
}

void EntityManager::SpawnAsteroid(Vec3D center, Vec3D velocity, float spread)
{
	static int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DeformedSphere"];

	TransformComponent transform;
	transform.location = center + Vec3D(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500) * spread / 1000.0f;
	transform.orientation = Rotation((float)(rand() % 31415) / 10000.0f, Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	transform.scale = (float)(rand() % 20 + 2) / 20.0f;

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
	dynProps.velocity = velocity;
	dynProps.angular_velocity = 0.0001 * Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);

	Entity newEntity = m_Scene->CreateEntity("");

	// make the Earth hitting asteroids big
	Vec3D dr = transform.location;
	Vec3D v = velocity;
	if (sqrt(dr.lengthSquare() - pow(dr * v, 2) / v.lengthSquare()) < (2.3f + transform.scale + 0.2)) // 2.3 is the radius of the earth, 0.2 is a safety factor
	{
		transform.scale = 5.0f;
		newEntity.AddComponent<MarkerComponent>(MarkerComponent(1.0f, 0.0f, 0.0f, 1.0f));
	}

	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(asteroidIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<HitPointComponent>(transform.scale * 20);
	// newEntity.AddComponent<HitPointComponent>(1.0f + (float)(rand() % 10));
	// newEntity.AddComponent<HitPointComponent>(1.0f);
	newEntity.AddComponent<AsteroidComponent>(AsteroidComponent());

}

void EntityManager::SpawnAsteroidCloud()
{
}

void EntityManager::SpawnDebris(Vec3D center, Vec3D velocity, float spread, float bulletChance)
{
	static int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];

	TransformComponent transform;
	transform.location = center;
	transform.orientation = Rotation((float)(rand() % 31415) / 10000.0f, Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	transform.scale = (float)(rand() % 10 + 2) / 100.0f;

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
	dynProps.velocity = velocity + Vec3D(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500) * spread / 1000.0f;
	dynProps.angular_velocity = 0.0001 * Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(asteroidIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(1000.0f)); // provide ttl in mili seconds

	if ((float)(rand() % 1000) / 1000 < bulletChance)
	{
		newEntity.AddComponent<BulletComponent>(BulletComponent());
	}
}

void EntityManager::ShootBullett(TransformComponent transform, Vec3D velocity)
{
	static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 0.001f;
	dynProps.velocity = velocity;
	transform.scale = 0.1f;
	transform.location += 0.1 * (transform.orientation.f3 - transform.orientation.f2);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(bulletIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(10000.0f)); // provide ttl in mili seconds
	newEntity.AddComponent<BulletComponent>(BulletComponent());
}

void EntityManager::LaunchMissile(int meshIdx, TransformComponent transform, entt::entity target)
{
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 0.001f;
	dynProps.velocity = 0.01f * transform.orientation.f1; // 0.00000001f
	dynProps.angular_velocity = Vec3D();
	transform.scale = 0.05f;
	transform.location += 0.1 * (transform.orientation.f3 + transform.orientation.f1);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(meshIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(20000.0f)); // provide ttl in mili seconds
	newEntity.AddComponent<TargetComponent>(target);
}

void EntityManager::RemoveMesh(int meshIdx)
{
	if (m_Scene->GetMeshLibrary().m_MeshTransforms[meshIdx].size() > 0)
		m_Scene->GetMeshLibrary().m_MeshTransforms[meshIdx].pop_back();
}

void EntityManager::SpawnExplosion(TransformComponent trf, DynamicPropertiesComponent dyn)
{
	static int explosionIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Explosion"];

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(trf);
	newEntity.AddComponent<MeshIndexComponent>(explosionIdx);
	newEntity.AddComponent<ColourComponent>(ColourComponent(0.8, 0.1f + float(rand() % 1000) / 5000.0f, 0.1f + float(rand() % 1000) / 5000.0f, 0.8f));
	newEntity.AddComponent<DynamicPropertiesComponent>(dyn);
	newEntity.AddComponent<TimerComponent>(TimerComponent(2000.0f));
	newEntity.AddComponent<ExplosionComponent>(ExplosionComponent());
}








