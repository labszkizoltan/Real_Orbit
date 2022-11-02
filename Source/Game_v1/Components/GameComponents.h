
#ifndef GAME_COMPONENTS_H
#define GAME_COMPONENTS_H

#include <string>
#include <utils/Matrix_3D.h>
#include <core/rendering/drawables/Mesh.h>

#include <entt/entt.hpp>

//---------------------------//
//----- Game components -----//
//---------------------------//

//----- Moving around in space -----//
struct DynamicPropertiesComponent
{
	float inertial_mass;
	Vec3D velocity;
	Vec3D angular_velocity;
	//	Vec3D acceleration;

	DynamicPropertiesComponent()
		: inertial_mass(0), velocity(Vec3D()), angular_velocity(Vec3D()) {}

};

struct GravitationalMassComponent
{
	float gravitational_mass;
};

struct ColliderComponent { char something = 0; };

//----- Entity lifetime -----//

struct TimerComponent
{
	float timeToLive = 0.0f;
	TimerComponent(float ttl) : timeToLive(ttl) {};
};

struct HitPointComponent
{
	float HP;
};

//----- Other -----//
struct TargetComponent
{
	entt::entity targetEntity = entt::null;
	TargetComponent(entt::entity tgt) : targetEntity(tgt) {};
};

// struct TeamComponent_0 { ColourComponent team_colour = { 0.3f, 0.9f, 0.3f, 1.0f }; };
// struct TeamComponent_1 { ColourComponent team_colour = { 0.9f, 0.3f, 0.3f, 1.0f }; };
// struct TeamComponent_2 { ColourComponent team_colour = { 0.3f, 0.3f, 0.9f, 1.0f }; };
// struct TeamComponent_3 { ColourComponent team_colour = { 0.9f, 0.9f, 0.3f, 1.0f }; };
// struct TeamComponent_4 { ColourComponent team_colour = { 0.9f, 0.5f, 0.1f, 1.0f }; };
// struct TeamComponent_5 { ColourComponent team_colour = { 0.3f, 0.9f, 0.9f, 1.0f }; };
// struct TeamComponent_6 { ColourComponent team_colour = { 0.5f, 0.1f, 0.9f, 1.0f }; };
// struct TeamComponent_7 { ColourComponent team_colour = { 0.5f, 0.3f, 0.1f, 1.0f }; };
// struct TeamComponent_default { ColourComponent team_colour = { 0.7f, 0.7f, 0.7f, 1.0f }; };

struct TeamComponent_0 { char something = 0; };
struct TeamComponent_1 { char something = 0; };
struct TeamComponent_2 { char something = 0; };
struct TeamComponent_3 { char something = 0; };
struct TeamComponent_4 { char something = 0; };
struct TeamComponent_5 { char something = 0; };
struct TeamComponent_6 { char something = 0; };
struct TeamComponent_7 { char something = 0; };
struct TeamComponent_default { char something = 0; };

struct PlayerComponent { char something = 0; };

struct ExplosionComponent { char something = 0; };

struct AsteroidComponent { char something = 0; };

struct BulletComponent { char something = 0; };

struct AntiMissilleComponent { char something = 0; };

struct MarkedForDestructionComponent { char something = 0; };

struct EnemyShipComponent { float shotTimer = 0.0f; };

struct ControlPointComponent 
{
	float capture_timer = 0.0f;
	float spawn_frequency = 900.0f;
	float spawn_timer = 900.0f;
	float radius = 1.0f;
	Vec3D location = Vec3D();
	ColourComponent colour = ColourComponent();
};

struct MovementControllComponent
{
	std::vector<Vec3D> waypoints;

	float GetPathLength()
	{
		if (waypoints.size() < 2)
			return 0;
		//	return (waypoints[1] - waypoints[0]).length();

		float pathLen = 0.0f;
		for (int i = waypoints.size(); i > 1; i--)
			pathLen += (waypoints[i - 1] - waypoints[i - 2]).length();

		return pathLen;
	}
};

struct WeaponControllComponent
{
	short int gunShots;
	float gunShotTimer; // in milli seconds
	short int missilleShots;
	float missilleShotTimer; // in milli seconds
	short int antiMissilleShots;
	float antiMissilleShotTimer; // in milli seconds

	short int gunShotsRemaining;
	float gunShotTimerRemaining; // in milli seconds
	short int missilleShotsRemaining;
	float missilleShotTimerRemaining; // in milli seconds
	short int antiMissilleShotsRemaining;
	float antiMissilleShotTimerRemaining; // in milli seconds

//	bool hasGun = false;
//	bool hasMissille = false;
//	bool hasAntiMissille = false;
};


struct VictoryComponent { char something = 0; };

struct LoosingComponent { char something = 0; };

struct MarkerComponent
{ 
	ColourComponent marker_colour = { 0.0f, 0.0f, 1.0f, 1.0f };

	MarkerComponent()
	{
		marker_colour = { 0.0f, 0.0f, 1.0f, 1.0f };
	}

	MarkerComponent(float r, float g, float b, float a)
	{
		marker_colour.r = r;
		marker_colour.g = g;
		marker_colour.b = b;
		marker_colour.a = a;
	}
};

enum class WeaponType { BULLET, MISSILLE, BEAM };

struct WeaponComponent
{
	WeaponType weapon_type = WeaponType::MISSILLE;

	static std::string WeaponType_to_String(const WeaponType& type) {
		switch (type)
		{
		case WeaponType::BULLET:	return std::string("BULLET");
		case WeaponType::MISSILLE:	return std::string("MISSILLE");
		case WeaponType::BEAM:		return std::string("BEAM");
		}

		LOG_CORE_INFO("MeshType not recognized, cannot convert to string");
		return std::string("MeshType not recognized");
	}

	static WeaponType String_to_WeaponType(const std::string& s) {
		if (s.compare("BULLET") == 0) { return WeaponType::BULLET; }
		if (s.compare("MISSILLE") == 0) { return WeaponType::MISSILLE; }
		if (s.compare("BEAM") == 0) { return WeaponType::BEAM; }

		LOG_CORE_INFO("String not recognized, cannot convert to MeshType");
		return WeaponType::BULLET;
	}

};


enum class PickupType { HEALTH, FUEL, AMMO };

struct HiddenPickupComponent
{
	PickupType type;
};

struct PickupComponent
{
	PickupType type;

//	PickupComponent(PickupType _type) : type(_type) {}
};



#endif // GAME_COMPONENTS_H
