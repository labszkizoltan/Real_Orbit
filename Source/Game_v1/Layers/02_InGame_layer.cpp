
#include "02_InGame_layer.h"
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
	serializer.DeSerialize_file("assets/scenes/02_InGameLayer.yaml");

	m_SceneRenderer.SetScene(m_Scene);

	Box3D tmp_box; tmp_box.radius = Vec3D(10000, 10000, 10000);
	m_AsteroidOctTree = std::make_unique<DualOctTree>(tmp_box);

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

	// Load a sound to play
	if (!m_ExplosionSoundBuffer.loadFromFile("assets/audio/Explosion_big.wav"))
		LOG_ERROR("SceneUpdater() - Audio not found: assets/audio/Explosion_big.wav");
	m_ExplosionSound.setBuffer(m_ExplosionSoundBuffer);
	m_ExplosionSound.setVolume(60.0f);



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
		m_Music.play();
	}

	HandleUserInput(ts);
	{
		static int randomLaunchCounter = 0;
		static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];
		static int blueIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["BlueSphere"];
		static int yellowIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["YellowSphere"];

		int asteroid_count = 128;
		float spawn_frequency = 10000.0f;
		static float asteroid_spawn_timer = spawn_frequency; // in milli seconds
		asteroid_spawn_timer -= m_SimulationSpeed * ts;
		if (asteroid_spawn_timer < 0.0f)
		{
			asteroid_spawn_timer = spawn_frequency;
			Vec3D center = Vec3D(50, 0, 0);
			Vec3D velocity = -0.01 * center / center.length();
			for (int i = 0; i < asteroid_count; i++)
				SpawnAsteroid(500.0f * center / center.length(), velocity, 80.0f);
		}

		randomLaunchCounter++;
	}

	{
		/*
		TransformComponent& camLoc = m_Scene->GetCamera();
		Box3D camVicinity;
		camVicinity.center = camLoc.location;
		camVicinity.radius = Vec3D(30, 30, 30);

		std::vector<entt::entity> close_entities = m_AsteroidOctTree->GetActiveTree()->QueryRange(camVicinity);
		for (entt::entity entity : close_entities)
		{
			if (m_Scene->m_Registry.valid(entity))
			{
				TransformComponent& trf = m_Scene->m_Registry.get<TransformComponent>(entity);
				DynamicPropertiesComponent& dyn = m_Scene->m_Registry.get<DynamicPropertiesComponent>(entity);
				SpawnExplosion(trf, dyn);
				m_ExplosionSound.play();
				m_Scene->m_Registry.destroy(entity);
			}
		}
		*/
	}

	// if the update-render order is swapped, something is un-initialized and the program fails at alpha mesh rendering
	m_SceneRenderer.RenderScene();
	// m_SceneUpdater.UpdateScene(m_SimulationSpeed * ts);
	UpdateScene(m_SimulationSpeed * ts);
	m_AsteroidOctTree->Update(m_Scene.get());

	//	m_ImgProcessor->Blur(g_RendererBlurDepthSlot, Renderer::s_BlurBuffer); // this is not working, as expected
	m_ImgProcessor->Blur(g_RendererBrightColAttchSlot, Renderer::s_BlurBuffer);

	//	m_FbDisplay.Draw();
	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, g_RendererBlurredSlot);

	m_ElapsedTime += m_SimulationSpeed * ts;
}

void InGame_layer::OnEvent(Event& event)
{
	event.Dispatch<sf::Event::EventType::Resized>				(BIND_EVENT_FN(OnWindowResize));
	event.Dispatch<sf::Event::EventType::LostFocus>				(BIND_EVENT_FN(OnLoosingFocus));
	event.Dispatch<sf::Event::EventType::GainedFocus>			(BIND_EVENT_FN(OnGainingFocus));
	event.Dispatch<sf::Event::EventType::KeyPressed>			(BIND_EVENT_FN(OnKeyPressed));
	event.Dispatch<sf::Event::EventType::KeyReleased>			(BIND_EVENT_FN(OnKeyReleased));
	event.Dispatch<sf::Event::EventType::MouseWheelScrolled>	(BIND_EVENT_FN(MouseWheelScrolled));
	event.Dispatch<sf::Event::EventType::MouseButtonPressed>	(BIND_EVENT_FN(OnMouseButtonPressed));
	event.Dispatch<sf::Event::EventType::MouseButtonReleased>	(BIND_EVENT_FN(OnMouseButtonReleased));
	//	event.Dispatch<sf::Event::EventType::MouseMoved>			(BIND_EVENT_FN(OnMouseMoved));
}

void InGame_layer::Activate()
{
	m_IsActive = true;
	m_Music.play();
	Renderer::SetZoomLevel(m_ZoomLevel);
}

void InGame_layer::DeActivate()
{
	Application& app = Application::Get();
	((GameApplication*)(&app))->ActitivateLayer(GameLayers::MENU_LAYER);
	m_Music.pause();
	m_IsActive = false;
}


entt::entity InGame_layer::GetTarget()
{
	return GetTarget(m_Scene->GetCamera().location, m_Scene->GetCamera().orientation.f3);
}

entt::entity InGame_layer::GetTarget(const Vec3D& acquisitionLocation, const Vec3D& acquisitionDirection)
{
	static int counter = 0;
	int queue_size = 15;
	std::priority_queue<targeting_data<entt::entity>> targeting_queue;

//	Vec3D pos = m_Scene->GetCamera().location;
//	Vec3D dir = m_Scene->GetCamera().orientation.f3;
	Vec3D pos = acquisitionLocation;
	Vec3D dir = acquisitionDirection;

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


entt::entity InGame_layer::GetClosestTarget(const Vec3D& acquisitionLocation, const Vec3D& acquisitionDirection)
{
	static int counter = 0;
	int queue_size = 15;
	std::priority_queue<targeting_data<entt::entity>> targeting_queue;

	Vec3D pos = acquisitionLocation;
	Vec3D dir = acquisitionDirection;

	entt::entity result = entt::null;

	float maxScalarProd = -1.0f;

	// exclude TargetComponent, so missiles wont target other missiles
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent, AsteroidComponent>();
	if (view.begin() == view.end())
		return entt::null;

	for (auto entity : view)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(entity);
		Vec3D dx = entity_trf.location - pos;

		float scalarProd = dx * dir / dx.length();

		if (maxScalarProd < scalarProd)
		{
			maxScalarProd = scalarProd;
			result = entity;
		}
	}

	return result;
}

void InGame_layer::SpawnAsteroid(Vec3D center, Vec3D velocity, float spread)
{
	static int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DeformedSphere"];

	TransformComponent transform;
	transform.location = center + Vec3D(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500) * spread / 1000.0f;
	transform.orientation = Rotation((float)(rand() % 31415) / 10000.0f, Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	transform.scale = (float)(rand() % 20 + 2) / 20.0f;

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
	dynProps.velocity = velocity;
	dynProps.angular_velocity = 0.0001 * Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);

	Entity newEntity = m_Scene->CreateEntity("");

	// make the Earth hitting asteroids big
	Vec3D dr = transform.location;
	Vec3D v = velocity;
	if (sqrt(dr.lengthSquare() - pow(dr * v, 2) / v.lengthSquare()) < (2.3f + transform.scale + 0.2)) // 2.3 is the radius of the earth, 0.2 is a safety factor
	{
		transform.scale = 5.0f;
		newEntity.AddComponent<MarkerComponent>(MarkerComponent(1.0f, 0.0f, 0.0f, 1.0f));
	}

	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(asteroidIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<HitPointComponent>(transform.scale*20);
	// newEntity.AddComponent<HitPointComponent>(1.0f + (float)(rand() % 10));
	// newEntity.AddComponent<HitPointComponent>(1.0f);
	newEntity.AddComponent<AsteroidComponent>(AsteroidComponent());

}


void InGame_layer::SpawnDebris(Vec3D center, Vec3D velocity, float spread)
{
	static int asteroidIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];

	TransformComponent transform;
	transform.location = center;
	transform.orientation = Rotation((float)(rand() % 31415) / 10000.0f, Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	transform.scale = (float)(rand() % 10 + 2) / 100.0f;

	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
	dynProps.velocity = velocity + Vec3D(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500) * spread / 1000.0f;
	dynProps.angular_velocity = 0.0001 * Vec3D(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(asteroidIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(1000.0f)); // provide ttl in mili seconds
}


void InGame_layer::ShootBullett(TransformComponent transform, float velocity)
{
	static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];

	/*
	if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) {
		m_ShotSound.play();
	}
	*/
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 0.001f;
	dynProps.velocity = transform.orientation.f3 * velocity;
	transform.scale = 0.1f;
	transform.location += 0.1 * (transform.orientation.f3 - transform.orientation.f2);

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(transform);
	newEntity.AddComponent<MeshIndexComponent>(bulletIdx);
	newEntity.AddComponent<DynamicPropertiesComponent>(dynProps);
	newEntity.AddComponent<TimerComponent>(TimerComponent(10000.0f)); // provide ttl in mili seconds
	newEntity.AddComponent<BulletComponent>(BulletComponent());
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
	// dynProps.velocity = 00000001f * transform.orientation.f3; // 0.00000001f
	dynProps.velocity = 0.01f * transform.orientation.f1; // 0.00000001f
	dynProps.angular_velocity = Vec3D();
	transform.scale = 0.05f;
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
		DeActivate(); // this automatically activates the menu layer
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
	// static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];
	static int blueIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["BlueSphere"];
	static int yellowIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["YellowSphere"];
	static int explosionIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Explosion"];
	static int skip = 0;
	if (Input::IsMouseButtonPressed(sf::Mouse::Left))
	{
		ShootBullett(cam_trf, 0.05f);
		if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) {
			m_ShotSound.play();
		}
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Right) && skip % 2 == 0)
	{
		LaunchMissile(yellowIdx, cam_trf, GetClosestTarget(cam_trf.location, cam_trf.orientation.f3));
		if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) { m_ShotSound.play(); }
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Middle) && skip % 2 == 1)
	{
		// LaunchMissile(blueIdx, cam_trf, GetTarget());
		// LaunchMissile(blueIdx, cam_trf, GetTarget());
		// LaunchMissile(blueIdx, cam_trf, GetTarget());
		// LaunchMissile(bulletIdx, cam_trf, GetTarget());
		// if (m_ShotSound.getStatus() != sf::SoundSource::Status::Playing) { m_ShotSound.play(); }

		auto armed_ships = m_Scene->m_Registry.view<WeaponComponent, TransformComponent>();
		for (auto armed_ship : armed_ships)
		{
			TransformComponent& shipTrf = armed_ships.get<TransformComponent>(armed_ship);
			// LaunchMissile(bulletIdx, shipTrf, GetTarget());
			if (skip % 2 == 0)
				LaunchMissile(yellowIdx, shipTrf, GetTarget());
				//LaunchMissile(yellowIdx, shipTrf, GetClosestTarget(cam_trf.location, cam_trf.orientation.f3));
			else
				LaunchMissile(blueIdx, shipTrf, GetTarget());
				//LaunchMissile(blueIdx, shipTrf, GetClosestTarget(cam_trf.location, cam_trf.orientation.f3));
		}
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

void InGame_layer::UpdateScene(Timestep ts)
{
	m_Scene->m_MeshLibrary.Clear();

	auto timed_entities = m_Scene->m_Registry.view<TimerComponent>();
	for (auto entity : timed_entities)
	{
		TimerComponent& ttl = timed_entities.get<TimerComponent>(entity);
		ttl.timeToLive -= ts;
		if (ttl.timeToLive < 0.0f)
			m_Scene->m_Registry.destroy(entity);
	}

	auto HP_entities = m_Scene->m_Registry.view<HitPointComponent>();
	for (auto entity : HP_entities)
	{
		HitPointComponent& hp = HP_entities.get<HitPointComponent>(entity);
		if (hp.HP < 0.0f)
		{
			TransformComponent& trf = m_Scene->m_Registry.get<TransformComponent>(entity);
			DynamicPropertiesComponent& dyn = m_Scene->m_Registry.get<DynamicPropertiesComponent>(entity);
			dyn.velocity *= 0.5f;
			SpawnExplosion(trf, dyn);
			m_Scene->m_Registry.destroy(entity);
			m_ExplosionSound.play();
		}
	}

	auto explosions = m_Scene->m_Registry.view<ExplosionComponent>();
	for (auto explosion : explosions)
	{
		TransformComponent& trf = m_Scene->m_Registry.get<TransformComponent>(explosion);
		trf.scale += ts / 100.0f;
		ColourComponent& col = m_Scene->m_Registry.get<ColourComponent>(explosion);
		col.a = 1.0f / (trf.scale * trf.scale * trf.scale);
	}

	auto missiles = m_Scene->m_Registry.view<TargetComponent, TransformComponent, DynamicPropertiesComponent>();
	for (auto missile : missiles)
	{
		TargetComponent& target = missiles.get<TargetComponent>(missile);
		if (m_Scene->m_Registry.valid(target.targetEntity))
		{
			TransformComponent& missileTrf = missiles.get<TransformComponent>(missile);
			DynamicPropertiesComponent& missileVelocity = missiles.get<DynamicPropertiesComponent>(missile);
			TransformComponent& targetLoc = m_Scene->m_Registry.get<TransformComponent>(target.targetEntity);
			DynamicPropertiesComponent& targetVelocity = m_Scene->m_Registry.get<DynamicPropertiesComponent>(target.targetEntity);

			Vec3D dx = targetLoc.location - missileTrf.location;
			Vec3D dv = targetVelocity.velocity - missileVelocity.velocity;

			// check collision:
			float lambda = dv.lengthSquare() < 0.00001f ? 0 : -(dx * dv) / (ts * dv.lengthSquare());
			Vec3D hit_location = dx + dv * ts * std::min(1.0f, std::max(lambda, 0.0f));
			float distance = hit_location.length();

			// delete missille on hit (by setting its timer to 0, so it will be removed automatically)
			if (distance < (targetLoc.scale + missileTrf.scale))
			{
				TimerComponent& ttl = timed_entities.get<TimerComponent>(missile);
				ttl = 0.0f;
				HitPointComponent& targetHP = m_Scene->m_Registry.get<HitPointComponent>(target.targetEntity);
				targetHP.HP -= 10.0f;

				for (int k = 0; k < 30; k++)
				{
					SpawnDebris(missileTrf.location + hit_location - dx, targetVelocity.velocity, 0.05);
				}
			}
			float dt = sqrt(dx.lengthSquare() / dv.lengthSquare());
			dx += dt * dv*0.9f;
			float accel = 0.0001f;

			missileVelocity.velocity += accel * ts * dx / dx.length();
		}
		else
		{
			// this always returns false according the documentation: registry.valid(entt::tombstone);
			target.targetEntity = entt::tombstone;
			// acquire new target
			// TransformComponent& missileTrf = missiles.get<TransformComponent>(missile);
			// DynamicPropertiesComponent& missileVelocity = missiles.get<DynamicPropertiesComponent>(missile);
			// //target.targetEntity = GetTarget(missileTrf.location, missileVelocity.velocity);
			// target.targetEntity = GetClosestTarget(missileTrf.location, missileVelocity.velocity);
		}
	}


	auto bullets = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, BulletComponent>();
	for (auto bullet : bullets)
	{
		TransformComponent& bulletTrf = bullets.get<TransformComponent>(bullet);
		DynamicPropertiesComponent& bulletVelocity = bullets.get<DynamicPropertiesComponent>(bullet);

		static std::vector<entt::entity> bullet_vicinity;
		Box3D box; box.center = bulletTrf.location; box.radius = Vec3D(10, 10, 10);
		bullet_vicinity.clear();
		m_AsteroidOctTree->GetActiveTree()->QueryRange(box, bullet_vicinity, 0);

		for (int i = 0; i < bullet_vicinity.size(); i++)
		{
			// check validity
			if (m_Scene->m_Registry.valid(bullet_vicinity[i]))
			{
				TransformComponent& targetLoc = m_Scene->m_Registry.get<TransformComponent>(bullet_vicinity[i]);
				DynamicPropertiesComponent& targetVelocity = m_Scene->m_Registry.get<DynamicPropertiesComponent>(bullet_vicinity[i]);

				Vec3D dx = targetLoc.location - bulletTrf.location;
				Vec3D dv = targetVelocity.velocity - bulletVelocity.velocity;

				// check collision:
				float lambda = dv.lengthSquare() < 0.00001f ? 0 : -(dx * dv) / (ts * dv.lengthSquare());
				Vec3D hit_location = dx + dv * ts * std::min(1.0f, std::max(lambda, 0.0f));
				float distance = hit_location.length();

				// delete bullet on hit (by setting its timer to 0, so it will be removed automatically)
				if (distance < (targetLoc.scale + bulletTrf.scale))
				{
					TimerComponent& ttl = timed_entities.get<TimerComponent>(bullet);
					ttl = 0.0f;
					HitPointComponent& targetHP = m_Scene->m_Registry.get<HitPointComponent>(bullet_vicinity[i]);
					targetHP.HP -= 1.0f;

					bulletTrf.scale /= 8;
					// SpawnExplosion(bulletTrf, targetVelocity);
					for (int k = 0; k < 30; k++)
					{
						SpawnDebris(bulletTrf.location+ hit_location-dx, targetVelocity.velocity, 0.05);
						// SpawnDebris(hit_location, targetVelocity.velocity, 0.1);
					}
				}
			}

		}

	}

	auto asteroids = m_Scene->m_Registry.view<TransformComponent, AsteroidComponent>();
	for (auto asteroid : asteroids)
	{
		TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
		if (asteroidTrf.location.length() < 2.3f + asteroidTrf.scale) // 2.3 is the radius of the earth
		{
			SpawnExplosion(asteroidTrf, DynamicPropertiesComponent());
			m_Scene->m_Registry.destroy(asteroid);
			m_ExplosionSound.play();
		}
	}

	// update wo collision
	auto grav_masses = m_Scene->m_Registry.view<TransformComponent, GravitationalMassComponent>();
	auto view = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, MeshIndexComponent>();
	for (auto entity : view)
	{
		TransformComponent& entity_trf = view.get<TransformComponent>(entity);
		if (entity_trf.location.lengthSquare() > 1000000)
		{
			m_Scene->m_Registry.destroy(entity);
			continue;
		}
		DynamicPropertiesComponent& dyn_prop = view.get<DynamicPropertiesComponent>(entity);

		entity_trf.location += ts * dyn_prop.velocity;
		entity_trf.orientation = Rotation(ts * dyn_prop.angular_velocity.length(), dyn_prop.angular_velocity) * entity_trf.orientation;

		// this is not that great, should be separated cus meshes wo dyn props wont be drawn (they dont get into the mesh library)
		MeshIndexComponent& idx = view.get<MeshIndexComponent>(entity);
		m_Scene->m_MeshLibrary.m_MeshTransforms[idx.idx].push_back(entity_trf);

		if (m_Scene->m_Registry.all_of<ColourComponent>(entity))
		{
			std::shared_ptr<Mesh> tmp_mesh = m_Scene->m_MeshLibrary.m_Meshes[idx.idx];
			int colBufIdx = tmp_mesh->GetColourInstances();
			if (colBufIdx != -1)
			{
				ColourComponent& colour = m_Scene->m_Registry.get<ColourComponent>(entity);
				m_Scene->m_MeshLibrary.m_ColourBuffers[colBufIdx].push_back(colour);
			}
		}

		if (!m_Scene->m_Registry.all_of<ExplosionComponent>(entity))
		{
			Vec3D acceleration = { 0,0,0 };
			for (auto grav_entity : grav_masses)
			{
				if (entity != grav_entity)
				{
					GravitationalMassComponent& grav_mass = grav_masses.get<GravitationalMassComponent>(grav_entity);
					TransformComponent& grav_trf = grav_masses.get<TransformComponent>(grav_entity);
					Vec3D dr = grav_trf.location - entity_trf.location;
					acceleration += grav_mass.gravitational_mass * dr / pow(dr.length(), 3); // a gravitatonal constant scalar could be added here
				}
			}
			dyn_prop.velocity += ts * acceleration;
		}
	}

	// mark asteroids that are on collision course with the earth
	/*static*/ int markerIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["DefaultMarker"];
	/*static*/ std::shared_ptr<Mesh> marker_mesh = m_Scene->m_MeshLibrary.m_Meshes[markerIdx];
	/*static*/ int markerColBufIdx = marker_mesh->GetColourInstances();

	auto marked_entities= m_Scene->m_Registry.view<TransformComponent, MarkerComponent>();
	for (auto marked_entity : marked_entities)
	{
		TransformComponent& entityTrf = marked_entities.get<TransformComponent>(marked_entity);
		MarkerComponent& marker = marked_entities.get<MarkerComponent>(marked_entity);
		m_Scene->m_MeshLibrary.m_MeshTransforms[markerIdx].push_back(entityTrf);
		m_Scene->m_MeshLibrary.m_ColourBuffers[markerColBufIdx].push_back(marker.marker_colour);
	}
}


void InGame_layer::SpawnExplosion(TransformComponent trf, DynamicPropertiesComponent dyn)
{
	static int explosionIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Explosion"];

	Entity newEntity = m_Scene->CreateEntity("");
	newEntity.AddComponent<TransformComponent>(trf);
	newEntity.AddComponent<MeshIndexComponent>(explosionIdx);
	newEntity.AddComponent<ColourComponent>(ColourComponent(0.8, 0.1f + float(rand() % 1000) / 5000.0f, 0.1f + float(rand() % 1000) / 5000.0f, 0.8f));
	newEntity.AddComponent<DynamicPropertiesComponent>(dyn);
	newEntity.AddComponent<TimerComponent>(TimerComponent(2000.0f));
	newEntity.AddComponent<ExplosionComponent>(ExplosionComponent());
}




