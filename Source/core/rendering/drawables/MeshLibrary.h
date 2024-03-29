
#ifndef MESH_LIBRARY_H
#define MESH_LIBRARY_H

#include <core/rendering/drawables/Mesh.h>
#include <vector>
#include <core/scene/CoreComponents.h>

struct MeshLibrary
{
public:
	MeshLibrary() = default;
	~MeshLibrary() = default;
	int size() { return m_Meshes.size(); }
	void Clear();

public:
	std::unordered_map<std::string, int> m_NameIndexLookup;
	std::vector<std::shared_ptr<Mesh>> m_Meshes;
	std::vector<std::vector<TransformComponent>> m_MeshTransforms;
	std::vector<std::vector<ColourComponent>> m_ColourBuffers;
};


#endif // MESH_LIBRARY_H
