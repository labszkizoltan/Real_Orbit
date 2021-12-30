
// https://yaml-cpp.docsforge.com/master/tutorial/

#include "SceneSerializer.h"
#include <iostream>
#include <fstream>

#include <core/GlobalConstants.h>

#include <utils/Vector_3D.h>
#include <utils/Matrix_3D.h>
#include <core/scene/Entity.h>
#include <core/scene/CoreComponents.h>

#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/BrightColouredMesh.h>
#include <core/rendering/drawables/NormalMesh.h>
#include <core/rendering/drawables/Skybox.h>

SceneSerializer::SceneSerializer()
{
}

SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{
}

SceneSerializer::~SceneSerializer()
{
}

void SceneSerializer::Serialize(const std::string& output_file)
{
}

void SceneSerializer::DeSerialize_text(const std::string& scene_description)
{
	YAML::Node data = YAML::Load(scene_description);
	InitMeshLibrary(data);

	int camera_counter = 0, light_counter = 0;
	auto entities = data["Entities"];
	for (auto entity : entities)
	{
		std::string name = entity["Entity"].as<std::string>();
		Entity deserializedEntity = m_Scene->CreateEntity(name);

		//----- CameraComponent -----//
		auto cam_com = entity["CameraComponent"];
		if (cam_com)
		{
			if (camera_counter > 0) { LOG_CORE_WARN("SceneSerializer::DeSerialize() encountered more than one CameraComponent!"); continue; }

			camera_counter++;
			TransformComponent& result = m_Scene->GetCamera();
			result.location = cam_com["location"].as<Vec3D>();
			result.orientation = Mat_3D(cam_com["orientation"]["f1"].as<Vec3D>(), cam_com["orientation"]["f2"].as<Vec3D>(), cam_com["orientation"]["f3"].as<Vec3D>());
			result.scale = cam_com["scale"].as<float>();
		}

		//----- LightComponent -----//
		auto light_com = entity["LightComponent"];
		if (light_com)
		{
			if (light_counter > 0) { LOG_CORE_WARN("SceneSerializer::DeSerialize() encountered more than one LightComponent!"); continue; }

			light_counter++;
			TransformComponent& result = m_Scene->GetLight();
			result.location = light_com["location"].as<Vec3D>();
			result.orientation = Mat_3D(light_com["orientation"]["f1"].as<Vec3D>(), light_com["orientation"]["f2"].as<Vec3D>(), light_com["orientation"]["f3"].as<Vec3D>());
			result.scale = light_com["scale"].as<float>();
		}


		//----- TransformComponent -----//
		TransformComponent transformResult;
		auto trf_com = entity["TransformComponent"];
		if (trf_com)
		{
			transformResult.location = trf_com["location"].as<Vec3D>();
			transformResult.orientation = Mat_3D(trf_com["orientation"]["f1"].as<Vec3D>(), trf_com["orientation"]["f2"].as<Vec3D>(), trf_com["orientation"]["f3"].as<Vec3D>());
			transformResult.scale = trf_com["scale"].as<float>();
			deserializedEntity.AddComponent<TransformComponent>(transformResult);
		}

		//----- MeshComponent -----//
		auto mesh_com = entity["MeshComponent"];
		if (mesh_com)
		{
			std::string meshName = entity["MeshComponent"].as<std::string>();

			// check if key is present in the map
			if (m_Scene->m_MeshLibrary.m_NameIndexLookup.count(meshName) != 0)
			{
				int meshIdx = m_Scene->m_MeshLibrary.m_NameIndexLookup[meshName];
				std::shared_ptr<Mesh> meshPtr = m_Scene->m_MeshLibrary.m_Meshes[meshIdx];
				m_Scene->m_MeshLibrary.m_MeshTransforms[meshIdx].push_back(transformResult);
				deserializedEntity.AddComponent<MeshIndexComponent>(meshIdx);
			}
		}

		//----- DynamicPropertiesComponent -----//
		auto dynprop_com = entity["DynamicPropertiesComponent"];
		if (dynprop_com)
		{
			DynamicPropertiesComponent result;
			result.inertial_mass = dynprop_com["inertial_mass"].as<float>();
			result.velocity = dynprop_com["velocity"].as<Vec3D>();
			result.angular_velocity = dynprop_com["angular_velocity"].as<Vec3D>();
			deserializedEntity.AddComponent<DynamicPropertiesComponent>(result);
		}

		//----- GravitationalMassComponent -----//
		auto gravmass_com = entity["GravitationalMassComponent"];
		if (gravmass_com)
		{
			GravitationalMassComponent result;
			result.gravitational_mass = gravmass_com.as<float>();
			deserializedEntity.AddComponent<GravitationalMassComponent>(result);
		}
	}
}


void SceneSerializer::DeSerialize_file(const std::string& input_file)
{
	std::ifstream stream(input_file);
	if (!stream.is_open())
		LOG_CORE_WARN("SceneSerializer::DeSerialize() was unable to open scene file: {0}", input_file);

	std::stringstream strStream;
	strStream << stream.rdbuf();
	DeSerialize_text(strStream.str());
}

// this may be not necessary
static bool contains_key(std::unordered_map<std::string, int> map, std::string key)
{
	// Key is not present
	if (map.find(key) == map.end())
		return false;

	return true;


	// Key is not present
	if (map.count(key) == 0)
		return "Not Present";

	return "Present";
}

void SceneSerializer::InitMeshLibrary(const YAML::Node& data)
{
	YAML::Node meshes = data["Meshes"];
	std::cout << "number of meshes defined in config: " << meshes.size() << "\n";

	int mesh_idx = 0;
	for (auto mesh : meshes)
	{
		std::string mesh_name = mesh["Mesh"].as<std::string>();
		MeshType type = mesh["MeshType"].as<MeshType>();
		std::cout << "mesh name and type: " << mesh_name << ", " << MeshType_to_String(type) << "\n";

		if (type == MeshType::COLOURED_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<ColouredMesh>(buffer_data.vertex_data, buffer_data.index_data);

			m_Scene->m_MeshLibrary.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			m_Scene->m_MeshLibrary.m_Meshes.push_back(mesh_ptr);
			m_Scene->m_MeshLibrary.m_MeshTransforms.push_back(std::vector<TransformComponent>());
		}
		else if (type == MeshType::BRIGHT_COLOURED_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<BrightColouredMesh>(buffer_data.vertex_data, buffer_data.index_data);

			m_Scene->m_MeshLibrary.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			m_Scene->m_MeshLibrary.m_Meshes.push_back(mesh_ptr);
			m_Scene->m_MeshLibrary.m_MeshTransforms.push_back(std::vector<TransformComponent>());
		}
		else if (type == MeshType::NORMAL_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::string texturePath = mesh["texture_file"].as<std::string>();
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<NormalMesh>(buffer_data.vertex_data, buffer_data.index_data, texturePath);

			m_Scene->m_MeshLibrary.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			m_Scene->m_MeshLibrary.m_Meshes.push_back(mesh_ptr);
			m_Scene->m_MeshLibrary.m_MeshTransforms.push_back(std::vector<TransformComponent>());
		}
		else if (type == MeshType::SKYBOX)
		{
			std::vector<float> vertex_data = Skybox::CreateSkyboxVertexData(g_SkyboxMeshResolution);
			std::vector<uint32_t> index_data = Skybox::CreateSkyboxIndexData(g_SkyboxMeshResolution);
			std::vector<std::string> textureFilenames = {
				mesh["texture_file_f"].as<std::string>(),
				mesh["texture_file_b"].as<std::string>(),
				mesh["texture_file_l"].as<std::string>(),
				mesh["texture_file_r"].as<std::string>(),
				mesh["texture_file_u"].as<std::string>(),
				mesh["texture_file_d"].as<std::string>()
			};

			m_Scene->m_Skybox = std::make_shared<Skybox>(vertex_data, index_data, textureFilenames);
		}
	}
}

/*
void SceneSerializer::FillMeshLibrary(const YAML::Node& data)
{
	auto meshes = data["Meshes"];
	std::cout << "number of meshes defined in config: " << meshes.size() << "\n";
	for (auto mesh : meshes)
	{
		std::string mesh_name = mesh["Mesh"].as<std::string>();
		MeshType type = mesh["MeshType"].as<MeshType>();
		if (type == MeshType::COLOURED_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
//			std::shared_ptr<Mesh> mesh_ptr = std::shared_ptr<Mesh>(new ColouredMesh(buffer_data.vertex_data, buffer_data.index_data));
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<ColouredMesh>(buffer_data.vertex_data, buffer_data.index_data);
			m_Scene->m_MeshLibrary_old.m_Meshes[mesh_name] = mesh_ptr;
		}
		else if (type == MeshType::TEXTURED_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::string texturePath = mesh["texture_file"].as<std::string>();

//			std::shared_ptr<Mesh> mesh_ptr = std::shared_ptr<Mesh>(new TexturedMesh(buffer_data.vertex_data, buffer_data.index_data, texturePath));
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<TexturedMesh>(buffer_data.vertex_data, buffer_data.index_data, texturePath);
			m_Scene->m_MeshLibrary_old.m_Meshes[mesh_name] = mesh_ptr;
		}
		else if (type == MeshType::NORMAL_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::string texturePath = mesh["texture_file"].as<std::string>();

//			std::shared_ptr<Mesh> mesh_ptr = std::shared_ptr<Mesh>(new NormalMesh(buffer_data.vertex_data, buffer_data.index_data, texturePath));
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<NormalMesh>(buffer_data.vertex_data, buffer_data.index_data, texturePath);
			m_Scene->m_MeshLibrary_old.m_Meshes[mesh_name] = mesh_ptr;
		}
		else if (type == MeshType::SKYBOX)
		{
			std::vector<float> vertex_data = Skybox::CreateSkyboxVertexData(g_SkyboxMeshResolution);
			std::vector<uint32_t> index_data = Skybox::CreateSkyboxIndexData(g_SkyboxMeshResolution);
			std::vector<std::string> textureFilenames = {
				mesh["texture_file_f"].as<std::string>(),
				mesh["texture_file_b"].as<std::string>(),
				mesh["texture_file_l"].as<std::string>(),
				mesh["texture_file_r"].as<std::string>(),
				mesh["texture_file_u"].as<std::string>(),
				mesh["texture_file_d"].as<std::string>()
			};

//			std::shared_ptr<Mesh> mesh_ptr = std::shared_ptr<Mesh>(new Skybox(vertex_data, index_data, textureFilenames));
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<Skybox>(vertex_data, index_data, textureFilenames);
			m_Scene->m_MeshLibrary_old.m_Meshes[mesh_name] = mesh_ptr;
		}
		else
		{
			LOG_CORE_WARN("Unidentified MeshType found when creating MeshLibrary!");
		}
	}
}
*/


OGLBufferData SceneSerializer::ParseVertexFile(const std::string& filename)
{
	OGLBufferData result;
	std::ifstream myfile(filename.c_str());
	if(!myfile.is_open())
	{
		LOG_CORE_WARN("SceneSerializer::ParseVertexFile() was unable to open file: {0}", filename);
		return result; 
	}

	int v_count = 0, i_count = 0;

	myfile >> v_count;
	myfile >> i_count;
	
	result.vertex_data.resize(v_count);
	result.index_data.resize(i_count);

	for (int i = 0; i < (v_count); i++)
		myfile >> result.vertex_data[i];

	for (int i = 0; i < i_count; i++)
		myfile >> result.index_data[i];

	myfile.close();

	if (result.vertex_data.size() == 0 || result.index_data.size() == 0)
		LOG_CORE_WARN("SceneSerializer::ParseVertexFile(): vertex or index data missing from file: ", filename);

	return result;
}



namespace YAML {

	template<>
	struct convert<Vec3D>
	{
		static Node encode(const Vec3D v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			return node;
		}

		static bool decode(const Node& node, Vec3D& v)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			return true;
		}

	};


	template<>
	struct convert<MeshType>
	{
		static Node encode(const MeshType t)
		{
			Node node;
//			node["MeshType"] = MeshType_to_String(t);
			node["MeshType"] = MeshType_to_String(t);
			return node;
		}

		static bool decode(const Node& node, MeshType& t)
		{
			if (!node.IsScalar())
				return false;

			std::string mesh_type_string = node.as<std::string>();
			t = String_to_MeshType(mesh_type_string);
			return true;
		}

	};


}




