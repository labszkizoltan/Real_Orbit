#include "Mesh.h"

#include <core/pch/pch.h>
#include <core/rendering/drawables/ColouredMesh.h>
//#include <core/rendering/drawables/TexturedMesh.h>

std::string MeshType_to_String(MeshType type)
{
	switch (type)
	{
	case MeshType::COLOURED_MESH:	return std::string("COLOURED_MESH");
	case MeshType::BRIGHT_COLOURED_MESH:	return std::string("BRIGHT_COLOURED_MESH");
	case MeshType::NORMAL_MESH:		return std::string("NORMAL_MESH");
	case MeshType::ALPHA_MESH:		return std::string("ALPHA_MESH");
	case MeshType::MARKER_MESH:		return std::string("MARKER_MESH");
	case MeshType::LETTER_MESH:		return std::string("LETTER_MESH");
	case MeshType::SKYBOX:			return std::string("SKYBOX");
	case MeshType::SHADOW_MAP:		return std::string("SHADOW_MAP");
	}

	LOG_CORE_INFO("MeshType not recognized, cannot convert to string");
	return std::string("MeshType not recognized");
}

MeshType String_to_MeshType(std::string s)
{
	if (s.compare("COLOURED_MESH") == 0) { return MeshType::COLOURED_MESH; }
	if (s.compare("BRIGHT_COLOURED_MESH") == 0) { return MeshType::BRIGHT_COLOURED_MESH; }
	if (s.compare("NORMAL_MESH") == 0) { return MeshType::NORMAL_MESH; }
	if (s.compare("ALPHA_MESH") == 0) { return MeshType::ALPHA_MESH; }
	if (s.compare("MARKER_MESH") == 0) { return MeshType::MARKER_MESH; }
	if (s.compare("LETTER_MESH") == 0) { return MeshType::LETTER_MESH; }
	if (s.compare("SKYBOX") == 0) { return MeshType::SKYBOX; }
	if (s.compare("SHADOW_MAP") == 0) { return MeshType::SHADOW_MAP; }

	LOG_CORE_INFO("String not recognized, cannot convert to MeshType");
	return MeshType::UNKOWN;
}






