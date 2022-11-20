
// https://yaml-cpp.docsforge.com/master/tutorial/

#include "SceneSerializer.h"
#include <iostream>
#include <fstream>

#include <core/GlobalConstants.h>

#include <utils/Vector_3D.h>
#include <utils/Matrix_3D.h>
#include <core/scene/Entity.h>
#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>

#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/BrightColouredMesh.h>
#include <core/rendering/drawables/NormalMesh.h>
#include <core/rendering/drawables/RoughMesh.h>
#include <core/rendering/drawables/AlphaMesh.h>
#include <core/rendering/drawables/MarkerMesh.h>
#include <core/rendering/drawables/LetterMesh.h>
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
	InitMeshLibrary(data, m_Scene->m_MeshLibrary);
//	InitMeshLibrary(data, m_Scene->m_StaticMeshLibrary);

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

		//----- ColourComponent -----//
		ColourComponent colResult;
		auto col_com = entity["ColourComponent"];
		if (col_com)
		{
			colResult = col_com.as<ColourComponent>();
			deserializedEntity.AddComponent<ColourComponent>(colResult);
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

				// if entity doesnt have a colour component, but should, give it a default one
				if (meshPtr->GetColourInstances() != -1 && !deserializedEntity.HasComponent<ColourComponent>())
					deserializedEntity.AddComponent<ColourComponent>(ColourComponent(1.0f, 1.0f, 1.0f, 0.1f));
					//deserializedEntity.AddComponent<ColourComponent>(ColourComponent(0.0f, 0.0f, 0.0f, 0.1f));

				if (deserializedEntity.HasComponent<ColourComponent>())
				{
					int colBufIdx = meshPtr->GetColourInstances();
					m_Scene->m_MeshLibrary.m_ColourBuffers[colBufIdx].push_back(deserializedEntity.GetComponent<ColourComponent>());
				}
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

		//----- ColliderComponent -----//
		auto collider_com = entity["ColliderComponent"];
		if (collider_com)
		{
			ColliderComponent coll_com;
			deserializedEntity.AddComponent<ColliderComponent>(coll_com);
		}

		//----- GravitationalMassComponent -----//
		auto gravmass_com = entity["GravitationalMassComponent"];
		if (gravmass_com)
		{
			GravitationalMassComponent result;
			result.gravitational_mass = gravmass_com.as<float>();
			deserializedEntity.AddComponent<GravitationalMassComponent>(result);
		}

		//----- HitPointComponent -----//
		auto hp_com = entity["HitPointComponent"];
		if (hp_com)
		{
			HitPointComponent result;
			result.HP = hp_com.as<float>();
			deserializedEntity.AddComponent<HitPointComponent>(result);
		}

		//----- MarkerComponent -----//
		auto marker_com = entity["MarkerComponent"];
		if (marker_com)
		{
			MarkerComponent result;
			result.marker_colour = marker_com.as<ColourComponent>();
			deserializedEntity.AddComponent<MarkerComponent>(result);
		}

		//----- TeamComponent -----//
		auto team_com = entity["TeamComponent"];
		if (team_com)
		{
			int team_id = team_com.as<int>();
			if (team_id == 0) deserializedEntity.AddComponent<TeamComponent_0>();
			else if (team_id == 1) deserializedEntity.AddComponent<TeamComponent_1>();
			else if (team_id == 2) deserializedEntity.AddComponent<TeamComponent_2>();
			else if (team_id == 3) deserializedEntity.AddComponent<TeamComponent_3>();
			else if (team_id == 4) deserializedEntity.AddComponent<TeamComponent_4>();
			else if (team_id == 5) deserializedEntity.AddComponent<TeamComponent_5>();
			else if (team_id == 6) deserializedEntity.AddComponent<TeamComponent_6>();
			else if (team_id == 7) deserializedEntity.AddComponent<TeamComponent_7>();
			else deserializedEntity.AddComponent<TeamComponent_default>();
		}

		//----- WeaponComponent -----//
		auto weapon_com = entity["WeaponComponent"];
		if (weapon_com)
		{
			WeaponComponent result;
			result.weapon_type = weapon_com.as<WeaponType>();
			deserializedEntity.AddComponent<WeaponComponent>(result);
		}

		//----- EnemyShipComponent -----//
		auto enemy_ship_com = entity["EnemyShipComponent"];
		if (enemy_ship_com)
		{
			deserializedEntity.AddComponent<EnemyShipComponent>();
		}


		//----- ControlPointComponent -----//
		auto control_point_com = entity["ControlPointComponent"];
		if (control_point_com)
		{
			ControlPointComponent cpc;
			cpc.radius = control_point_com["radius"].as<float>();
			cpc.location = control_point_com["location"].as<Vec3D>();
			cpc.colour = control_point_com["colour"].as<ColourComponent>();
			deserializedEntity.AddComponent<ControlPointComponent>(cpc);
		}


		//----- WeaponControllComponent -----//
		auto weapon_controll_com = entity["WeaponControllComponent"];
		if (weapon_controll_com)
		{
			WeaponControllComponent wcc = weapon_controll_com.as<WeaponControllComponent>();
			deserializedEntity.AddComponent<WeaponControllComponent>(wcc);
		}

		//----- MovementControllComponent -----//
		auto movement_controll_com = entity["MovementControllComponent"];
		if (movement_controll_com)
		{
			MovementControllComponent mcc = movement_controll_com.as<MovementControllComponent>();
			deserializedEntity.AddComponent<MovementControllComponent>(mcc);
		}

		//----- VictoryComponent -----//
		auto victory_com = entity["VictoryComponent"];
		if (victory_com)
		{
			deserializedEntity.AddComponent<VictoryComponent>();
		}

		//----- LoosingComponent -----//
		auto loosing_com = entity["LoosingComponent"];
		if (loosing_com)
		{
			deserializedEntity.AddComponent<LoosingComponent>();
		}


		//----- PlayerComponent -----//
		auto player_com = entity["PlayerComponent"];
		if (player_com)
		{
//			PlayerComponent player_com;
//			deserializedEntity.AddComponent<PlayerComponent>(player_com);
			deserializedEntity.AddComponent<PlayerComponent>();
		}

//		std::cout << "entity de-serialized\n";
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

void SceneSerializer::InitMeshLibrary(const YAML::Node& data, MeshLibrary& mesh_library)
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

			mesh_library.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			mesh_library.m_Meshes.push_back(mesh_ptr);
			mesh_library.m_MeshTransforms.push_back(std::vector<TransformComponent>());
			// mesh_library.m_StaticMeshTransforms.push_back(std::vector<TransformComponent>());
		}
		else if (type == MeshType::BRIGHT_COLOURED_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<BrightColouredMesh>(buffer_data.vertex_data, buffer_data.index_data);

			mesh_library.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			mesh_library.m_Meshes.push_back(mesh_ptr);
			mesh_library.m_MeshTransforms.push_back(std::vector<TransformComponent>());
			// mesh_library.m_StaticMeshTransforms.push_back(std::vector<TransformComponent>());
		}
		else if (type == MeshType::NORMAL_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::string texturePath = mesh["texture_file"].as<std::string>();
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<NormalMesh>(buffer_data.vertex_data, buffer_data.index_data, texturePath);

			mesh_library.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			mesh_library.m_Meshes.push_back(mesh_ptr);
			mesh_library.m_MeshTransforms.push_back(std::vector<TransformComponent>());
			// mesh_library.m_StaticMeshTransforms.push_back(std::vector<TransformComponent>());
		}
		else if (type == MeshType::ROUGH_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::string texturePath = mesh["texture_file"].as<std::string>();
			std::string normalMapPath = mesh["normal_map_file"].as<std::string>();
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<RoughMesh>(buffer_data.vertex_data, buffer_data.index_data, texturePath, normalMapPath);

			mesh_library.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			mesh_library.m_Meshes.push_back(mesh_ptr);
			mesh_library.m_MeshTransforms.push_back(std::vector<TransformComponent>());
			// mesh_library.m_StaticMeshTransforms.push_back(std::vector<TransformComponent>());
		}
		else if (type == MeshType::LETTER_MESH)
		{
			std::string texturePath = mesh["texture_file"].as<std::string>();
			std::shared_ptr<Mesh> mesh_ptr = std::make_shared<LetterMesh>(texturePath);

			mesh_library.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			mesh_library.m_Meshes.push_back(mesh_ptr);
			mesh_library.m_MeshTransforms.push_back(std::vector<TransformComponent>());
		}
		// leave out the transparent mesh types, so they end up at the end of the mesh library and will be rendered last
//		else if (type == MeshType::ALPHA_MESH) {}
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

	// do another round to read transparent meshes
	for (auto mesh : meshes)
	{
		std::string mesh_name = mesh["Mesh"].as<std::string>();
		MeshType type = mesh["MeshType"].as<MeshType>();
		std::cout << "mesh name and type: " << mesh_name << ", " << MeshType_to_String(type) << "\n";

		if (type == MeshType::ALPHA_MESH)
		{
			std::string vertex_file = mesh["vertex_file"].as<std::string>();
			OGLBufferData buffer_data = ParseVertexFile(vertex_file);
			std::shared_ptr<AlphaMesh> temp_mesh_ptr = std::make_shared<AlphaMesh>(buffer_data.vertex_data, buffer_data.index_data);
			temp_mesh_ptr->SetColourBufferIndex(mesh_library.m_ColourBuffers.size());
			std::shared_ptr<Mesh> mesh_ptr = temp_mesh_ptr;

			mesh_library.m_NameIndexLookup[mesh_name] = mesh_idx; mesh_idx++;
			mesh_library.m_Meshes.push_back(mesh_ptr);
			mesh_library.m_MeshTransforms.push_back(std::vector<TransformComponent>());
			mesh_library.m_ColourBuffers.push_back(std::vector<ColourComponent>());
			// mesh_library.m_StaticMeshTransforms.push_back(std::vector<TransformComponent>());
			// mesh_library.m_StaticColourBuffers.push_back(std::vector<ColourComponent>());
		}
	}

	// regardless of being defined in the scene file, add the default marker mesh:
	{
		std::shared_ptr<MarkerMesh> temp_mesh_ptr = std::make_shared<MarkerMesh>();
		temp_mesh_ptr->SetColourBufferIndex(mesh_library.m_ColourBuffers.size());
		std::shared_ptr<Mesh> mesh_ptr = temp_mesh_ptr;

		mesh_library.m_NameIndexLookup["DefaultMarker"] = mesh_idx; mesh_idx++;
		mesh_library.m_Meshes.push_back(mesh_ptr);
		mesh_library.m_MeshTransforms.push_back(std::vector<TransformComponent>());
		mesh_library.m_ColourBuffers.push_back(std::vector<ColourComponent>());
		// mesh_library.m_StaticMeshTransforms.push_back(std::vector<TransformComponent>());
		// mesh_library.m_StaticColourBuffers.push_back(std::vector<ColourComponent>());
	}

	// also add the target marker mesh:
	{
		std::shared_ptr<MarkerMesh> temp_mesh_ptr = std::make_shared<MarkerMesh>(MarkerMesh::s_DefaultTargetVertices, MarkerMesh::s_DefaultTargetIndices);
		temp_mesh_ptr->SetColourBufferIndex(mesh_library.m_ColourBuffers.size());
		std::shared_ptr<Mesh> mesh_ptr = temp_mesh_ptr;

		mesh_library.m_NameIndexLookup["TargetMarker"] = mesh_idx; mesh_idx++;
		mesh_library.m_Meshes.push_back(mesh_ptr);
		mesh_library.m_MeshTransforms.push_back(std::vector<TransformComponent>());
		mesh_library.m_ColourBuffers.push_back(std::vector<ColourComponent>());
		// mesh_library.m_StaticMeshTransforms.push_back(std::vector<TransformComponent>());
		// mesh_library.m_StaticColourBuffers.push_back(std::vector<ColourComponent>());
	}

}

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
	struct convert<ColourComponent>
	{
		static Node encode(const ColourComponent v)
		{
			Node node;
			node.push_back(v.r);
			node.push_back(v.g);
			node.push_back(v.b);
			node.push_back(v.a);
			return node;
		}

		static bool decode(const Node& node, ColourComponent& v)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			v.r = node[0].as<float>();
			v.g = node[1].as<float>();
			v.b = node[2].as<float>();
			v.a = node[3].as<float>();
			return true;
		}

	};


	template<>
	struct convert<MeshType>
	{
		static Node encode(const MeshType t)
		{
			Node node;
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


	template<>
	struct convert<WeaponType>
	{
		static Node encode(const WeaponType t)
		{
			Node node;
			node["WeaponComponent"] = WeaponComponent::WeaponType_to_String(t);
			return node;
		}

		static bool decode(const Node& node, WeaponType& t)
		{
			if (!node.IsScalar())
				return false;

			std::string mesh_type_string = node.as<std::string>();
			t = WeaponComponent::String_to_WeaponType(mesh_type_string);
			return true;
		}

	};


	template<>
	struct convert<WeaponControllComponent>
	{
		static Node encode(const WeaponControllComponent v)
		{
			Node node;
			node.push_back(v.gunShots);
			node.push_back(v.gunShotTimer);
			node.push_back(v.missilleShots);
			node.push_back(v.missilleShotTimer);
			node.push_back(v.antiMissilleShots);
			node.push_back(v.antiMissilleShotTimer);

			return node;
		}

		static bool decode(const Node& node, WeaponControllComponent& v)
		{
			if (!node.IsSequence() || node.size() != 6)
				return false;

			v.gunShots = node[0].as<short int>();
			v.gunShotTimer = node[1].as<short int>();
			v.missilleShots = node[2].as<short int>();
			v.missilleShotTimer = node[3].as<short int>();
			v.antiMissilleShots = node[4].as<short int>();
			v.antiMissilleShotTimer = node[5].as<short int>();

			v.gunShotsRemaining = node[0].as<short int>();
			v.gunShotTimerRemaining = node[1].as<short int>();
			v.missilleShotsRemaining = node[2].as<short int>();
			v.missilleShotTimerRemaining = node[3].as<short int>();
			v.antiMissilleShotsRemaining = node[4].as<short int>();
			v.antiMissilleShotTimerRemaining = node[5].as<short int>();

			return true;
		}

	};


	template<>
	struct convert<MovementControllComponent>
	{
		static Node encode(const MovementControllComponent v)
		{
			Node node;
			for (int i = 0; i < v.waypoints.size(); i++)
				node.push_back(v.waypoints[i]);
			return node;
		}

		static bool decode(const Node& node, MovementControllComponent& v)
		{
			if (!node.IsSequence())
				return false;

			for (int i = node.size(); i > 0; i--)
				v.waypoints.push_back(node[i-1].as<Vec3D>());

			return true;
		}

	};


}



