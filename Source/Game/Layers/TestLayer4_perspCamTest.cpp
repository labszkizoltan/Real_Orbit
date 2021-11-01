
#include "TestLayer4_perspCamTest.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>
#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/TexturedMesh.h>
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

	// Create the framebuffer:
	FrameBufferSpecification fbspec;
	fbspec.Width = 256;
	fbspec.Height = 256;
	m_Framebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(fbspec));

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

	// textured rectangle
	std::vector<float> vertices_textured = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
	};

	std::vector<uint32_t> indices_textured = {
		0, 1, 2,   // first triangle
		1, 2, 3    // second triangle
	};
	
//	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/saucer_texture.png"));
//	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/all_in_one.png"));
//	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, m_Framebuffer->GetColorAttachment()));
	m_Textured = std::shared_ptr<Mesh>(new TexturedMesh(vertices_textured, indices_textured, m_Framebuffer->GetDepthAttachment()));

	// construct the scene and the entities
	TransformComponent cam_trf;
	cam_trf.location = Vec3D({ 0.0f,0.0f,-1.0f });
	cam_trf.orientation = Identity(1.0f);
	//cam_trf.scale = 1.0f;

	TransformComponent tetrahedron_trf;
	tetrahedron_trf.location = Vec3D({ -0.2f, 0.0f, -0.5f });
	tetrahedron_trf.orientation = Identity(1.0f);
	tetrahedron_trf.scale = 1.0f;

	TransformComponent rect_trf;
	rect_trf.location = Vec3D({ 0.3f, 0.0f, -0.3f });
	rect_trf.orientation = Identity(1.0f);
	rect_trf.scale = 1.0f;

	TransformComponent textured_trf;
	textured_trf.location = Vec3D({ 0.0f, 0.0f, 0.0f });
	textured_trf.orientation = Identity(1.0f);
	textured_trf.scale = 1.0f;

	m_Scene = std::shared_ptr<Scene>(new Scene());

	m_CameraEntity = m_Scene->CreateEntity("Camera");
	m_TetrahedronEntity = m_Scene->CreateEntity("Tetrahedron");
	m_RectangleEntity = m_Scene->CreateEntity("Rectangle");
	m_TexturedEntity = m_Scene->CreateEntity("Textured");

	m_CameraEntity.AddComponent<TransformComponent>(cam_trf);
	m_TetrahedronEntity.AddComponent<TransformComponent>(tetrahedron_trf);
	m_RectangleEntity.AddComponent<TransformComponent>(rect_trf);
	m_TexturedEntity.AddComponent<TransformComponent>(textured_trf);

	MeshComponent tetrahedron_mesh_component; tetrahedron_mesh_component.meshPtr = m_Tetrahedron;
	MeshComponent rectangle_mesh_component; rectangle_mesh_component.meshPtr = m_Rectangle;
	MeshComponent textured_mesh_component; textured_mesh_component.meshPtr = m_Textured;
	m_TetrahedronEntity.AddComponent<MeshComponent>(tetrahedron_mesh_component);
	m_RectangleEntity.AddComponent<MeshComponent>(rectangle_mesh_component);
	m_TexturedEntity.AddComponent<MeshComponent>(textured_mesh_component);


	glEnable(GL_DEPTH_TEST);
}

void TestLayer4::OnDetach()
{
	LOG_INFO("TestLayer4 detached");
}

void TestLayer4::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer4 updated");

	HandleUserInput(ts);

	// render into the frame buffer
	{
//		TransformComponent& cam_trf = m_CameraEntity.GetComponent<TransformComponent>();
//		TransformComponent mirrored_cam_trf = cam_trf;
//		mirrored_cam_trf.location.z = -cam_trf.location.z;
//		mirrored_cam_trf.orientation.f1.z = -cam_trf.orientation.f1.z;
//		mirrored_cam_trf.orientation.f2.z = -cam_trf.orientation.f2.z;
//		mirrored_cam_trf.orientation.f3.z = -cam_trf.orientation.f3.z;
//
//		Renderer::SetCamera(mirrored_cam_trf);

		m_Framebuffer->Bind();
		glClearColor(0.9f, 0.05f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		TransformComponent& tetr_trf = m_TetrahedronEntity.GetComponent<TransformComponent>();
		tetr_trf.orientation = tetr_trf.orientation * Rotation(0.001f * ts, Vec3D({ 1.0f, 0.0f, 0.0f }));
		Renderer::Draw(m_TetrahedronEntity);

		TransformComponent& rect_trf = m_RectangleEntity.GetComponent<TransformComponent>();
		rect_trf.orientation = rect_trf.orientation * Rotation(0.001f * ts, Vec3D({ 0.0f, -1.0f, 0.0f }));
		rect_trf.location = Vec3D({ 0.5f, 0.2f * sin(0.005f * m_ElapsedTime), 0.0f });
		Renderer::Draw(m_RectangleEntity);

		m_Framebuffer->Unbind();

//		Renderer::SetCamera(cam_trf);
	}

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

	Renderer::Draw(m_TexturedEntity);


	m_ElapsedTime += ts;
}

void TestLayer4::OnEvent(Event& event)
{
	LOG_INFO("TestLayer4 event received");

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

bool TestLayer4::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer4 received OnKeyReleased event");
	return false;
}

bool TestLayer4::MouseWheelScrolled(Event& e)
{
	static float zoom_level = 1.0f;

	sf::Event& event = e.GetEvent();

	zoom_level *= event.mouseWheelScroll.delta > 0 ? 2.0f : 0.5f;
	zoom_level = zoom_level < 1.0f ? 1.0f : zoom_level;
	zoom_level = zoom_level > 128.0f ? 128.0f : zoom_level;

	Renderer::SetZoomLevel(zoom_level);

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

void TestLayer4::HandleUserInput(Timestep ts)
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
