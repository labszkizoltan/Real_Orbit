
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
	entt::entity targetEntity;
	TargetComponent(entt::entity tgt) : targetEntity(tgt) {};
};

struct TeamComponent_0 { ColourComponent team_colour = { 0.3f, 0.9f, 0.3f, 1.0f }; };
struct TeamComponent_1 { ColourComponent team_colour = { 0.9f, 0.3f, 0.3f, 1.0f }; };
struct TeamComponent_2 { ColourComponent team_colour = { 0.3f, 0.3f, 0.9f, 1.0f }; };
struct TeamComponent_3 { ColourComponent team_colour = { 0.9f, 0.9f, 0.3f, 1.0f }; };
struct TeamComponent_4 { ColourComponent team_colour = { 0.9f, 0.5f, 0.1f, 1.0f }; };
struct TeamComponent_5 { ColourComponent team_colour = { 0.3f, 0.9f, 0.9f, 1.0f }; };
struct TeamComponent_6 { ColourComponent team_colour = { 0.5f, 0.1f, 0.9f, 1.0f }; };
struct TeamComponent_7 { ColourComponent team_colour = { 0.5f, 0.3f, 0.1f, 1.0f }; };
struct TeamComponent_default { ColourComponent team_colour = { 0.7f, 0.7f, 0.7f, 1.0f }; };


struct ExplosionComponent { char something = 0; };

struct AsteroidComponent { char something = 0; };

struct MarkerComponent
{ 
	ColourComponent marker_colour = { 0.0f, 0.0f, 1.0f, 1.0f };

	MarkerComponent(float r, float g, float b, float a)
	{
		marker_colour.r = r;
		marker_colour.g = g;
		marker_colour.b = b;
		marker_colour.a = a;
	}
};


#endif // GAME_COMPONENTS_H
