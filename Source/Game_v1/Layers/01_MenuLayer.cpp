
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

	FrameBufferSpecification fbSpec;
	fbSpec.Width = Application::Get().GetWindow().GetWidth();
	fbSpec.Height = Application::Get().GetWindow().GetHeight();
	fbSpec.Samples = 1;

	m_Scene = std::make_shared<Scene>();

	SceneSerializer serializer(m_Scene);
	//	serializer.DeSerialize_file("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/scenes/test_scene_3.yaml");
	serializer.DeSerialize_file("assets/scenes/01_MenuLayer.yaml");

	m_SceneRenderer.SetScene(m_Scene);
	m_SceneUpdater.SetScene(m_Scene);

	//	m_FbDisplay.SetTexture(Renderer::GetColorAttachment());
	m_FbDisplay.SetTexture(Renderer::GetBlurredAttachment());

	m_ImgProcessor = std::make_unique<ImageProcessor>();
	m_ImgProcessor->SetMipMapLevel(4);

	// load background music
	/*
	if (!m_Music.openFromFile("assets/audio/Kevin_MacLeod-Broken_Reality.wav"))
		LOG_ERROR("Audio not found: assets/audio/Kevin_MacLeod-Broken_Reality.wav");
	m_Music.play();
	*/

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
		//		m_Music.openFromFile("assets/audio/Puritania.wav");
		m_Music.play();
	}

	HandleUserInput(ts);

	// if the update-render order is swapped, something is un-initialized and the program fails at alpha mesh rendering
	m_SceneRenderer.RenderScene();
	m_SceneUpdater.UpdateScene(m_SimulationSpeed * ts);

	//	m_ImgProcessor->Blur(g_RendererBlurDepthSlot, Renderer::s_BlurBuffer); // this is not working, as expected
	m_ImgProcessor->Blur(g_RendererBrightColAttchSlot, Renderer::s_BlurBuffer);

	//	m_FbDisplay.Draw();
	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, g_RendererBlurredSlot);

	//	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, 0);
	//	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, g_RendererBrightColAttchSlot);
	//	m_FbDisplay.DrawCombined(Renderer::GetColorAttachment(), Renderer::GetBrightColorAttachment());

	m_ElapsedTime += m_SimulationSpeed * ts;
}

void Menu_layer::OnEvent(Event& event)
{
	//	LOG_INFO("Menu_layer event received");

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
	m_Music.play();
	m_IsActive = true;
}

void Menu_layer::DeActivate()
{
	Application& app = Application::Get();
	((GameApplication*)(&app))->ActitivateLayer(GameLayers::INGAME_LAYER);
	m_Music.pause();
	m_IsActive = false;
}

/*
void Menu_layer::RandomRocketLaunch(int meshIdx, Vec3D origin)
{
	// get a random target
//	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent>(entt::exclude<TimerComponent>);
	auto poolOfTargets = m_Scene->m_Registry.view<TransformComponent>(entt::exclude<TimerComponent>);
	int size = poolOfTargets.size_hint();
	int idx = rand() % size;

	entt::entity target = entt::null;
	while (target == entt::null)
	{
		for (auto entity : poolOfTargets)
		{
			if (idx == 0)
			{
				target = entity;
				break;
			}
			idx--;
		}
	}

	TransformComponent trf;
	trf.location = origin;
	LaunchMissile(meshIdx, trf, target);
}

entt::entity Menu_layer::GetTarget()
{
	Vec3D pos = m_Scene->GetCamera().location;
	Vec3D dir = m_Scene->GetCamera().orientation.f3;

	entt::entity result = entt::null;

	float maxScalarProd = -1.0f;

	// exclude TargetComponent, so missiles wont target other missiles
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent>(entt::exclude<TargetComponent>);
	for (auto entity : view)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(entity);
		Vec3D dx = entity_trf.location - pos;

		float scalarProd = dx * dir / dx.length();
		if (scalarProd > maxScalarProd)
		{
			maxScalarProd = scalarProd;
			result = entity;
		}
	}

	return result;
}

void Menu_layer::SpawnAsteroid(Vec3D center, Vec3D velocity, float spread)
{
	static int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DeformedSphere"];

	TransformComponent transform;
	transform.location = center + Vec3D(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500) * spread / 1000.0f;
	transform.orientation = Rotation((float)(rand() % 31415) / 10000.0f, Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	//	transform.orientation.f1 *= 1.0f + (float)(rand() % 1024) / 1024.0f;
	//	transform.orientation.f2 *= 1.0f + (float)(rand() % 1024) / 1024.0f;
	//	transform.orientation.f3 *= 1.0f + (float)(rand() % 1024) / 1024.0f;
	transform.scale = (float)(rand() % 10 + 2) / 20.0f;

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
	dynProps.velocity = velocity;
	dynProps.angular_velocity = 0.0001 * Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(asteroidIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	//	newEntity.AddComponent<HitPointComponent>(1.0f + (float)(rand() % 10));
	newEntity.AddComponent<HitPointComponent>(1.0f);
	newEntity.AddComponent<AsteroidComponent>(AsteroidComponent());


}

void Menu_layer::EmitMesh(int meshIdx, TransformComponent transform)
{
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 0.001f;
	dynProps.velocity = 0.05f * transform.orientation.f3;
	dynProps.angular_velocity = Vec3D();
	transform.scale = 0.1f; // 0.02f;
	transform.location += 0.1 * (transform.orientation.f3 - transform.orientation.f2);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(meshIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(10000.0f)); // provide ttl in mili seconds
	newEntity.AddComponent<HitPointComponent>(1.0f);
}

void Menu_layer::LaunchMissile(int meshIdx, TransformComponent transform, entt::entity target)
{
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 0.001f;
	//	dynProps.velocity = 0.01f * (transform.orientation.f1 + transform.orientation.f3);
	//	dynProps.velocity = 0.05f * transform.orientation.f3;
	dynProps.velocity = 0.00000001f * transform.orientation.f3;
	dynProps.angular_velocity = Vec3D();
	transform.scale = 0.01f;
	transform.location += 0.1 * (transform.orientation.f3 + transform.orientation.f1);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(meshIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(20000.0f)); // provide ttl in mili seconds
	newEntity.AddComponent<TargetComponent>(target);
}

void Menu_layer::RemoveMesh(int meshIdx)
{
	if (m_Scene->GetMeshLibrary().m_MeshTransforms[meshIdx].size() > 0)
		m_Scene->GetMeshLibrary().m_MeshTransforms[meshIdx].pop_back();
}
*/


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
	if (event.key.code == sf::Keyboard::Key::Escape)
	{
		Application::Get().Close();
	}
	else if (event.key.code == sf::Keyboard::Key::Space)
		m_SimulationSpeed = 0.0f;
	else if (event.key.code == sf::Keyboard::Key::Num1)
		m_SimulationSpeed = 0.2f;
	else if (event.key.code == sf::Keyboard::Key::Num2)
		m_SimulationSpeed = 1.0f;
	else if (event.key.code == sf::Keyboard::Key::Num3)
		m_SimulationSpeed = 5.0f;
	else if (event.key.code == sf::Keyboard::Key::C)
		m_CameraContinuousRotation = !m_CameraContinuousRotation;

	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);

	return false;
}

bool Menu_layer::OnKeyReleased(Event& e)
{
	return false;
}

bool Menu_layer::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();

	m_ZoomLevel *= event.mouseWheelScroll.delta > 0 ? 1.25f : 0.8f;
	m_ZoomLevel = m_ZoomLevel < g_InitialZoomLevel ? g_InitialZoomLevel : m_ZoomLevel;
	m_ZoomLevel = m_ZoomLevel > g_MaxZoomLevel ? g_MaxZoomLevel : m_ZoomLevel;

	Renderer::SetZoomLevel(m_ZoomLevel);

	return false;
}

bool Menu_layer::OnMouseButtonPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		DeActivate();

	}


	return false;
}

bool Menu_layer::OnMouseButtonReleased(Event& e)
{
	return false;
}

bool Menu_layer::OnMouseMoved(Event& e)
{
	/*
	static int center_x = Application::Get().GetWindow().GetWidth() / 2;
	static int center_y = Application::Get().GetWindow().GetHeight() / 2;

	sf::Event& event = e.GetEvent();
	std::cout << "new mouse x: " << event.mouseMove.x << "\n";
	std::cout << "new mouse y: " << event.mouseMove.y << "\n";
	if (m_InFocus)
	{
		TransformComponent& cam_trf = m_Scene->GetCamera();
		static const float r_min_square = 50.0f * 50.0f;

		int dx = event.mouseMove.x - center_x;
		int dy = event.mouseMove.y - center_y;
		//		float moveLengthSqr = dx * dx + dy * dy;
		float moveLength = sqrt(dx * dx + dy * dy);
		Vec3D rotationAxis = dy * cam_trf.orientation.f1 + dx * cam_trf.orientation.f2;
		//		Mat_3D rotationMatrix = Rotation(0.0001f * moveLengthSqr / m_ZoomLevel, rotationAxis); // angle is divided by zoom level to slow down turning when really zoomed into something
		Mat_3D rotationMatrix = Rotation(0.005f * moveLength / m_ZoomLevel, rotationAxis);

		cam_trf.orientation = rotationMatrix * cam_trf.orientation;

		// re-center mouse
		static int center_x = Application::Get().GetWindow().GetWidth() / 2;
		static int center_y = Application::Get().GetWindow().GetHeight() / 2;
		sf::Mouse::setPosition(sf::Vector2i(center_x, center_y), Application::Get().GetWindow().GetNativeWindow());
	}

	*/
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
	if (Input::IsKeyPressed(sf::Keyboard::Key::P)) { ZoomIn(); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::O)) { ZoomOut(); }

	if (Input::IsKeyPressed(sf::Keyboard::Key::Add)) { cam_velocity *= 1.1f; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Subtract)) { cam_velocity /= 1.1f; }


	// move with as the mouse changes position
	if (m_InFocus)
	{
		if (!m_CameraContinuousRotation)
		{
			sf::Vector2i mousePos = Input::GetMousePosition();
			static int center_x = Application::Get().GetWindow().GetWidth() / 2;
			static int center_y = Application::Get().GetWindow().GetHeight() / 2;

			int dx = mousePos.x - center_x;
			int dy = mousePos.y - center_y;

			float radiusFromCenter = sqrt(dx * dx + dy * dy);
			float radiusFromCenterSqr = dx * dx + dy * dy;
			Vec3D rotationAxis = dy * cam_trf.orientation.f1 + dx * cam_trf.orientation.f2;
			Mat_3D rotationMatrix = Rotation(ts * (0.00025f * radiusFromCenter + 0.0000001f * radiusFromCenterSqr) / m_ZoomLevel, rotationAxis); // angle could be divided by zoom level to slow down turning when really zoomed into something
	//		Mat_3D rotationMatrix = Rotation(0.0001f * ts * radiusFromCenter / m_ZoomLevel, rotationAxis); // angle could be divided by zoom level to slow down turning when really zoomed into something

			cam_trf.orientation = rotationMatrix * cam_trf.orientation;
			sf::Mouse::setPosition(sf::Vector2i(center_x, center_y), Application::Get().GetWindow().GetNativeWindow());
		}
		else
		{
			float r_min_square = 50.0f * 50.0f;
			sf::Vector2i mousePos = Input::GetMousePosition();
			mousePos.x -= Application::Get().GetWindow().GetWidth() / 2;
			mousePos.y -= Application::Get().GetWindow().GetHeight() / 2;
			float radiusFromCenter_square = (mousePos.x * mousePos.x + mousePos.y * mousePos.y);
			Vec3D rotationAxis = mousePos.y * cam_trf.orientation.f1 + mousePos.x * cam_trf.orientation.f2;
			Mat_3D rotationMatrix = Rotation(0.00000001f * ts * std::max(0.0f, radiusFromCenter_square - r_min_square) / m_ZoomLevel, rotationAxis); // angle could be divided by zoom level to slow down turning when really zoomed into something

			cam_trf.orientation = rotationMatrix * cam_trf.orientation;
		}
	}

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
