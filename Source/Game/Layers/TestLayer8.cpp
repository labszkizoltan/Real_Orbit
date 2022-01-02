
#include "TestLayer8.h"
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
		LOG_ERROR("Audio not found: assets/audio/Adrift_by_Hayden_Folker.ogg");
	m_Music.play();

	// Load a sound to play
	if (!m_ShotSoundBuffer.loadFromFile("assets/audio/SciFi_weapon_MultiShot_1.wav"))
		LOG_ERROR("Audio not found: assets/audio/SciFi_weapon_MultiShot_1.wav");
	m_ShotSound.setBuffer(m_ShotSoundBuffer);
	m_ShotSound.setVolume(60.0f);



}

void TestLayer8::OnDetach()
{
	LOG_INFO("TestLayer8 detached");
}

void TestLayer8::OnUpdate(Timestep ts)
{
	// if music finished, start again
	if (m_Music.getStatus() == sf::SoundSource::Status::Stopped)
	{
		m_Music.openFromFile("assets/audio/Adrift_by_Hayden_Folker.ogg");
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

		int asteroid_count = 30;
		if (randomLaunchCounter % (asteroid_count * 80) == 0)
		{
			for(int i=0; i<asteroid_count; i++)
				SpawnAsteroid();
		}

		randomLaunchCounter++;
	}

	m_SceneRenderer.RenderScene();
	m_SceneUpdater.UpdateScene(m_SimulationSpeed*ts);

//	m_ImgProcessor->Blur(g_RendererBlurDepthSlot, Renderer::s_BlurBuffer); // this is not working, as expected
	m_ImgProcessor->Blur(g_RendererBrightColAttchSlot, Renderer::s_BlurBuffer);

//	m_FbDisplay.Draw();
	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, g_RendererBlurredSlot);

//	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, 0);
//	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, g_RendererBrightColAttchSlot);
//	m_FbDisplay.DrawCombined(Renderer::GetColorAttachment(), Renderer::GetBrightColorAttachment());

	m_ElapsedTime += m_SimulationSpeed*ts;
}

void TestLayer8::OnEvent(Event& event)
{
//	LOG_INFO("TestLayer8 event received");

	event.Dispatch<sf::Event::EventType::Resized>				(BIND_EVENT_FN(OnWindowResize)); // this should be removed when this is resolved through the renderer
	event.Dispatch<sf::Event::EventType::LostFocus>				(BIND_EVENT_FN(OnLoosingFocus));
	event.Dispatch<sf::Event::EventType::GainedFocus>			(BIND_EVENT_FN(OnGainingFocus));
	event.Dispatch<sf::Event::EventType::KeyPressed>			(BIND_EVENT_FN(OnKeyPressed));
	event.Dispatch<sf::Event::EventType::KeyReleased>			(BIND_EVENT_FN(OnKeyReleased));
	event.Dispatch<sf::Event::EventType::MouseWheelScrolled>	(BIND_EVENT_FN(MouseWheelScrolled));
	event.Dispatch<sf::Event::EventType::MouseButtonPressed>	(BIND_EVENT_FN(OnMouseButtonPressed));
	event.Dispatch<sf::Event::EventType::MouseButtonReleased>	(BIND_EVENT_FN(OnMouseButtonReleased));
//	event.Dispatch<sf::Event::EventType::MouseMoved>			(BIND_EVENT_FN(OnMouseMoved));
}

void TestLayer8::RandomRocketLaunch(int meshIdx, Vec3D origin)
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

entt::entity TestLayer8::GetTarget()
{
	Vec3D pos = m_Scene->GetCamera().location;
	Vec3D dir = m_Scene->GetCamera().orientation.f3;

	entt::entity result = entt::null;

	float maxScalarProd = -1.0f;

	// exclude TargetComponent, so missiles wont target other missiles
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent, HitPointComponent>(entt::exclude<TargetComponent>);
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

void TestLayer8::SpawnAsteroid()
{
	static int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DeformedSphere"];

	TransformComponent transform;
	transform.location = Vec3D(rand() % 50 + 500, rand() % 50, rand() % 50);
	transform.orientation = Rotation((float)(rand() % 31415)/10000.0f, Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	transform.scale = (float)(rand() % 10 + 2)/20.0f;

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
	dynProps.velocity = -0.01 * transform.location / transform.location.length();
	dynProps.angular_velocity = 0.0001 * Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(asteroidIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<HitPointComponent>(1.0f + (float)(rand()%10));
}

void TestLayer8::EmitMesh(int meshIdx, TransformComponent transform)
{
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
	dynProps.velocity = 0.05f * transform.orientation.f3;
	dynProps.angular_velocity = Vec3D();
	transform.scale = 0.02f;
	transform.location += 0.1*(transform.orientation.f3 - transform.orientation.f2);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(meshIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(10000.0f)); // provide ttl in mili seconds
	newEntity.AddComponent<HitPointComponent>(1.0f);
}

void TestLayer8::LaunchMissile(int meshIdx, TransformComponent transform, entt::entity target)
{
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
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

void TestLayer8::RemoveMesh(int meshIdx)
{
	if(m_Scene->GetMeshLibrary().m_MeshTransforms[meshIdx].size()>0)
		m_Scene->GetMeshLibrary().m_MeshTransforms[meshIdx].pop_back();
}


/***********************************
***** Private member functions *****
************************************/

bool TestLayer8::OnWindowResize(Event& e)
{
	return false;
}

bool TestLayer8::OnLoosingFocus(Event& e)
{
	m_InFocus = false;
	return false;
}

bool TestLayer8::OnGainingFocus(Event& e)
{
	m_InFocus = true;
	return false;
}

bool TestLayer8::OnTextEntered(Event& e)
{
	return false;
}

bool TestLayer8::OnKeyPressed(Event& e)
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
	else if (event.key.code == sf::Keyboard::Key::N)
	{
		LOG_WARN("Display Texture Slot was changed to: {0}", m_current_slot % 32);
		m_FbDisplay.SetDisplayedSlot(m_current_slot % 32);
		m_current_slot++;
	}
	else if (event.key.code == sf::Keyboard::Key::M)
	{
		LOG_WARN("MipMap level was changed to: {0}", m_mipmap_level % 12);
		m_ImgProcessor->SetMipMapLevel(m_mipmap_level%12);
		m_mipmap_level++;
	}
	else if (event.key.code == sf::Keyboard::Key::C)
		m_CameraContinuousRotation = !m_CameraContinuousRotation;

	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);

	return false;
}

bool TestLayer8::OnKeyReleased(Event& e)
{
	return false;
}

bool TestLayer8::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();

	m_ZoomLevel *= event.mouseWheelScroll.delta > 0 ? 1.25f : 0.8f;
	m_ZoomLevel = m_ZoomLevel < g_InitialZoomLevel ? g_InitialZoomLevel : m_ZoomLevel;
	m_ZoomLevel = m_ZoomLevel > 128.0f ? 128.0f : m_ZoomLevel;

	Renderer::SetZoomLevel(m_ZoomLevel);

	return false;
}

bool TestLayer8::OnMouseButtonPressed(Event& e)
{
	return false;
}

bool TestLayer8::OnMouseButtonReleased(Event& e)
{
	return false;
}

bool TestLayer8::OnMouseMoved(Event& e)
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

bool TestLayer8::OnMouseEntered(Event& e)
{
	return false;
}

bool TestLayer8::OnMouseLeft(Event& e)
{
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
	if(Input::IsMouseButtonPressed(sf::Mouse::Left))
	{
//		EmitMesh(orangeIdx, cam_trf);
		if(skip%4==0)
			EmitMesh(bulletIdx, cam_trf);
		if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) {
			m_ShotSound.play(); 
		}
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Right) && skip%2 == 0) 
	{
		LaunchMissile(blueIdx, cam_trf, GetTarget());
		//LaunchMissile(explosionIdx, cam_trf, GetTarget());
		if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) { m_ShotSound.play(); }
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Middle) && skip % 2 == 1)
	{
		LaunchMissile(yellowIdx, cam_trf, GetTarget());
		if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) { m_ShotSound.play(); }
	}
	if(!Input::IsMouseButtonPressed(sf::Mouse::Left) && !Input::IsMouseButtonPressed(sf::Mouse::Right) && !Input::IsMouseButtonPressed(sf::Mouse::Middle))
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

void TestLayer8::ZoomIn()
{
	m_ZoomLevel *= 1.05f;
	m_ZoomLevel = m_ZoomLevel > 128.0f ? 128.0f : m_ZoomLevel;
	Renderer::SetZoomLevel(m_ZoomLevel);
}

void TestLayer8::ZoomOut()
{
	m_ZoomLevel /= 1.05f;
	m_ZoomLevel = m_ZoomLevel < g_InitialZoomLevel ? g_InitialZoomLevel : m_ZoomLevel;
	Renderer::SetZoomLevel(m_ZoomLevel);
}
