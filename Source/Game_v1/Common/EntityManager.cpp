
#include "EntityManager.h"
#include <utils/RandomGeneration.h>

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


void EntityManager::SpawnAsteroid(Vec3D center, Vec3D velocity, float spread, float markerRadius)
{
	static int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DeformedSphere"];
	static int battleshipIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["battleship"];

	int meshIdx = asteroidIdx;

	TransformComponent transform;
	transform.location = center + Vec3D(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500) * spread / 1000.0f;
	transform.orientation = Rotation((float)(rand() % 31415) / 10000.0f, Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	transform.scale = (float)(rand() % 20 + 2) / 20.0f;

	float hitPoints = transform.scale * 20;

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
	dynProps.velocity = velocity;
	dynProps.angular_velocity = 0.0001 * Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);

	Entity newEntity = m_Scene->CreateEntity("");

	// make the Earth hitting asteroids big
	Vec3D dr = transform.location;
	Vec3D v = velocity;
	// if (sqrt(dr.lengthSquare() - pow(dr * v, 2) / v.lengthSquare()) < (2.3f + transform.scale + 0.2)) // 2.3 is the radius of the earth, 0.2 is a safety factor
	if (sqrt(dr.lengthSquare() - pow(dr * v, 2) / v.lengthSquare()) < markerRadius) // 2.3 is the radius of the earth, 0.2 is a safety factor
	{
		/*
		Vec3D dr = transform.location;
		Vec3D z = transform.orientation.f3;
		// turn towards the target:
		Vec3D n = CrossProduct(z, dr); // no need to normalize the vector, since the Rotation matrix generator function does that
		float phi = acos(z * dr / (z.length() * dr.length()));
		phi = !std::isfinite(phi) ? 0.0f : (phi);
		transform.orientation = Rotation(phi, n) * transform.orientation;
		*/
		transform.orientation.f1 = Vec3D(0, -1, 0);
		transform.orientation.f2 = Vec3D(0, 0, -1);
		transform.orientation.f3 = Vec3D(1, 0, 0);

		transform.scale = 1.0f;
		hitPoints = 100;
		dynProps.angular_velocity = Vec3D(0,0,0);
		newEntity.AddComponent<MarkerComponent>(MarkerComponent(1.0f, 0.0f, 0.0f, 1.0f));
		meshIdx = battleshipIdx;
	}

	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(meshIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<HitPointComponent>(hitPoints);
	// newEntity.AddComponent<HitPointComponent>(1.0f + (float)(rand() % 10));
	// newEntity.AddComponent<HitPointComponent>(1.0f);
	newEntity.AddComponent<AsteroidComponent>(AsteroidComponent());
	newEntity.AddComponent<ColliderComponent>(ColliderComponent());
	

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

void EntityManager::ShootBullett(TransformComponent transform, Vec3D velocity, bool anti_missille)
{
	static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 0.001f;
	dynProps.velocity = velocity;
	transform.scale = 0.05f;
	transform.location += 0.1 * (transform.orientation.f3 - transform.orientation.f2);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(bulletIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(10000.0f)); // provide ttl in mili seconds
	newEntity.AddComponent<BulletComponent>(BulletComponent());
	if (anti_missille)
	{
		newEntity.AddComponent<AntiMissilleComponent>(AntiMissilleComponent());
		//newEntity.AddComponent<HitPointComponent>(0.01f);
		//newEntity.AddComponent<ColliderComponent>(ColliderComponent());
	}
}

void EntityManager::LaunchMissile(int meshIdx, TransformComponent hostTransform, DynamicPropertiesComponent hostVelocity, entt::entity target)
{
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 0.001f;
	dynProps.velocity = hostVelocity.velocity + 0.02f * hostTransform.orientation.f1; // 0.00000001f
	dynProps.angular_velocity = Vec3D();
	hostTransform.location += hostTransform.scale * (hostTransform.orientation.f3 + hostTransform.orientation.f1);
	hostTransform.scale = 0.05f;

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(hostTransform);
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

void EntityManager::SpawnExplosion(TransformComponent trf, DynamicPropertiesComponent dyn, ColourComponent col)
{
	static int explosionIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Explosion"];

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(trf);
	newEntity.AddComponent<MeshIndexComponent>(explosionIdx);
//	newEntity.AddComponent<ColourComponent>(ColourComponent(0.8, 0.1f + float(rand() % 1000) / 5000.0f, 0.1f + float(rand() % 1000) / 5000.0f, 0.8f));
	newEntity.AddComponent<ColourComponent>(col);
	newEntity.AddComponent<DynamicPropertiesComponent>(dyn);
	newEntity.AddComponent<TimerComponent>(TimerComponent(2000.0f));
	newEntity.AddComponent<ExplosionComponent>(ExplosionComponent());
}

void EntityManager::CreateStars()
{
	/*
	int starCount = 10000;
	// int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["YellowSphere"]; // -> this is a bright mesh and it slows down the program much more
	int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DeformedSphere"];

	std::vector<TransformComponent> star_transforms;
	for (int i = 0; i < starCount; i++)
	{
		TransformComponent tmpTransform;
		tmpTransform.location = Vec3D(rand() % 2000 - 1000, rand() % 2000 - 1000, rand() % 200 - 100);
		tmpTransform.orientation = Rotation((float)(rand() % 31415) / 10000.0f, Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
		tmpTransform.scale = (float)(rand() % 20 + 2) / 20.0f;
		star_transforms.push_back(tmpTransform);
	}

	m_Scene->m_StaticMeshLibrary.m_Meshes[asteroidIdx]->SetInstances(star_transforms);
	*/
}

void EntityManager::BuildStaticAsteroidField(DualOctTree* tree, float radius, int asteroidCount)
{
	for (int i = 0; i < asteroidCount; i++)
	{
		static int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DeformedSphere"];
		static int battleshipIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["battleship"];
		int meshIdx = asteroidIdx;

		TransformComponent transform;
		transform.location = Vec3D(RORNG::runif(), RORNG::runif(), RORNG::runif()) * 2 * radius - Vec3D(radius, radius, radius);
		transform.orientation = Rotation(RORNG::runif()*3.141592f, Vec3D(RORNG::runif(), RORNG::runif(), RORNG::runif()) - Vec3D(0.5f, 0.5f, 0.5f));
		transform.scale = 10 * RORNG::runif() + 5;

		DynamicPropertiesComponent dynProps;

		//if (transform.location.length() > radius / 10)
		{
			// i--;
			float hitPoints = transform.scale * 100;


			Entity newEntity = m_Scene->CreateEntity("");

			newEntity.AddComponent<TransformComponent>(transform);
			newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
			newEntity.AddComponent<MeshIndexComponent>(meshIdx);
			newEntity.AddComponent<HitPointComponent>(hitPoints);
			newEntity.AddComponent<AsteroidComponent>(AsteroidComponent());
			newEntity.AddComponent<ColliderComponent>(ColliderComponent());
		}

		if (i % 40 == 0)
		{
			Entity newEntity = m_Scene->CreateEntity("");

			TransformComponent trf2 = transform;
			trf2.location = trf2.location * (trf2.location.length() + trf2.scale + 5) / trf2.location.length();
			trf2.scale = 1;

			newEntity.AddComponent<TransformComponent>(trf2);
			newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
			newEntity.AddComponent<MeshIndexComponent>(battleshipIdx);
			newEntity.AddComponent<HitPointComponent>(100.0f);
			newEntity.AddComponent<AsteroidComponent>(AsteroidComponent()); // this component needed so the enemy ships are defending against missilles
			newEntity.AddComponent<ColliderComponent>(ColliderComponent());
			newEntity.AddComponent<MarkerComponent>(MarkerComponent(1.0f, 0.0f, 0.0f, 1.0f));
		}

	}

	tree->Update<ColliderComponent>(m_Scene);
}






