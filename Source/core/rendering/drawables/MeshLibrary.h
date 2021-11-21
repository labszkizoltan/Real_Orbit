
#ifndef MESH_LIBRARY_H
#define MESH_LIBRARY_H

#include <core/rendering/drawables/Mesh.h>

class MeshLibrary
{
public:
	MeshLibrary() = default;
	~MeshLibrary() = default;

public:
	std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;

};






















#endif // MESH_LIBRARY_H
