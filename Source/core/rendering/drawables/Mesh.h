
#ifndef MESH_H
#define MESH_H

#include <core/rendering/Buffer.h>

// each mesh type has a fixed layout, and a corresponding shader that can draw it
enum class MeshType
{
	COLOURED_MESH, TEXTURED_MESH, SKYBOX
};

class Mesh
{
public:
	virtual ~Mesh() {};

	virtual void Draw() = 0;
	virtual MeshType GetMeshType() = 0;
	virtual BufferLayout GetBufferLayout() = 0;

	static std::unique_ptr<Mesh> Create(MeshType type);
};



#endif // MESH_H
