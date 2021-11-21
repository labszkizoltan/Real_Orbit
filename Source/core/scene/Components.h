
#ifndef COMPONENTS_H
#define COMPONENTS_H

// #include <glm/glm.hpp>
#include <string>
#include <utils/Matrix_3D.h>
#include <core/rendering/drawables/Mesh.h>


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
};

struct DynamicPropertiesComponent
{
	float mass;
	Vec3D velocity;
	Vec3D angular_velocity;

//	Vec3D acceleration;
};

struct MeshComponent
{
	std::shared_ptr<Mesh> meshPtr;
};

template <MeshType Type>
struct TypedMeshComponent
{
	std::shared_ptr<Mesh> meshPtr;

	// for some reason this has to be constant, but Im fine with it
	static const MeshType s_Type = Type;
	static MeshType GetStaticType() { return s_Type; }
};

// identical to the transform component... I just use it to be able to query the scene registry for the camera
struct CameraComponent
{
	TransformComponent camera_transform;
};

// identical to the transform component... I just use it to be able to query the scene registry for the light position
struct LightComponent
{
	TransformComponent light_transform;
};



#endif // COMPONENTS_H
