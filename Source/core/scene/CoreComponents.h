
#ifndef CORE_COMPONENTS_H
#define CORE_COMPONENTS_H

// #include <glm/glm.hpp>
#include <string>
#include <utils/Matrix_3D.h>
#include <core/rendering/drawables/Mesh.h>

#include <entt/entt.hpp>

//---------------------------//
//----- Core components -----//
//---------------------------//

struct TagComponent
{
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
		: Tag(tag) {}

};

struct TransformComponent
{
	Vec3D location;
	Mat_3D orientation;
	float scale;

	TransformComponent()
		: location(Vec3D()), orientation(Identity(1.0f)), scale(1.0f) {}
};

struct MeshComponent
{
	std::shared_ptr<Mesh> meshPtr;
};

struct MeshIndexComponent
{
	int idx = -1;
};

struct ColourComponent
{
	float r=0.7f;
	float g=0.3f;
	float b=0.3f;
	float a=0.2f;
};


/*
template <MeshType Type>
struct TypedMeshComponent
{
	std::shared_ptr<Mesh> meshPtr;

	// for some reason this has to be constant, but Im fine with it
	static const MeshType s_Type = Type;
	static MeshType GetStaticType() { return s_Type; }
};
*/

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

struct TeamComponent_0 {};
struct TeamComponent_1 {};
struct TeamComponent_2 {};
struct TeamComponent_3 {};
struct TeamComponent_4 {};
struct TeamComponent_5 {};
struct TeamComponent_6 {};
struct TeamComponent_7 {};
struct TeamComponent_default {};


struct ExplosionComponent { char something=0; };

#endif // CORE_COMPONENTS_H
