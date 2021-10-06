
#ifndef COMPONENTS_H
#define COMPONENTS_H

// #include <glm/glm.hpp>
#include <string>
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
	float x, y, rotation;
//	glm::mat4 Transform = glm::mat4(1.0f);
//
//	TransformComponent() = default;
//	TransformComponent(const TransformComponent&) = default;
//	TransformComponent(const glm::mat4& transform)
//		: Transform(transform) {}
//
//	operator const glm::mat4& () const { return Transform; } // this is like an implicit cast so the TransformComponent struct can be passed as a glm::mat4 to functions
//	operator glm::mat4& () { return Transform; }
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
