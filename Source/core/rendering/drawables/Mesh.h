
#ifndef MESH_H
#define MESH_H

#include <core/rendering/Buffer.h>

// forward declare struct:
struct TransformComponent;
struct ColourComponent;

// each mesh type has a fixed layout, and a corresponding shader that can draw it
enum class MeshType
{
	COLOURED_MESH, BRIGHT_COLOURED_MESH, NORMAL_MESH, ALPHA_MESH, ROUGH_MESH, MARKER_MESH, LETTER_MESH, SKYBOX, SHADOW_MAP, UNKOWN
};

std::string MeshType_to_String(MeshType type);
MeshType String_to_MeshType(std::string s);

class Mesh
{
public:
	virtual ~Mesh() {};

	virtual void Draw() = 0;
	virtual void SetInstances(const std::vector<TransformComponent>& transforms) = 0;
	virtual void SetColourInstances(const std::vector<ColourComponent>& colours) = 0;
	virtual void DrawInstances(const std::vector<TransformComponent>& transforms) = 0;
	virtual int GetColourInstances() = 0; // returns the index of the ColourComponent vector from the mesh library
	virtual MeshType GetMeshType() = 0;
	virtual BufferLayout GetVertexLayout() = 0;
	virtual BufferLayout GetInstanceLayout() = 0;

//	static std::unique_ptr<Mesh> Create(MeshType type);
};



#endif // MESH_H
