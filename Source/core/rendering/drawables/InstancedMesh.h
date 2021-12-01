

#ifndef INSTANCED_MESH_H
#define INSTANCED_MESH_H

#include <core/rendering/Buffer.h>
#include <core/rendering/drawables/Mesh.h>

// forward declaring TransformComponent
struct TransformComponent;

// each mesh type has a fixed layout, and a corresponding shader that can draw it
enum class InstancedMeshType
{
	INSTANCED_COLOURED_MESH, INSTANCED_NORMAL_MESH, INSTANCED_SHADOW_MAP, UNKOWN
};


class InstancedMesh
{
public:
	virtual ~InstancedMesh() {};

	virtual void DrawInstances(const std::vector<TransformComponent>& transforms) = 0;
	virtual InstancedMeshType GetMeshType() = 0;
	virtual BufferLayout GetVertexLayout() = 0;
	virtual BufferLayout GetInstanceLayout() = 0;
};



#endif // INSTANCED_MESH_H





