
#include "TestLayer7.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>

#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/Skybox.h>
#include <core/rendering/drawables/NormalMesh.h>

//#include <core/rendering/shader_source_files/instanced_normal_shader.h>
//#include <core/rendering/shader_source_files/instanced_colour_shader.h>

#include <core/scene/SceneSerializer.h>
#include <core/scene/Components.h>

#include <core/GlobalConstants.h>

#include <utils/Vector_3D.h>
#include <glad/glad.h>

#include "scene_descriptions.h"

#define BIND_EVENT_FN(x) std::bind(&TestLayer7::x, this, std::placeholders::_1)

OGLBufferData ParseVertexFile(const std::string& filename)
{
	OGLBufferData result;
	std::ifstream myfile(filename.c_str());
	if (!myfile.is_open())
	{
		LOG_CORE_WARN("TestLayer7::ParseVertexFile() was unable to open file: {0}", filename);
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
		LOG_CORE_WARN("TestLayer7::ParseVertexFile(): vertex or index data missing from file: ", filename);

	return result;
}


TestLayer7::TestLayer7()
	: Layer("TestLayer7")
{
	LOG_INFO("TestLayer7 constructed");
}

void TestLayer7::OnAttach()
{
	LOG_INFO("TestLayer7 attached");


	TransformComponent cam_trf_tmp = { Vec3D(), Identity(1.0f), 1.0f };
	m_Camera = cam_trf_tmp;

	// skybox
	auto skybox_vertices = Skybox::CreateSkyboxVertexData(10);
	auto skybox_indices = Skybox::CreateSkyboxIndexData(10);
	std::vector<std::string> textureFilenames = {
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_front.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_back.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_left.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_right.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_up.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_down.png"
	};

	m_Skybox = std::make_shared<Skybox>(skybox_vertices, skybox_indices, textureFilenames);


//	std::string vertex_file("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/meshes/03_normalMeshes/Sphere_4.txt");
	std::string vertex_file("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/meshes/03_normalMeshes/Sphere_16_corrected.txt");
	OGLBufferData buffer_data = ParseVertexFile(vertex_file);
	std::string texturePath("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/Earth_Realistic_lowres.png");
	m_NormalMesh = std::make_shared<NormalMesh>(buffer_data.vertex_data, buffer_data.index_data, texturePath);

	vertex_file = "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/meshes/01_colouredMeshes/tetrahedron.txt";
	buffer_data = ParseVertexFile(vertex_file);
	m_ColorMesh = std::make_shared<ColouredMesh>(buffer_data.vertex_data, buffer_data.index_data);

	for (int i = 0; i < 500; i++)
	{
		TransformComponent tmp;
		tmp.location = Vec3D(
			2 * (float)(std::rand() % 1000) / 1000.0f - 1,
			2 * (float)(std::rand() % 1000) / 1000.0f - 1,
			2 * (float)(std::rand() % 1000) / 1000.0f - 1);
		tmp.location *= 20.0f;
		tmp.orientation = Rotation(3.141592f * (float)(std::rand() % 1000) / 1000.0f, tmp.location);
		tmp.scale = 0.05f+0.25f * (float)(std::rand() % 1000) / 1000.0f;
		m_NormalTransforms.push_back(tmp);
	}
//	m_NormalTransforms[0].location = Vec3D(0.0f, 00.0f, 0.0f);
//	m_NormalTransforms[0].orientation = Identity(1.0f);
//	m_NormalTransforms[0].scale = 2.0f;

	m_NormalTransforms[1].location = Vec3D(30.0f, 0.0f, 0.0f);
	m_NormalTransforms[1].orientation = Identity(1.0f);
	m_NormalTransforms[1].scale = 10.0f;

	m_NormalTransforms[2].location = Vec3D(-30.0f, 0.0f, 0.0f);
	m_NormalTransforms[2].orientation = Identity(1.0f);
	m_NormalTransforms[2].scale = 10.0f;

	m_NormalTransforms[3].location = Vec3D(0.0f, 30.0f, 0.0f);
	m_NormalTransforms[3].orientation = Identity(1.0f);
	m_NormalTransforms[3].scale = 10.0f;

	m_NormalTransforms[4].location = Vec3D(0.0f, -30.0f, 0.0f);
	m_NormalTransforms[4].orientation = Identity(1.0f);
	m_NormalTransforms[4].scale = 10.0f;

	m_NormalTransforms[5].location = Vec3D(0.0f, 0.0f, 30.0f);
	m_NormalTransforms[5].orientation = Identity(1.0f);
	m_NormalTransforms[5].scale = 10.0f;

	m_NormalTransforms[6].location = Vec3D(0.0f, 0.0f, -30.0f);
	m_NormalTransforms[6].orientation = Identity(1.0f);
	m_NormalTransforms[6].scale = 10.0f;


	for (int i = 0; i < 500; i++)
	{
		TransformComponent tmp;
		tmp.location = Vec3D(
			2 * (float)(std::rand() % 1000) / 1000.0f - 1,
			2 * (float)(std::rand() % 1000) / 1000.0f - 1,
			2 * (float)(std::rand() % 1000) / 1000.0f - 1);
		tmp.location *= 20.0f;
		tmp.orientation = Rotation(3.141592f * (float)(std::rand() % 1000) / 1000.0f, tmp.location);
		tmp.scale = 0.05f + 0.25f * (float)(std::rand() % 1000) / 1000.0f;
		m_ColorTransforms.push_back(tmp);
	}
	
	glEnable(GL_DEPTH_TEST);
}

void TestLayer7::OnDetach()
{
	LOG_INFO("TestLayer7 detached");
}

void TestLayer7::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer7 updated");

	HandleUserInput(ts);

	// draw with this command once, to fill the instance buffer:
	static bool first_time = true;
	if (first_time)
	{
		m_NormalMesh->DrawInstances(m_NormalTransforms);
		m_ColorMesh->DrawInstances(m_ColorTransforms);
		first_time = false;
	}

	Renderer::Refresh();

//	Renderer::SetLightPosition(Vec3D(13.0f * sin(0.0005 * m_ElapsedTime), 0.0f, 0.0f));
//	Renderer::SetLightPosition(m_Camera.location);
//	m_NormalTransforms[0].location = Vec3D(13.0f * sin(0.0005 * m_ElapsedTime), 0.0f, 0.0f);

	// render into the shadow map
	Renderer::s_DepthBuffer->Bind();
	glCullFace(GL_FRONT);

	auto shadow_shader = Renderer::s_ShaderLibrary.BindShader(MeshType::SHADOW_MAP);
	m_NormalMesh->Draw();
	m_ColorMesh->Draw();

	Renderer::s_DepthBuffer->Unbind();
	glCullFace(GL_BACK);

	Renderer::BindShader(m_Skybox->GetMeshType());
	m_Skybox->Draw();
	
	// draw the mesh instances
	Renderer::BindShader(m_NormalMesh->GetMeshType());
//	m_NormalMesh->Draw();
	m_NormalMesh->DrawInstances(m_NormalTransforms);

	Renderer::BindShader(m_ColorMesh->GetMeshType());
//	m_ColorMesh->Draw();
	m_ColorMesh->DrawInstances(m_ColorTransforms);

	m_ElapsedTime += ts;
}

void TestLayer7::OnEvent(Event& event)
{
	LOG_INFO("TestLayer7 event received");

	event.Dispatch<sf::Event::EventType::Resized>				(BIND_EVENT_FN(OnWindowResize)); // this should be removed when this is resolved through the renderer
	event.Dispatch<sf::Event::EventType::LostFocus>				(BIND_EVENT_FN(OnLoosingFocus));
	event.Dispatch<sf::Event::EventType::GainedFocus>			(BIND_EVENT_FN(OnGainingFocus));
	event.Dispatch<sf::Event::EventType::KeyPressed>			(BIND_EVENT_FN(OnKeyPressed));
	event.Dispatch<sf::Event::EventType::KeyReleased>			(BIND_EVENT_FN(OnKeyReleased));
	event.Dispatch<sf::Event::EventType::MouseWheelScrolled>	(BIND_EVENT_FN(MouseWheelScrolled));
	event.Dispatch<sf::Event::EventType::MouseButtonPressed>	(BIND_EVENT_FN(OnMouseButtonPressed));
	event.Dispatch<sf::Event::EventType::MouseButtonReleased>	(BIND_EVENT_FN(OnMouseButtonReleased));
}


/***********************************
***** Private member functions *****
************************************/

bool TestLayer7::OnWindowResize(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Window Resize event captured in TestLayer7: width - {0}, height - {1}", event.size.width, event.size.height);
	return false;
}

bool TestLayer7::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer7 received LostFocus event");
	return false;
}

bool TestLayer7::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer7 received GainFocus event");
	return false;
}

bool TestLayer7::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer7 received OnTextEntered event");
	return false;
}

bool TestLayer7::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	if (event.key.code == sf::Keyboard::Key::Escape)
	{
		Application::Get().Close();
	}

	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);

	return false;
}

bool TestLayer7::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer7 received OnKeyReleased event");
	return false;
}

bool TestLayer7::MouseWheelScrolled(Event& e)
{
	static float zoom_level = g_InitialZoomLevel;

	sf::Event& event = e.GetEvent();

	zoom_level *= event.mouseWheelScroll.delta > 0 ? 1.25f : 0.8f;
	zoom_level = zoom_level < g_InitialZoomLevel ? g_InitialZoomLevel : zoom_level;
	zoom_level = zoom_level > 128.0f ? 128.0f : zoom_level;

	Renderer::SetZoomLevel(zoom_level);

	LOG_INFO("TestLayer7 received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer7::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer7 received OnMouseButtonPressed event");
	return false;
}

bool TestLayer7::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer7 received OnMouseButtonReleased event");
	return false;
}

bool TestLayer7::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer7 received OnMouseMoved event");
	return false;
}

bool TestLayer7::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer7 received OnMouseEntered event");
	return false;
}

bool TestLayer7::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer7 received OnMouseLeft event");
	return false;
}

void TestLayer7::HandleUserInput(Timestep ts)
{
//	TransformComponent& cam_trf = m_Scene->GetCamera().camera_transform;
//	static TransformComponent cam_trf = { Vec3D(), Identity(1.0f), 1.0f };
	TransformComponent& cam_trf = m_Camera;

	float cam_velocity = 0.01f;
	// moves
	if (Input::IsKeyPressed(sf::Keyboard::Key::W)) { cam_trf.location += ts * cam_velocity * cam_trf.orientation.f3; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::S)) { cam_trf.location -= ts * cam_velocity * cam_trf.orientation.f3; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::A)) { cam_trf.location -= ts * cam_velocity * cam_trf.orientation.f1; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::D)) { cam_trf.location += ts * cam_velocity * cam_trf.orientation.f1; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::R)) { cam_trf.location += ts * cam_velocity * cam_trf.orientation.f2; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::F)) { cam_trf.location -= ts * cam_velocity * cam_trf.orientation.f2; }
	// rotations
	if (Input::IsKeyPressed(sf::Keyboard::Key::Q)) { cam_trf.orientation = Rotation(0.001f * ts, cam_trf.orientation.f3) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::E)) { cam_trf.orientation = Rotation(-0.001f * ts, cam_trf.orientation.f3) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Up)) { cam_trf.orientation = Rotation(0.001f * ts, cam_trf.orientation.f1) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Down)) { cam_trf.orientation = Rotation(-0.001f * ts, cam_trf.orientation.f1) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Left)) { cam_trf.orientation = Rotation(-0.001f * ts, cam_trf.orientation.f2) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Right)) { cam_trf.orientation = Rotation(0.001f * ts, cam_trf.orientation.f2) * cam_trf.orientation; }

	if (Input::IsKeyPressed(sf::Keyboard::Key::Space)) { Renderer::SetLightPosition(m_Camera.location); }

	

	Renderer::SetCamera(cam_trf);
}
