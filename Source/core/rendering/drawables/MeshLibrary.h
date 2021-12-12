
#ifndef MESH_LIBRARY_H
#define MESH_LIBRARY_H

#include <core/rendering/drawables/Mesh.h>
#include <vector>
#include <core/scene/Components.h>

struct MeshLibrary
{
public:
	MeshLibrary() = default;
	~MeshLibrary() = default;
	int size() { return m_Meshes.size(); }

public:
	std::unordered_map<std::string, int> m_NameIndexLookup;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;
	std::vector<std::vector<TransformComponent>> m_MeshTransforms;
};


#endif // MESH_LIBRARY_H
