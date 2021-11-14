
#include "TestLayer5.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>
#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/TexturedMesh.h>
#include <core/rendering/drawables/Skybox.h>
#include <core/rendering/drawables/NormalMesh.h>
#include <core/scene/Components.h>

#include <core/GlobalConstants.h>

#include <utils/Vector_3D.h>
#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&TestLayer5::x, this, std::placeholders::_1)

TestLayer5::TestLayer5()
	: Layer("TestLayer5")
{
	LOG_INFO("TestLayer5 constructed");
}

void TestLayer5::OnAttach()
{
	LOG_INFO("TestLayer5 attached");

	// Hello Tetrahedron //
	// Vertices with their own color:
	std::vector<Vec3D> vertices = {
		{-0.05f, -0.05f, 0.0f}, {1.0f, 0.0f, 0.0f},
		{0.05f, -0.05f, 0.0f}, {0.0f, 1.0f, 0.0f},
		{0.0f,  0.05f, 0.0f}, {0.0f, 0.0f, 1.0f},
		{0.0f,  0.0f, -0.07f}, {1.0f, 1.0f, 0.0f}
	};

	std::vector<uint32_t> indices = {
		0, 1, 2,   // first triangle
		2, 1, 3,    // second triangle
		0, 2, 3,	// ...
		1, 0, 3
	};

	m_Tetrahedron = std::shared_ptr<Mesh>(new ColouredMesh(vertices, indices));

	// rectangle
	std::vector<Vec3D> vertices_rect = {
		{-0.05f, -0.05f, 0.0f}, {1.0f, 0.0f, 0.0f},
		{-0.05f,  0.05f, 0.0f}, {0.0f, 1.0f, 0.0f},
		{ 0.05f, -0.05f, 0.0f}, {0.0f, 0.0f, 1.0f},
		{ 0.05f,  0.05f, 0.0f}, {1.0f, 1.0f, 0.0f}
	};

	std::vector<uint32_t> indices_rect = {
		0, 1, 2,   // first triangle
		1, 2, 3    // second triangle
	};

	m_Rectangle = std::shared_ptr<Mesh>(new ColouredMesh(vertices_rect, indices_rect));

	// cube
	std::vector<float> vertices_cube = {
		-0.707107,	-0.707107,	-0.707107,	0,	-1,	0,	1.000,	0.0,
		 0.707107,	-0.707107,	-0.707107,	0,	-1,	0,	0.666,	0.0,
		-0.707107,	-0.707107,	 0.707107,	0,	-1,	0,	1.000,	0.5,
		 0.707107,	-0.707107,	 0.707107,	0,	-1,	0,	0.666,	0.5,

		-0.707107,	0.707107,	-0.707107,	0,	1,	0,	1.000,	1.0,
		0.707107,	0.707107,	-0.707107,	0,	1,	0,	0.666,	1.0,
		-0.707107,	0.707107,	 0.707107,	0,	1,	0,	1.000,	0.5,
		0.707107,	0.707107,	 0.707107,	0,	1,	0,	0.666,	0.5,

		-0.707107,	-0.707107,	 0.707107,	-1,	0,	0,	0.333,	0.5,
		-0.707107,	-0.707107,	-0.707107,	-1,	0,	0,	0.666,	0.5,
		-0.707107,	0.707107,	 0.707107,	-1,	0,	0,	0.333,	1.0,
		-0.707107,	0.707107,	-0.707107,	-1,	0,	0,	0.666,	1.0,

		0.707107,	-0.707107,	 0.707107,	1,	0,	0,	0.666,	0.0,
		0.707107,	-0.707107,	-0.707107,	1,	0,	0,	0.333,	0.0,
		0.707107,	0.707107,	 0.707107,	1,	0,	0,	0.666,	0.5,
		0.707107,	0.707107,	-0.707107,	1,	0,	0,	0.333,	0.5,

		-0.707107,	-0.707107,	0.707107,	0,	0,	1,	0.333,	0.5,
		0.707107,	-0.707107,	0.707107,	0,	0,	1,	0.000,	0.5,
		-0.707107,	0.707107,	0.707107,	0,	0,	1,	0.333,	1.0,
		0.707107,	0.707107,	0.707107,	0,	0,	1,	0.000,	1.0,

		-0.707107,	-0.707107,	-0.707107,	0,	0,	-1,	0.000,	0.0,
		0.707107,	-0.707107,	-0.707107,	0,	0,	-1,	0.333,	0.0,
		-0.707107,	0.707107,	-0.707107,	0,	0,	-1,	0.000,	0.5,
		0.707107,	0.707107,	-0.707107,	0,	0,	-1,	0.333,	0.5
	};

	std::vector<uint32_t> indices_cube = {
		0,	1,	2,
		1,	3,	2,
		4,	6,	5,
		5,	6,	7,
		8,	10,	9,
		9,	10,	11,
		12,	13,	14,
		13,	15,	14,
		16,	17,	18,
		17,	19,	18,
		20,	22,	21,
		21,	22,	23
	};

	m_Cube = std::shared_ptr<Mesh>(new NormalMesh(vertices_cube, indices_cube, "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/all_in_one.png"));

	// textured
	std::vector<float> vertices_textured = {
		-0.3f, -0.3f, 0.0f, 0.00f, 0.0f,
		-0.1f, -0.3f, 0.0f, 0.33f, 0.0f,
		 0.1f, -0.3f, 0.0f, 0.67f, 0.0f,
		 0.3f, -0.3f, 0.0f, 1.00f, 0.0f,

		-0.3f, -0.1f, 0.0f, 0.00f, 0.33f,
		-0.1f, -0.1f, 0.0f, 0.33f, 0.33f,
		 0.1f, -0.1f, 0.0f, 0.67f, 0.33f,
		 0.3f, -0.1f, 0.0f, 1.00f, 0.33f,

		-0.3f,  0.1f, 0.0f, 0.00f, 0.67f,
		-0.1f,  0.1f, 0.0f, 0.33f, 0.67f,
		 0.1f,  0.1f, 0.0f, 0.67f, 0.67f,
		 0.3f,  0.1f, 0.0f, 1.00f, 0.67f,

		-0.3f,  0.3f, 0.0f, 0.00f, 1.0f,
		-0.1f,  0.3f, 0.0f, 0.33f, 1.0f,
		 0.1f,  0.3f, 0.0f, 0.67f, 1.0f,
		 0.3f,  0.3f, 0.0f, 1.00f, 1.0f
	};

	std::vector<uint32_t> indices_textured = {
		0, 1, 4,   // first quad
		5, 4, 1,   // 
		1, 2, 5,   // second quad
		6, 5, 2,   // 
		2, 3, 6,   // third quad
		7, 6, 3,   // 

		4, 5, 8, // second line
		9, 8, 5,  // 
		5, 6, 9, // 
		10, 9, 6,  // 
		6, 7, 10,  // 
		11, 10, 7,   // 

		8, 9, 12, // third line
		13, 12, 9,  // 
		9, 10, 13, // 
		14, 13, 10,  // 
		10, 11, 14,  // 
		15, 14, 11   // 
	};

//	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/saucer_texture.png"));
	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/all_in_one.png"));
//	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skybox_test_front.png"));
//	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, m_Framebuffer->GetColorAttachment()));
//	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, m_Framebuffer->GetDepthAttachment()));
//	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, m_Depthbuffer->GetDepthAttachment()));

	// skybox
	auto skybox_vertices = Skybox::CreateSkyboxVertexData(10);
	auto skybox_indices = Skybox::CreateSkyboxIndexData(10);
	std::vector<std::string> textureFilenames = {
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/skybox_test_front.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/skybox_test_back.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/skybox_test_left.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/skybox_test_right.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/skybox_test_up.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/skybox_test_down.png"

//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/mountains_front.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/mountains_back.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/mountains_left.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/mountains_right.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/mountains_up.png",
//		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/mountains_down.png"

		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space_front.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space_back.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space_left.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space_right.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space_up.png",
		"D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space_down.png"
	};

	m_Skybox = std::shared_ptr<Mesh>(new Skybox(skybox_vertices, skybox_indices, textureFilenames));

	// construct the scene and the entities
	TransformComponent cam_trf;
	cam_trf.location = Vec3D({ 0.0f,0.0f,-1.0f });
	cam_trf.orientation = Identity(1.0f);
	//cam_trf.scale = 1.0f;

	TransformComponent tetrahedron_trf;
//	tetrahedron_trf.location = Vec3D({ -0.2f, 0.2f, -0.3f });
	tetrahedron_trf.location = Vec3D({ 0.0f, 0.0f, 0.0f });
	tetrahedron_trf.orientation = Identity(1.0f);
	tetrahedron_trf.scale = 1.0f;

	TransformComponent rect_trf;
	rect_trf.location = Vec3D({ 0.1f, 0.0f, -0.3f });
	rect_trf.orientation = Identity(1.0f);
	rect_trf.scale = 1.0f;

	TransformComponent cube_trf;
	cube_trf.location = Vec3D({ 0.3f, 0.4f, -0.1f });
//	cube_trf.location = Vec3D(-0.3f, 2.1f, 0.3f);
	cube_trf.orientation = Identity(1.0f);
	cube_trf.scale = 0.2f;

	TransformComponent textured_trf;
	textured_trf.location = Vec3D({ 0.0f, 3.0f, 0.0f });
//	textured_trf.orientation = Identity(1.0f);
	textured_trf.orientation = Rotation(1.57079633f, Vec3D({ 1.0f, 0.0f, 0.0f }));
	textured_trf.scale = 10.0f;

	TransformComponent skybox_trf;
	skybox_trf.location = Vec3D({ 0.0f, 0.0f, 0.0f });
	skybox_trf.orientation = Identity(1.0f);
	skybox_trf.scale = 1.0f;

	m_Scene = std::shared_ptr<Scene>(new Scene());

	m_CameraEntity = m_Scene->CreateEntity("Camera");
	m_TetrahedronEntity = m_Scene->CreateEntity("Tetrahedron");
	m_RectangleEntity = m_Scene->CreateEntity("Rectangle");
	m_CubeEntity = m_Scene->CreateEntity("Cube");
	m_TexturedEntity = m_Scene->CreateEntity("Textured");
	m_SkyboxEntity = m_Scene->CreateEntity("Skybox");

	m_CameraEntity.AddComponent<TransformComponent>(cam_trf);
	m_TetrahedronEntity.AddComponent<TransformComponent>(tetrahedron_trf);
	m_RectangleEntity.AddComponent<TransformComponent>(rect_trf);
	m_CubeEntity.AddComponent<TransformComponent>(cube_trf);
	m_TexturedEntity.AddComponent<TransformComponent>(textured_trf);
	m_SkyboxEntity.AddComponent<TransformComponent>(skybox_trf);

	MeshComponent tetrahedron_mesh_component; tetrahedron_mesh_component.meshPtr = m_Tetrahedron;
	MeshComponent rectangle_mesh_component; rectangle_mesh_component.meshPtr = m_Rectangle;
	MeshComponent normal_mesh_component; normal_mesh_component.meshPtr = m_Cube;
	MeshComponent textured_mesh_component; textured_mesh_component.meshPtr = m_Textured;
	MeshComponent skybox_mesh_component; skybox_mesh_component.meshPtr = m_Skybox;
	m_TetrahedronEntity.AddComponent<MeshComponent>(tetrahedron_mesh_component);
	m_RectangleEntity.AddComponent<MeshComponent>(rectangle_mesh_component);
	m_CubeEntity.AddComponent<MeshComponent>(normal_mesh_component);
	m_TexturedEntity.AddComponent<MeshComponent>(textured_mesh_component);
	m_SkyboxEntity.AddComponent<MeshComponent>(skybox_mesh_component);

	Renderer::SetLightPosition(tetrahedron_trf.location);
	glEnable(GL_DEPTH_TEST);
}

void TestLayer5::OnDetach()
{
	LOG_INFO("TestLayer5 detached");
}

void TestLayer5::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer5 updated");

	HandleUserInput(ts);
	
	Renderer::Refresh();

	// render into the depth buffer
	{
		glCullFace(GL_FRONT);
		// TransformComponent& camera_trf = m_CameraEntity.GetComponent<TransformComponent>();
		// Renderer::SetLightPosition(camera_trf.location);
		Renderer::SetLightPosition(Vec3D(sin(0.001f * m_ElapsedTime), 0.0f, 0.0f));

		//TransformComponent& tetr_trf = m_TetrahedronEntity.GetComponent<TransformComponent>();
		//tetr_trf.location = Vec3D(-0.1f, 1.1f, 0.1f);
		//tetr_trf.orientation = tetr_trf.orientation * Rotation(0.001f * ts, Vec3D({ 1.0f, 0.0f, 0.0f }));
		//Renderer::DrawToShadowMap(m_TetrahedronEntity);

		TransformComponent& rect_trf = m_RectangleEntity.GetComponent<TransformComponent>();
		rect_trf.orientation = rect_trf.orientation * Rotation(0.001f * ts, Vec3D({ 0.0f, -1.0f, 0.0f }));
		//	rect_trf.location = Vec3D({ 0.0f, 0.7f*cos(0.001f * m_ElapsedTime), 0.0f });
		rect_trf.location = Vec3D({ 0.1f, 0.7f * cos(0.001f * m_ElapsedTime), 0.7f * sin(0.001f * m_ElapsedTime) - 0.0f });
		Renderer::DrawToShadowMap(m_RectangleEntity);

		TransformComponent& textured_trf = m_TexturedEntity.GetComponent<TransformComponent>();
		textured_trf.orientation = Rotation(1.57079633f, Vec3D({ -1.0f, 0.0f, 0.0f }));
		textured_trf.location = Vec3D({ 0.0f, 3.0f, 0.0f });
		Renderer::DrawToShadowMap(m_TexturedEntity);

		textured_trf.orientation = Rotation(0.0002f * m_ElapsedTime, Vec3D({ 0.0f, 1.0f, 0.0f })) * Rotation(1.7f, Vec3D({ -1.0f, 0.0f, 0.0f }));
		textured_trf.location = Vec3D({ 3.0f, 1.5f, 2.5f });
		Renderer::DrawToShadowMap(m_TexturedEntity);

		TransformComponent& cube_trf = m_CubeEntity.GetComponent<TransformComponent>();
		for (int i = 0; i < 3; i++)
		{
			cube_trf.orientation = Rotation(0.0002f * m_ElapsedTime, Vec3D({ 0.0f, 1.0f, -1.0f }));
			cube_trf.location = Vec3D(-0.1f, 1.1f + i * 0.3, i * 0.1f + 0.1f);
			cube_trf.scale = 0.1f + 0.1f * i;
			Renderer::DrawToShadowMap(m_CubeEntity);
		}
		// Renderer::DrawToShadowMap(m_CubeEntity);

		textured_trf.location = Vec3D({ 0.0f, 3.0f, 0.0f });
		// m_Depthbuffer->Unbind();

		glCullFace(GL_BACK);
	}

	// render the scene
	{
		Renderer::Draw(m_SkyboxEntity);

		TransformComponent& tetr_trf = m_TetrahedronEntity.GetComponent<TransformComponent>();
		tetr_trf.location = Vec3D(sin(0.001f * m_ElapsedTime), 0.0f, 0.0f);
		tetr_trf.orientation = tetr_trf.orientation * Rotation(0.001f * ts, Vec3D({ 1.0f, 0.0f, 0.0f }));
		Renderer::Draw(m_TetrahedronEntity);

		TransformComponent& rect_trf = m_RectangleEntity.GetComponent<TransformComponent>();
		rect_trf.orientation = rect_trf.orientation * Rotation(0.001f * ts, Vec3D({ 0.0f, -1.0f, 0.0f }));
		//	rect_trf.location = Vec3D({ 0.0f, 0.7f*cos(0.001f * m_ElapsedTime), 0.0f });
		rect_trf.location = Vec3D({ 0.1f, 0.7f * cos(0.001f * m_ElapsedTime), 0.7f * sin(0.001f * m_ElapsedTime) - 0.0f });
		Renderer::Draw(m_RectangleEntity);

//		for (int i = 0; i < 3; i++)
//		{
//			rect_trf.location = Vec3D(-0.1f, 2.1f, i * 0.2f + 0.1f);
//			Renderer::Draw(m_RectangleEntity);
//		}

		TransformComponent& textured_trf = m_TexturedEntity.GetComponent<TransformComponent>();
		textured_trf.orientation = Rotation(1.57079633f, Vec3D({ -1.0f, 0.0f, 0.0f }));
		textured_trf.location = Vec3D({ 0.0f, 3.0f, 0.0f });
		Renderer::Draw(m_TexturedEntity);

		textured_trf.orientation = Rotation(0.0002f * m_ElapsedTime, Vec3D({ 0.0f, 1.0f, 0.0f })) * Rotation(1.7f, Vec3D({ -1.0f, 0.0f, 0.0f }));
		textured_trf.location = Vec3D({ 3.0f, 1.5f, 2.5f });
		Renderer::Draw(m_TexturedEntity);

		TransformComponent& cube_trf = m_CubeEntity.GetComponent<TransformComponent>();
		for (int i = 0; i < 3; i++)
		{
			cube_trf.orientation = Rotation(0.0002f * m_ElapsedTime, Vec3D({ 0.0f, 1.0f, -1.0f }));
			cube_trf.location = Vec3D(-0.1f, 1.1f + i * 0.3, i * 0.1f + 0.1f);
			cube_trf.scale = 0.1f + 0.1f * i;
			Renderer::Draw(m_CubeEntity);
		}
		// Renderer::Draw(m_CubeEntity);
	}

	m_ElapsedTime += ts;
}

void TestLayer5::OnEvent(Event& event)
{
	LOG_INFO("TestLayer5 event received");

//	event.Dispatch<sf::Event::EventType::Resized>				(BIND_EVENT_FN(OnWindowResize)); // this should be removed when this is resolved through the renderer
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

bool TestLayer5::OnWindowResize(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Window Resize event captured in TestLayer5: width - {0}, height - {1}", event.size.width, event.size.height);
	return false;
}

bool TestLayer5::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer5 received LostFocus event");
	return false;
}

bool TestLayer5::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer5 received GainFocus event");
	return false;
}

bool TestLayer5::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer5 received OnTextEntered event");
	return false;
}

bool TestLayer5::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();

	if(event.key.code == sf::Keyboard::Key::Add)
	{
		TransformComponent& trf = m_TetrahedronEntity.GetComponent<TransformComponent>();
		trf.scale *= 1.1f;
	}
	if (event.key.code == sf::Keyboard::Key::Subtract)
	{
		TransformComponent& trf = m_TetrahedronEntity.GetComponent<TransformComponent>();
		trf.scale /= 1.1f;
	}

	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);
	return false;
}

bool TestLayer5::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer5 received OnKeyReleased event");
	return false;
}

bool TestLayer5::MouseWheelScrolled(Event& e)
{
	static float zoom_level = g_InitialZoomLevel;

	sf::Event& event = e.GetEvent();

	zoom_level *= event.mouseWheelScroll.delta > 0 ? 1.25f : 0.8f;
	zoom_level = zoom_level < g_InitialZoomLevel ? g_InitialZoomLevel : zoom_level;
	zoom_level = zoom_level > 128.0f ? 128.0f : zoom_level;

	Renderer::SetZoomLevel(zoom_level);

	LOG_INFO("TestLayer5 received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer5::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer5 received OnMouseButtonPressed event");
	return false;
}

bool TestLayer5::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer5 received OnMouseButtonReleased event");
	return false;
}

bool TestLayer5::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer5 received OnMouseMoved event");
	return false;
}

bool TestLayer5::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer5 received OnMouseEntered event");
	return false;
}

bool TestLayer5::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer5 received OnMouseLeft event");
	return false;
}

void TestLayer5::HandleUserInput(Timestep ts)
{
	TransformComponent& cam_trf = m_CameraEntity.GetComponent<TransformComponent>();

	// moves
	if (Input::IsKeyPressed(sf::Keyboard::Key::W)) { cam_trf.location += ts * 0.001f * cam_trf.orientation.f3; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::S)) { cam_trf.location -= ts * 0.001f * cam_trf.orientation.f3; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::A)) { cam_trf.location -= ts * 0.001f * cam_trf.orientation.f1; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::D)) { cam_trf.location += ts * 0.001f * cam_trf.orientation.f1; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::R)) { cam_trf.location += ts * 0.001f * cam_trf.orientation.f2; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::F)) { cam_trf.location -= ts * 0.001f * cam_trf.orientation.f2; }
	// rotations
	if (Input::IsKeyPressed(sf::Keyboard::Key::Q)) { cam_trf.orientation = Rotation(0.001f * ts, cam_trf.orientation.f3) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::E)) { cam_trf.orientation = Rotation(-0.001f * ts, cam_trf.orientation.f3) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Up)) { cam_trf.orientation = Rotation(0.001f * ts, cam_trf.orientation.f1) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Down)) { cam_trf.orientation = Rotation(-0.001f * ts, cam_trf.orientation.f1) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Left)) { cam_trf.orientation = Rotation(-0.001f * ts, cam_trf.orientation.f2) * cam_trf.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Right)) { cam_trf.orientation = Rotation(0.001f * ts, cam_trf.orientation.f2) * cam_trf.orientation; }

	Renderer::SetCamera(cam_trf);
}
