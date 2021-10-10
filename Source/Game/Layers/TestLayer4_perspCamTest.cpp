
#include "TestLayer4_perspCamTest.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>
#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/scene/Components.h>

#include <utils/Vector_3D.h>
#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&TestLayer4::x, this, std::placeholders::_1)

TestLayer4::TestLayer4()
	: Layer("TestLayer4")
{
	LOG_INFO("TestLayer4 constructed");
}

void TestLayer4::OnAttach()
{
	LOG_INFO("TestLayer4 attached");

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


	// construct the scene and the entities
	TransformComponent cam_trf;
	cam_trf.location = Vec3D({ 0.0f,0.0f,-1.0f });
	cam_trf.orientation = Identity(1.0f);

	TransformComponent tetrahedron_trf;
	tetrahedron_trf.location = Vec3D({ 0,0,0 });
	tetrahedron_trf.orientation = Identity(1.0f);

	TransformComponent rect_trf;
	rect_trf.location = Vec3D({ 0.5,0,0 });
	rect_trf.orientation = Identity(1.0f);

	m_Scene = std::shared_ptr<Scene>(new Scene());

	m_CameraEntity = m_Scene->CreateEntity("Camera");
	m_TetrahedronEntity = m_Scene->CreateEntity("Tetrahedron");
	m_RectangleEntity = m_Scene->CreateEntity("Rectangle");

	m_CameraEntity.AddComponent<TransformComponent>(cam_trf);
	m_TetrahedronEntity.AddComponent<TransformComponent>(tetrahedron_trf);
	m_RectangleEntity.AddComponent<TransformComponent>(rect_trf);

	MeshComponent tetrahedron_mesh_component; tetrahedron_mesh_component.meshPtr = m_Tetrahedron;
	MeshComponent rectangle_mesh_component; rectangle_mesh_component.meshPtr = m_Rectangle;
	m_TetrahedronEntity.AddComponent<MeshComponent>(tetrahedron_mesh_component);
	m_RectangleEntity.AddComponent<MeshComponent>(rectangle_mesh_component);

	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
}

void TestLayer4::OnDetach()
{
	LOG_INFO("TestLayer4 detached");
}

void TestLayer4::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer4 updated");

	// Render
	glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	TransformComponent& tetr_trf = m_TetrahedronEntity.GetComponent<TransformComponent>();
	tetr_trf.orientation = tetr_trf.orientation * Rotation(0.001f * ts, Vec3D({1.0f, 0.0f, 0.0f}));
	Renderer::Draw(m_TetrahedronEntity);

	TransformComponent& rect_trf = m_RectangleEntity.GetComponent<TransformComponent>();
	rect_trf.orientation = rect_trf.orientation * Rotation(0.001f * ts, Vec3D({ 0.0f, -1.0f, 0.0f }));
	rect_trf.location = Vec3D({ 0.5f, 0.2f*sin(0.005f * m_ElapsedTime), 0.0f });
	Renderer::Draw(m_RectangleEntity);

	TransformComponent& cam_trf = m_CameraEntity.GetComponent<TransformComponent>();
	if (Input::IsKeyPressed(sf::Keyboard::Key::W))
	{
		LOG_CORE_INFO("W key is pressed");
		cam_trf.location.z += ts * 0.001f;
	}
	if (Input::IsKeyPressed(sf::Keyboard::Key::S))
	{
		LOG_CORE_INFO("S key is pressed");
		cam_trf.location.z -= ts * 0.001f;
	}
	if (Input::IsKeyPressed(sf::Keyboard::Key::A))
	{
		LOG_CORE_INFO("A key is pressed");
		cam_trf.location.x -= ts * 0.001f;
	}
	if (Input::IsKeyPressed(sf::Keyboard::Key::D))
	{
		LOG_CORE_INFO("D key is pressed");
		cam_trf.location.x += ts * 0.001f;
	}
	if (Input::IsKeyPressed(sf::Keyboard::Key::R))
	{
		LOG_CORE_INFO("A key is pressed");
		cam_trf.location.y += ts * 0.001f;
	}
	if (Input::IsKeyPressed(sf::Keyboard::Key::F))
	{
		LOG_CORE_INFO("D key is pressed");
		cam_trf.location.y -= ts * 0.001f;
	}

	Renderer::SetCamera(cam_trf);


	m_ElapsedTime += ts;
}

void TestLayer4::OnEvent(Event& event)
{
	LOG_INFO("TestLayer4 event received");

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

bool TestLayer4::OnWindowResize(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Window Resize event captured in TestLayer4: width - {0}, height - {1}", event.size.width, event.size.height);
	return false;
}

bool TestLayer4::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer4 received LostFocus event");
	return false;
}

bool TestLayer4::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer4 received GainFocus event");
	return false;
}

bool TestLayer4::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer4 received OnTextEntered event");
	return false;
}

bool TestLayer4::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);
	return false;
}

bool TestLayer4::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer4 received OnKeyReleased event");
	return false;
}

bool TestLayer4::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer4 received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer4::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer4 received OnMouseButtonPressed event");
	return false;
}

bool TestLayer4::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer4 received OnMouseButtonReleased event");
	return false;
}

bool TestLayer4::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer4 received OnMouseMoved event");
	return false;
}

bool TestLayer4::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer4 received OnMouseEntered event");
	return false;
}

bool TestLayer4::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer4 received OnMouseLeft event");
	return false;
}
