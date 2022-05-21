
#include "01_MenuLayer.h"
#include <core/Application.h>
#include <Game_v1/GameApp_v1.h>
#include <SFML/Window/Event.hpp>

#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/Skybox.h>
#include <core/rendering/drawables/NormalMesh.h>

#include <core/scene/SceneSerializer.h>
#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>

#include <utils/Vector_3D.h>
#include <glad/glad.h>

//#include "scene_descriptions.h"

#define BIND_EVENT_FN(x) std::bind(&Menu_layer::x, this, std::placeholders::_1)

Menu_layer::Menu_layer()
	: Layer("Menu_layer")
{
	LOG_INFO("Menu_layer constructed");
}

void Menu_layer::OnAttach()
{
	LOG_INFO("Menu_layer attached");

	m_Scene = std::make_shared<Scene>();

	SceneSerializer serializer(m_Scene);
	//	serializer.DeSerialize_file("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/scenes/test_scene_3.yaml");
	serializer.DeSerialize_file("assets/scenes/01_MenuLayer.yaml");

	InitStates();
	m_StateManager.SetState(m_StateManager.m_CurrentState);

	m_SceneRenderer.SetScene(m_Scene);
	m_SceneUpdater.SetScene(m_Scene);

	//	m_FbDisplay.SetTexture(Renderer::GetColorAttachment());
	m_FbDisplay.SetTexture(Renderer::GetBlurredAttachment());

	m_ImgProcessor = std::make_unique<ImageProcessor>();
	m_ImgProcessor->SetMipMapLevel(4);
}

void Menu_layer::OnDetach()
{
	LOG_INFO("Menu_layer detached");
}

void Menu_layer::OnUpdate(Timestep ts)
{
	// if music finished, start again
	if (m_Music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		m_Music.openFromFile("assets/audio/Kevin_MacLeod-Broken_Reality.wav");
		m_Music.play();
		m_Music.setVolume(10.0f);
	}

	HandleUserInput(ts);

	// if the update-render order is swapped, something is un-initialized and the program fails at alpha mesh rendering
	m_SceneRenderer.RenderScene();
	m_SceneUpdater.UpdateScene(m_SimulationSpeed * ts);

	//	m_ImgProcessor->Blur(g_RendererBlurDepthSlot, Renderer::s_BlurBuffer); // this is not working, as expected
	m_ImgProcessor->Blur(g_RendererBrightColAttchSlot, Renderer::s_BlurBuffer);

	//	m_FbDisplay.Draw();
	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, g_RendererBlurredSlot);
	// GameApplication::Game_DrawText("Hello Real Orbit!", Vec3D(10, 10, 0), Vec3D(0.3f, 0.9f, 0.5f), 2.0f);

	m_ElapsedTime += m_SimulationSpeed * ts;
}

void Menu_layer::OnEvent(Event& event)
{
	event.Dispatch<sf::Event::EventType::Resized>(BIND_EVENT_FN(OnWindowResize)); // this should be removed when this is resolved through the renderer
	event.Dispatch<sf::Event::EventType::LostFocus>(BIND_EVENT_FN(OnLoosingFocus));
	event.Dispatch<sf::Event::EventType::GainedFocus>(BIND_EVENT_FN(OnGainingFocus));
	event.Dispatch<sf::Event::EventType::KeyPressed>(BIND_EVENT_FN(OnKeyPressed));
	event.Dispatch<sf::Event::EventType::KeyReleased>(BIND_EVENT_FN(OnKeyReleased));
	event.Dispatch<sf::Event::EventType::MouseWheelScrolled>(BIND_EVENT_FN(MouseWheelScrolled));
	event.Dispatch<sf::Event::EventType::MouseButtonPressed>(BIND_EVENT_FN(OnMouseButtonPressed));
	event.Dispatch<sf::Event::EventType::MouseButtonReleased>(BIND_EVENT_FN(OnMouseButtonReleased));
	//	event.Dispatch<sf::Event::EventType::MouseMoved>			(BIND_EVENT_FN(OnMouseMoved));
}


void Menu_layer::Activate()
{
	m_IsActive = true;
	m_Music.play();
	Renderer::SetZoomLevel(m_ZoomLevel);
}

void Menu_layer::DeActivate()
{
	// Application& app = Application::Get();
	// ((GameApplication*)(&app))->ActitivateLayer(GameLayers::INGAME_LAYER);
	m_Music.pause();
	m_IsActive = false;
}


/***********************************
***** Private member functions *****
************************************/

bool Menu_layer::OnWindowResize(Event& e)
{
	return false;
}

bool Menu_layer::OnLoosingFocus(Event& e)
{
	m_InFocus = false;
	return false;
}

bool Menu_layer::OnGainingFocus(Event& e)
{
	m_InFocus = true;
	return false;
}

bool Menu_layer::OnTextEntered(Event& e)
{
	return false;
}

bool Menu_layer::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	if ((m_StateManager.m_CurrentState == 0) && (event.key.code == sf::Keyboard::Key::Escape))
		Application::Get().Close();

	if ((event.key.code == sf::Keyboard::Key::Left) || (event.key.code == sf::Keyboard::Key::Up))
		m_StateManager.SetState(m_StateManager.GetPreviousState());
	else if ((event.key.code == sf::Keyboard::Key::Right) || (event.key.code == sf::Keyboard::Key::Down))
		m_StateManager.SetState(m_StateManager.GetNextState());
	else if (event.key.code == sf::Keyboard::Key::Enter)
		m_StateManager.ActivateState();
	else if ((event.key.code == sf::Keyboard::Key::BackSpace) || (event.key.code == sf::Keyboard::Key::Escape))
		m_StateManager.SetState(m_StateManager.GetParentState());
	
	return false;
}

bool Menu_layer::OnKeyReleased(Event& e)
{
	return false;
}

bool Menu_layer::MouseWheelScrolled(Event& e)
{
//	sf::Event& event = e.GetEvent();
//
//	m_ZoomLevel *= event.mouseWheelScroll.delta > 0 ? 1.25f : 0.8f;
//	m_ZoomLevel = m_ZoomLevel < g_InitialZoomLevel ? g_InitialZoomLevel : m_ZoomLevel;
//	m_ZoomLevel = m_ZoomLevel > g_MaxZoomLevel ? g_MaxZoomLevel : m_ZoomLevel;
//
//	Renderer::SetZoomLevel(m_ZoomLevel);

	return false;
}

bool Menu_layer::OnMouseButtonPressed(Event& e)
{
//	sf::Event& event = e.GetEvent();
//	if (event.mouseButton.button == sf::Mouse::Right)
//	{
//		TransformComponent& trf = m_Scene->GetCamera();
//		std::cout << trf.location << "\n";
//		std::cout << trf.orientation << "\n";
//		std::cout << m_ZoomLevel << "\n";
//		std::cout << "camera stop\n";
//	}
//

	return false;
}

bool Menu_layer::OnMouseButtonReleased(Event& e)
{
	return false;
}

bool Menu_layer::OnMouseMoved(Event& e)
{
	return false;
}

bool Menu_layer::OnMouseEntered(Event& e)
{
	return false;
}

bool Menu_layer::OnMouseLeft(Event& e)
{
	return false;
}

void Menu_layer::HandleUserInput(Timestep ts)
{
//	TransformComponent& cam_trf = m_Scene->GetCamera();

//	static float cam_velocity = 0.0005f;
	// moves
//	if (Input::IsKeyPressed(sf::Keyboard::Key::W)) { cam_trf.location += ts * cam_velocity * cam_trf.orientation.f3; }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::S)) { cam_trf.location -= ts * cam_velocity * cam_trf.orientation.f3; }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::A)) { cam_trf.location -= ts * cam_velocity * cam_trf.orientation.f1; }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::D)) { cam_trf.location += ts * cam_velocity * cam_trf.orientation.f1; }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::R)) { cam_trf.location += ts * cam_velocity * cam_trf.orientation.f2; }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::F)) { cam_trf.location -= ts * cam_velocity * cam_trf.orientation.f2; }
	// rotations
//	if (Input::IsKeyPressed(sf::Keyboard::Key::Q)) { cam_trf.orientation = Rotation(0.001f * ts, cam_trf.orientation.f3) * cam_trf.orientation; }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::E)) { cam_trf.orientation = Rotation(-0.001f * ts, cam_trf.orientation.f3) * cam_trf.orientation; }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::P)) { ZoomIn(); }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::O)) { ZoomOut(); }
//
//	if (Input::IsKeyPressed(sf::Keyboard::Key::Add)) { cam_velocity *= 1.1f; }
//	if (Input::IsKeyPressed(sf::Keyboard::Key::Subtract)) { cam_velocity /= 1.1f; }

}

void Menu_layer::ZoomIn()
{
	m_ZoomLevel *= 1.05f;
	m_ZoomLevel = m_ZoomLevel > 128.0f ? 128.0f : m_ZoomLevel;
	Renderer::SetZoomLevel(m_ZoomLevel);
}

void Menu_layer::ZoomOut()
{
	m_ZoomLevel /= 1.05f;
	m_ZoomLevel = m_ZoomLevel < g_InitialZoomLevel ? g_InitialZoomLevel : m_ZoomLevel;
	Renderer::SetZoomLevel(m_ZoomLevel);
}

void Menu_layer::InitStates()
{
	m_StateManager.m_MenuLayer = this;
	m_StateManager.m_CurrentState = 0;

	MenuState tmp_state;

	// Main menu (0)
	tmp_state.camera_location.location = Vec3D(0.31f, 0.44f, -13.0f);
	tmp_state.camera_location.orientation = Identity(1.0f);
	tmp_state.camera_location.scale = 200.0f; // use the scale of the trf component to store the zoom level

	tmp_state.parent_state = -1;
	tmp_state.next_state = 1;
	tmp_state.previous_state = 0;
	tmp_state.child_state = 1;
	tmp_state.activation_function = [this]() {
		std::cout << "main menu activation was called\n";
		m_StateManager.SetState(m_StateManager.GetChildState());
	};

	m_StateManager.m_States.push_back(tmp_state);

	// Main menu - level select (1)
	tmp_state.camera_location.location = Vec3D(-0.315f, 0.315f, -13.0f);
	tmp_state.camera_location.orientation = Identity(1.0f);
	tmp_state.camera_location.scale = 200.0f; // use the scale of the trf component to store the zoom level

	tmp_state.parent_state = 0;
	tmp_state.next_state = 2;
	tmp_state.previous_state = 3;
	tmp_state.child_state = 4;
	tmp_state.activation_function = [this]() {
		std::cout << "level select activation was called\n";
		m_StateManager.SetState(m_StateManager.GetChildState());
	};

	m_StateManager.m_States.push_back(tmp_state);

	// Main options (2)
	tmp_state.camera_location.location = Vec3D(-0.065f, 0.315f, -13.0f);
	tmp_state.camera_location.orientation = Identity(1.0f);
	tmp_state.camera_location.scale = 200.0f; // use the scale of the trf component to store the zoom level

	tmp_state.parent_state = 0;
	tmp_state.next_state = 3;
	tmp_state.previous_state = 1;
	tmp_state.child_state = -1;
	tmp_state.activation_function = [this]() {
		std::cout << "options activation was called\n";
		m_StateManager.SetState(m_StateManager.GetChildState());
	};

	m_StateManager.m_States.push_back(tmp_state);

	// Main quit (3)
	tmp_state.camera_location.location = Vec3D(0.185f, 0.315f, -13.0f);
	tmp_state.camera_location.orientation = Identity(1.0f);
	tmp_state.camera_location.scale = 200.0f; // use the scale of the trf component to store the zoom level

	tmp_state.parent_state = 0;
	tmp_state.next_state = 1;
	tmp_state.previous_state = 2;
	tmp_state.child_state = -1;
	tmp_state.activation_function = []() {
		std::cout << "quit activation was called\n";
		Application::Get().Close();
	};

	m_StateManager.m_States.push_back(tmp_state);

	// Moon mission (4):
	tmp_state.camera_location.location = Vec3D(-0.19f, 0.44f, -13.0f);
	tmp_state.camera_location.orientation = Identity(1.0f);
	tmp_state.camera_location.scale = 200.0f; // use the scale of the trf component to store the zoom level

	tmp_state.parent_state = 1;
	tmp_state.next_state = 5;
	tmp_state.previous_state = 6;
	tmp_state.child_state = -1;
	tmp_state.activation_function = [this]() {
		std::cout << "moon mission activation was called\n";
		DeActivate();
		Application& app = Application::Get();
		((GameApplication*)(&app))->ActitivateLayer(GameLayers::INGAME_LAYER2);
	};

	m_StateManager.m_States.push_back(tmp_state);

	// Earth mission (5):
	tmp_state.camera_location.location = Vec3D(-0.44f, 0.44f, -13.0f);
	tmp_state.camera_location.orientation = Identity(1.0f);
	tmp_state.camera_location.scale = 200.0f; // use the scale of the trf component to store the zoom level

	tmp_state.parent_state = 1;
	tmp_state.next_state = 6;
	tmp_state.previous_state = 4;
	tmp_state.child_state = -1;
	tmp_state.activation_function = [this]() {
		std::cout << "earth mission activation was called\n";
		DeActivate();
		Application& app = Application::Get();
		((GameApplication*)(&app))->ActitivateLayer(GameLayers::INGAME_LAYER);
	};

	m_StateManager.m_States.push_back(tmp_state);

	// Mars mission (6):
	tmp_state.camera_location.location = Vec3D(0.06f, 0.44f, -13.0f);
	tmp_state.camera_location.orientation = Identity(1.0f);
	tmp_state.camera_location.scale = 200.0f; // use the scale of the trf component to store the zoom level

	tmp_state.parent_state = 1;
	tmp_state.next_state = 4;
	tmp_state.previous_state = 5;
	tmp_state.child_state = -1;
	tmp_state.activation_function = [this]() {
		std::cout << "earth mission activation was called\n";
		DeActivate();
		Application& app = Application::Get();
		((GameApplication*)(&app))->ActitivateLayer(GameLayers::MARS_MISSION_LAYER);
	};

	m_StateManager.m_States.push_back(tmp_state);


}








