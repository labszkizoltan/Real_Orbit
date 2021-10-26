#include "Mesh.h"

#include <core/pch/pch.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/TexturedMesh.h>

std::unique_ptr<Mesh> Mesh::Create(MeshType type)
{
    switch (type)
    {
    case MeshType::COLOURED_MESH:   return std::unique_ptr<ColouredMesh>();
    case MeshType::TEXTURED_MESH:   return std::unique_ptr<TexturedMesh>();
    default: LOG_CORE_INFO("MeshType not recognized, returning nullptr"); return nullptr;
    }

    return std::unique_ptr<Mesh>();
}
