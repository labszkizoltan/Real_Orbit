
#include "TestLayer8.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>

#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/Skybox.h>
#include <core/rendering/drawables/NormalMesh.h>

#include <core/scene/SceneSerializer.h>
#include <core/scene/Components.h>

#include <core/GlobalConstants.h>

#include <utils/Vector_3D.h>
#include <glad/glad.h>

#include "scene_descriptions.h"

#define BIND_EVENT_FN(x) std::bind(&TestLayer8::x, this, std::placeholders::_1)

OGLBufferData ParseVertexFile(const std::string& filename)
{
	OGLBufferData result;
	std::ifstream myfile(filename.c_str());
	if (!myfile.is_open())
	{
		LOG_CORE_WARN("TestLayer8::ParseVertexFile() was unable to open file: {0}", filename);
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
		LOG_CORE_WARN("TestLayer8::ParseVertexFile(): vertex or index data missing from file: ", filename);

	return result;
}


TestLayer8::TestLayer8()
	: Layer("TestLayer8")
{
	LOG_INFO("TestLayer8 constructed");
}

void TestLayer8::OnAttach()
{
	LOG_INFO("TestLayer8 attached");

	m_Scene = std::make_shared<Scene>();

	SceneSerializer serializer(m_Scene);
	serializer.DeSerialize_file("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/scenes/test_scene_3.yaml");

	m_SceneRenderer.SetScene(m_Scene);

	glEnable(GL_DEPTH_TEST);
}

void TestLayer8::OnDetach()
{
	LOG_INFO("TestLayer8 detached");
}

void TestLayer8::OnUpdate(Timestep ts)
{
	HandleUserInput(ts);

	m_SceneRenderer.RenderScene();

	m_ElapsedTime += ts;
}

void TestLayer8::OnEvent(Event& event)
{
	LOG_INFO("TestLayer8 event received");

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

bool TestLayer8::OnWindowResize(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Window Resize event captured in TestLayer8: width - {0}, height - {1}", event.size.width, event.size.height);
	return false;
}

bool TestLayer8::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer8 received LostFocus event");
	return false;
}

bool TestLayer8::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer8 received GainFocus event");
	return false;
}

bool TestLayer8::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer8 received OnTextEntered event");
	return false;
}

bool TestLayer8::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	if (event.key.code == sf::Keyboard::Key::Escape)
	{
		Application::Get().Close();
	}

	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);

	return false;
}

bool TestLayer8::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer8 received OnKeyReleased event");
	return false;
}

bool TestLayer8::MouseWheelScrolled(Event& e)
{
	static float zoom_level = g_InitialZoomLevel;

	sf::Event& event = e.GetEvent();

	zoom_level *= event.mouseWheelScroll.delta > 0 ? 1.25f : 0.8f;
	zoom_level = zoom_level < g_InitialZoomLevel ? g_InitialZoomLevel : zoom_level;
	zoom_level = zoom_level > 128.0f ? 128.0f : zoom_level;

	Renderer::SetZoomLevel(zoom_level);

	LOG_INFO("TestLayer8 received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer8::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer8 received OnMouseButtonPressed event");
	return false;
}

bool TestLayer8::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer8 received OnMouseButtonReleased event");
	return false;
}

bool TestLayer8::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer8 received OnMouseMoved event");
	return false;
}

bool TestLayer8::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer8 received OnMouseEntered event");
	return false;
}

bool TestLayer8::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer8 received OnMouseLeft event");
	return false;
}

void TestLayer8::HandleUserInput(Timestep ts)
{
	TransformComponent& cam_trf = m_Scene->GetCamera();
//	static TransformComponent cam_trf = TransformComponent();
//	TransformComponent& cam_trf = m_Camera;

	static float cam_velocity = 0.001f;
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

	if (Input::IsKeyPressed(sf::Keyboard::Key::Add)) { cam_velocity *= 1.1f; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Subtract)) { cam_velocity /= 1.1f; }

	if (Input::IsKeyPressed(sf::Keyboard::Key::Space))
	{
		TransformComponent& light_trf = m_Scene->GetLight();
		light_trf.location = cam_trf.location;
	}

//	Renderer::SetCamera(cam_trf);
}
