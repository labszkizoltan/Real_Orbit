
#include "TestLayer2.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>
#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>

#include <utils/Vector_3D.h>
#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&TestLayer2::x, this, std::placeholders::_1)

TestLayer2::TestLayer2()
	: Layer("TestLayer2")
{
	LOG_INFO("TestLayer2 constructed");
}

void TestLayer2::OnAttach()
{
	LOG_INFO("TestLayer2 attached");

	m_Shader = std::shared_ptr<Shader>(
		new Shader(
			ParseShader("Source/core/rendering/shader_source_files/basic_3D_vertex_shd.glsl"),
			ParseShader("Source/core/rendering/shader_source_files/basic_3D_fragment_shd.glsl")
		)
	);

	m_Shader->Bind();
	m_Shader->UploadUniformFloat("aspect_ratio", 1.0f);
	m_Shader->UploadUniformFloat("rotation_angle", 0.0f);
	m_Shader->UploadUniformFloat("offset_x", 0.0f);
	m_Shader->UploadUniformFloat("offset_y", 0.0f);

	// Hello Tetrahedron //
	// Vertices with their own color:
	std::vector<Vec3D> vertices = {
		{-0.05f, -0.05f, 0.0f}, {1.0f, 0.0f, 0.0f},
		{0.05f, -0.05f, 0.0f}, {0.0f, 1.0f, 0.0f},
		{0.0f,  0.05f, 0.0f}, {0.0f, 0.0f, 1.0f},
		{0.0f,  0.0f, -0.7f}, {1.0f, 1.0f, 0.0f}
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

	glEnable(GL_DEPTH_TEST);
}

void TestLayer2::OnDetach()
{
	LOG_INFO("TestLayer2 detached");
}

void TestLayer2::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer2 updated");
	if (Input::IsKeyPressed(sf::Keyboard::Key::Left)) { LOG_INFO("TestLayer2 OnUpdate: left arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Right)) { LOG_INFO("TestLayer2 OnUpdate: right arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Up)) { LOG_INFO("TestLayer2 OnUpdate: Up arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Down)) { LOG_INFO("TestLayer2 OnUpdate: down arrow is pressed"); }

	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Left)) { LOG_INFO("TestLayer2 OnUpdate: Left mouse button is pressed"); }
	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Right)) { LOG_INFO("TestLayer2 OnUpdate: Right mouse button is pressed"); }
	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Middle))
	{
		LOG_INFO("TestLayer2 OnUpdate: Middle mouse button is pressed");
		sf::Vector2i mouse_pos = Input::GetMousePosition();
		LOG_INFO("TestLayer2 OnUpdate: window mouse position: x: {0}, y: {1}", mouse_pos.x, mouse_pos.y);
		sf::Vector2i global_mouse_pos = Input::GetGlobalMousePosition();
		LOG_INFO("TestLayer2 OnUpdate: window mouse position: x: {0}, y: {1}", global_mouse_pos.x, global_mouse_pos.y);
	}


	// Render
	glClearColor(0.0f, 0.05f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	int grid_size = 10;
	for (int i = 1 - grid_size; i < grid_size; i++)
	{
		for (int j = 1 - grid_size; j < grid_size; j++)
		{
			if ((i + j) % 2 == 0)
			{
//				m_Shader->UploadUniformFloat("rotation_angle", m_ElapsedTime / 500.0f);
//				m_Shader->UploadUniformFloat("offset_x", (float)i / (float)grid_size);
//				m_Shader->UploadUniformFloat("offset_y", (float)j / (float)grid_size);
//				m_Tetrahedron->Draw();
				Renderer::Draw(m_Tetrahedron.get(), m_ElapsedTime / 500.0f, (float)i / (float)grid_size, (float)j / (float)grid_size);
			}
			else
			{
				m_Shader->Bind();
				m_Shader->UploadUniformFloat("rotation_angle", -m_ElapsedTime/500.0f);
				m_Shader->UploadUniformFloat("offset_x", (float)i / (float)grid_size);
				m_Shader->UploadUniformFloat("offset_y", (float)j / (float)grid_size);
				m_Rectangle->Draw();
			}
		}
	}

	m_ElapsedTime += ts;
}

void TestLayer2::OnEvent(Event& event)
{
	LOG_INFO("TestLayer2 event received");

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

bool TestLayer2::OnWindowResize(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Window Resize event captured in TestLayer2: width - {0}, height - {1}", event.size.width, event.size.height);
	m_Shader->Bind();
	m_Shader->UploadUniformFloat("aspect_ratio", (float)event.size.width/(float)event.size.height);
	return false;
}

bool TestLayer2::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer2 received LostFocus event");
	return false;
}

bool TestLayer2::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer2 received GainFocus event");
	return false;
}

bool TestLayer2::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer2 received OnTextEntered event");
	return false;
}

bool TestLayer2::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);
	return false;
}

bool TestLayer2::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer2 received OnKeyReleased event");
	return false;
}

bool TestLayer2::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer2 received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer2::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseButtonPressed event");
	return false;
}

bool TestLayer2::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseButtonReleased event");
	return false;
}

bool TestLayer2::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseMoved event");
	return false;
}

bool TestLayer2::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseEntered event");
	return false;
}

bool TestLayer2::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseLeft event");
	return false;
}
