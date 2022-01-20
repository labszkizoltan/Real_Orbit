
#include "InGame_layer.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>

#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/Skybox.h>
#include <core/rendering/drawables/NormalMesh.h>

#include <core/scene/SceneSerializer.h>
#include <core/scene/CoreComponents.h>


#include <utils/Vector_3D.h>
#include <glad/glad.h>

//#include "scene_descriptions.h"

#include <queue> // the priority_queue uses it in the targeting function

#define BIND_EVENT_FN(x) std::bind(&InGame_layer::x, this, std::placeholders::_1)

OGLBufferData ParseVertexFile(const std::string& filename)
{
	OGLBufferData result;
	std::ifstream myfile(filename.c_str());
	if (!myfile.is_open())
	{
		LOG_CORE_WARN("InGame_layer::ParseVertexFile() was unable to open file: {0}", filename);
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
		LOG_CORE_WARN("InGame_layer::ParseVertexFile(): vertex or index data missing from file: ", filename);

	return result;
}


InGame_layer::InGame_layer()
	: Layer("InGame_layer")
{
	LOG_INFO("InGame_layer constructed");
}

void InGame_layer::OnAttach()
{
	LOG_INFO("InGame_layer attached");

	FrameBufferSpecification fbSpec;
	fbSpec.Width = Application::Get().GetWindow().GetWidth();
	fbSpec.Height = Application::Get().GetWindow().GetHeight();
	fbSpec.Samples = 1;

	m_Scene = std::make_shared<Scene>();

	SceneSerializer serializer(m_Scene);
	//	serializer.DeSerialize_file("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/scenes/test_scene_3.yaml");
	serializer.DeSerialize_file("assets/scenes/test_scene_3.yaml");

	m_SceneRenderer.SetScene(m_Scene);
	m_SceneUpdater.SetScene(m_Scene);

	//	m_FbDisplay.SetTexture(Renderer::GetColorAttachment());
	m_FbDisplay.SetTexture(Renderer::GetBlurredAttachment());

	m_ImgProcessor = std::make_unique<ImageProcessor>();
	m_ImgProcessor->SetMipMapLevel(4);




	// load background music
	if (!m_Music.openFromFile("assets/audio/Adrift_by_Hayden_Folker.ogg"))
		//	if (!m_Music.openFromFile("assets/audio/Puritania.wav"))
		LOG_ERROR("Audio not found: assets/audio/Adrift_by_Hayden_Folker.ogg");
	m_Music.play();

	// Load a sound to play
	if (!m_ShotSoundBuffer.loadFromFile("assets/audio/SciFi_weapon_MultiShot_1.wav"))
		LOG_ERROR("Audio not found: assets/audio/SciFi_weapon_MultiShot_1.wav");
	m_ShotSound.setBuffer(m_ShotSoundBuffer);
	m_ShotSound.setVolume(60.0f);



}

void InGame_layer::OnDetach()
{
	LOG_INFO("InGame_layer detached");
}

void InGame_layer::OnUpdate(Timestep ts)
{
	// if music finished, start again
	if (m_Music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		m_Music.openFromFile("assets/audio/Adrift_by_Hayden_Folker.ogg");
		//		m_Music.openFromFile("assets/audio/Puritania.wav");
		m_Music.play();
	}

	HandleUserInput(ts);
	{
		static int randomLaunchCounter = 0;
		static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];
		static int blueIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["BlueSphere"];
		static int yellowIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["YellowSphere"];
		//if (randomLaunchCounter % 1000 < 50)
		//if (randomLaunchCounter % 8 == 0)
		//{
		//	RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
		//	RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
		//}
		/*
		if (randomLaunchCounter % 1000 < 50)
		{
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
			RandomRocketLaunch(blueIdx, Vec3D(-20, 20, -300));
		}
		if ((randomLaunchCounter+500) % 1000 < 50)
		{
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
			RandomRocketLaunch(yellowIdx, Vec3D(20, -20, 300));
		}
		*/

		int asteroid_count = 128;
		float spawn_frequency = 10000.0f;
		static float asteroid_spawn_timer = spawn_frequency; // in milli seconds
		asteroid_spawn_timer -= m_SimulationSpeed * ts;
		//		if (randomLaunchCounter % (int)spawn_frequency == 0)
		if (asteroid_spawn_timer < 0.0f)
		{
			asteroid_spawn_timer = spawn_frequency;
			//			spawn_frequency *= 0.9f;
			//			Vec3D center = Vec3D(rand() % 50 - 25, rand() % 50 - 25, 0);
			Vec3D center = Vec3D(50, 0, 0);
			Vec3D velocity = -0.01 * center / center.length();
			for (int i = 0; i < asteroid_count; i++)
				SpawnAsteroid(500.0f * center / center.length(), velocity, 80.0f);
		}

		randomLaunchCounter++;
	}

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

void InGame_layer::OnEvent(Event& event)
{
	//	LOG_INFO("InGame_layer event received");

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

void InGame_layer::RandomRocketLaunch(int meshIdx, Vec3D origin)
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

entt::entity InGame_layer::GetTarget()
{
	static int counter = 0;
	int queue_size = 15;
	std::priority_queue<targeting_data<entt::entity>> targeting_queue;

	Vec3D pos = m_Scene->GetCamera().location;
	Vec3D dir = m_Scene->GetCamera().orientation.f3;

	entt::entity result = entt::null;

	float maxScalarProd = -1.0f;

	// exclude TargetComponent, so missiles wont target other missiles
//	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent, HitPointComponent>(entt::exclude<TargetComponent>);
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent, AsteroidComponent>();
	if (view.begin() == view.end())
		return entt::null;

	for (auto entity : view)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(entity);
		Vec3D dx = entity_trf.location - pos;

		float scalarProd = dx * dir / dx.length();
		//		if (scalarProd > maxScalarProd)
		//		{
		//			maxScalarProd = scalarProd;
		//			result = entity;
		//		}

		targeting_data<entt::entity> temp_td;
		temp_td.dot_product = -scalarProd;
		temp_td.user_data = entity;
		if (queue_size > 0)
		{
			targeting_queue.push(temp_td);
			queue_size--;
		}
		else
		{
			if (temp_td < targeting_queue.top())
				//			if (targeting_queue.top() < temp_td)
			{
				targeting_queue.pop();
				targeting_queue.push(temp_td);
			}
		}

	}

	for (int i = 0; i < std::min(counter, (int)targeting_queue.size() - 1); i++)
		targeting_queue.pop();

	counter--;
	counter = counter < 0 ? 15 : counter;

	if (targeting_queue.size() == 0)
		return entt::null;
	else
		return targeting_queue.top().user_data;

	//	return result;
}

void InGame_layer::SpawnAsteroid(Vec3D center, Vec3D velocity, float spread)
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

void InGame_layer::EmitMesh(int meshIdx, TransformComponent transform)
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

void InGame_layer::LaunchMissile(int meshIdx, TransformComponent transform, entt::entity target)
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

void InGame_layer::RemoveMesh(int meshIdx)
{
	if (m_Scene->GetMeshLibrary().m_MeshTransforms[meshIdx].size() > 0)
		m_Scene->GetMeshLibrary().m_MeshTransforms[meshIdx].pop_back();
}


/***********************************
***** Private member functions *****
************************************/

bool InGame_layer::OnWindowResize(Event& e)
{
	return false;
}

bool InGame_layer::OnLoosingFocus(Event& e)
{
	m_InFocus = false;
	return false;
}

bool InGame_layer::OnGainingFocus(Event& e)
{
	m_InFocus = true;
	return false;
}

bool InGame_layer::OnTextEntered(Event& e)
{
	return false;
}

bool InGame_layer::OnKeyPressed(Event& e)
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

bool InGame_layer::OnKeyReleased(Event& e)
{
	return false;
}

bool InGame_layer::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();

	m_ZoomLevel *= event.mouseWheelScroll.delta > 0 ? 1.25f : 0.8f;
	m_ZoomLevel = m_ZoomLevel < g_InitialZoomLevel ? g_InitialZoomLevel : m_ZoomLevel;
	m_ZoomLevel = m_ZoomLevel > g_MaxZoomLevel ? g_MaxZoomLevel : m_ZoomLevel;

	Renderer::SetZoomLevel(m_ZoomLevel);

	return false;
}

bool InGame_layer::OnMouseButtonPressed(Event& e)
{
	return false;
}

bool InGame_layer::OnMouseButtonReleased(Event& e)
{
	return false;
}

bool InGame_layer::OnMouseMoved(Event& e)
{
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

	return false;
}

bool InGame_layer::OnMouseEntered(Event& e)
{
	return false;
}

bool InGame_layer::OnMouseLeft(Event& e)
{
	return false;
}

void InGame_layer::HandleUserInput(Timestep ts)
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

	static int orangeIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["OrangeSphere"];
	static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];
	static int blueIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["BlueSphere"];
	static int yellowIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["YellowSphere"];
	static int explosionIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Explosion"];
	static int skip = 0;
	if (Input::IsMouseButtonPressed(sf::Mouse::Left))
	{
		LaunchMissile(blueIdx, cam_trf, GetTarget());
		LaunchMissile(blueIdx, cam_trf, GetTarget());
		LaunchMissile(blueIdx, cam_trf, GetTarget());
		//LaunchMissile(explosionIdx, cam_trf, GetTarget());
		if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) { m_ShotSound.play(); }

	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Right) && skip % 2 == 0)
	{
		//		EmitMesh(orangeIdx, cam_trf);
		if (skip % 4 == 0)
			EmitMesh(bulletIdx, cam_trf);
		if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) {
			m_ShotSound.play();
		}
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Middle) && skip % 2 == 1)
	{
		LaunchMissile(yellowIdx, cam_trf, GetTarget());
		if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) { m_ShotSound.play(); }
	}
	if (!Input::IsMouseButtonPressed(sf::Mouse::Left) && !Input::IsMouseButtonPressed(sf::Mouse::Right) && !Input::IsMouseButtonPressed(sf::Mouse::Middle))
		m_ShotSound.stop();

	skip++;


	if (Input::IsKeyPressed(sf::Keyboard::Key::Space))
	{
		TransformComponent& light_trf = m_Scene->GetLight();
		light_trf.location = cam_trf.location;
	}

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

void InGame_layer::ZoomIn()
{
	m_ZoomLevel *= 1.05f;
	m_ZoomLevel = m_ZoomLevel > 128.0f ? 128.0f : m_ZoomLevel;
	Renderer::SetZoomLevel(m_ZoomLevel);
}

void InGame_layer::ZoomOut()
{
	m_ZoomLevel /= 1.05f;
	m_ZoomLevel = m_ZoomLevel < g_InitialZoomLevel ? g_InitialZoomLevel : m_ZoomLevel;
	Renderer::SetZoomLevel(m_ZoomLevel);
}
