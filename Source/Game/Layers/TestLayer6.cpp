
#include "TestLayer6.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>

#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/TexturedMesh.h>
#include <core/rendering/drawables/Skybox.h>
#include <core/rendering/drawables/NormalMesh.h>

#include <core/scene/SceneSerializer.h>
#include <core/scene/Components.h>

#include <core/GlobalConstants.h>

#include <utils/Vector_3D.h>
#include <glad/glad.h>

#include "scene_descriptions.h"

#define BIND_EVENT_FN(x) std::bind(&TestLayer6::x, this, std::placeholders::_1)

TestLayer6::TestLayer6()
	: Layer("TestLayer6")
{
	LOG_INFO("TestLayer6 constructed");
}

void TestLayer6::OnAttach()
{
	LOG_INFO("TestLayer6 attached");

	m_Scene = std::shared_ptr<Scene>(new Scene());

	SceneSerializer serializer(m_Scene);
//	serializer.DeSerialize_text(scene_string);
	serializer.DeSerialize_file("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/scenes/test_scene_2.yaml");

	m_SceneUpdater.SetScene(m_Scene);
	m_SceneRenderer.SetScene(m_Scene);

	glEnable(GL_DEPTH_TEST);
}

void TestLayer6::OnDetach()
{
	LOG_INFO("TestLayer6 detached");
}

void TestLayer6::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer6 updated");

	HandleUserInput(ts);
	
	Renderer::Refresh();

	m_SceneRenderer.RenderScene();

	// something like this could be next
	m_SceneUpdater.UpdateScene(ts);

	auto& light_comp = m_Scene->GetLight();
	light_comp.light_transform.location = light_comp.light_transform.location + Vec3D(0.1f*sin(0.0005f * m_ElapsedTime), 0.0f, 0.0f);

	m_ElapsedTime += ts;
}

void TestLayer6::OnEvent(Event& event)
{
	LOG_INFO("TestLayer6 event received");

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

bool TestLayer6::OnWindowResize(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Window Resize event captured in TestLayer6: width - {0}, height - {1}", event.size.width, event.size.height);
	return false;
}

bool TestLayer6::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer6 received LostFocus event");
	return false;
}

bool TestLayer6::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer6 received GainFocus event");
	return false;
}

bool TestLayer6::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer6 received OnTextEntered event");
	return false;
}

bool TestLayer6::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();

	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);
	return false;
}

bool TestLayer6::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer6 received OnKeyReleased event");
	return false;
}

bool TestLayer6::MouseWheelScrolled(Event& e)
{
	static float zoom_level = g_InitialZoomLevel;

	sf::Event& event = e.GetEvent();

	zoom_level *= event.mouseWheelScroll.delta > 0 ? 1.25f : 0.8f;
	zoom_level = zoom_level < g_InitialZoomLevel ? g_InitialZoomLevel : zoom_level;
	zoom_level = zoom_level > 128.0f ? 128.0f : zoom_level;

	Renderer::SetZoomLevel(zoom_level);

	LOG_INFO("TestLayer6 received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer6::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer6 received OnMouseButtonPressed event");
	return false;
}

bool TestLayer6::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer6 received OnMouseButtonReleased event");
	return false;
}

bool TestLayer6::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer6 received OnMouseMoved event");
	return false;
}

bool TestLayer6::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer6 received OnMouseEntered event");
	return false;
}

bool TestLayer6::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer6 received OnMouseLeft event");
	return false;
}

void TestLayer6::HandleUserInput(Timestep ts)
{
	TransformComponent& cam_trf = m_Scene->GetCamera().camera_transform;

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
