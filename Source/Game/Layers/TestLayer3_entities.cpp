
#include "TestLayer3_entities.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>
#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>

#include <utils/Vector_3D.h>
#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&TestLayer3::x, this, std::placeholders::_1)

TestLayer3::TestLayer3()
	: Layer("TestLayer3")
{
	LOG_INFO("TestLayer3 constructed");
}

void TestLayer3::OnAttach()
{
	LOG_INFO("TestLayer3 attached");

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

void TestLayer3::OnDetach()
{
	LOG_INFO("TestLayer3 detached");
}

void TestLayer3::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer3 updated");

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
				Renderer::Draw(m_Tetrahedron.get(), m_ElapsedTime / 500.0f, (float)i / (float)grid_size, (float)j / (float)grid_size);
			}
			else
			{
				Renderer::Draw(m_Rectangle.get(), -m_ElapsedTime / 500.0f, (float)i / (float)grid_size, (float)j / (float)grid_size);
			}
		}
	}

	m_ElapsedTime += ts;
}

void TestLayer3::OnEvent(Event& event)
{
	LOG_INFO("TestLayer3 event received");

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

bool TestLayer3::OnWindowResize(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Window Resize event captured in TestLayer3: width - {0}, height - {1}", event.size.width, event.size.height);
	return false;
}

bool TestLayer3::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer3 received LostFocus event");
	return false;
}

bool TestLayer3::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer3 received GainFocus event");
	return false;
}

bool TestLayer3::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer3 received OnTextEntered event");
	return false;
}

bool TestLayer3::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);
	return false;
}

bool TestLayer3::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer3 received OnKeyReleased event");
	return false;
}

bool TestLayer3::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer3 received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer3::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer3 received OnMouseButtonPressed event");
	return false;
}

bool TestLayer3::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer3 received OnMouseButtonReleased event");
	return false;
}

bool TestLayer3::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer3 received OnMouseMoved event");
	return false;
}

bool TestLayer3::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer3 received OnMouseEntered event");
	return false;
}

bool TestLayer3::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer3 received OnMouseLeft event");
	return false;
}
