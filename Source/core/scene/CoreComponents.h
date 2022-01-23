
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

	ColourComponent()
		: r(0.1f), g(0.1f), b(0.9f), a(0.2f) {}

	ColourComponent(float R, float G, float B, float A)
		: r(R), g(G), b(B), a(A) {}
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


#endif // CORE_COMPONENTS_H
