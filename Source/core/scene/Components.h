
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

//struct CameraComponent
//{
//	SceneCamera Camera;
//	bool Primary = true;
//	bool FixedAspectRatio = false;
//
//	CameraComponent() = default;
//	CameraComponent(const CameraComponent&) = default;
//};



#endif // COMPONENTS_H
