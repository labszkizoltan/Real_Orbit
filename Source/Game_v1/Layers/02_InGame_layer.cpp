
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
#include <utils/RandomGeneration.h>
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

	m_EntityManager.SetScene(m_Scene.get());
	m_EntityManager.CreateStars();

	// m_Font = std::make_unique<ROFont>("assets/fonts/ImageJ_alphabet_3.png", "assets/fonts/ImageJ_alphabet_3_description.txt");
	m_SceneRenderer.SetScene(m_Scene);

	Box3D tmp_box; tmp_box.radius = Vec3D(10000, 10000, 10000);
	m_CollidersOctTree = std::make_shared<DualOctTree>(tmp_box);
	m_MissillesOctTree = std::make_shared<DualOctTree>(tmp_box);

	m_Scene->m_Registry.on_destroy<VictoryComponent>().connect<&InGame_layer::OnVictoryComponentDestroyed>(this);

	//	m_FbDisplay.SetTexture(Renderer::GetColorAttachment());
	m_FbDisplay.SetTexture(Renderer::GetBlurredAttachment());

	m_ImgProcessor = std::make_unique<ImageProcessor>();
	m_ImgProcessor->SetMipMapLevel(4);

	m_AudioManager.SetVolume(10.0f);
	m_AudioManager.SetIntroSpeech("assets/audio/EarthMission_introSpeech.wav");

}

void InGame_layer::OnDetach()
{
	LOG_INFO("InGame_layer detached");
}

void InGame_layer::OnUpdate(Timestep ts)
{
	static int windowHeight = GameApplication::Get().GetWindow().GetHeight();
	static int windowWidth = GameApplication::Get().GetWindow().GetWidth();
	m_AudioManager.PlayMusic();
	m_AudioManager.PlayIntroSpeech();

	TransformComponent& light_trf = m_Scene->GetLight();
	light_trf.location = m_Player.m_Transform.location+Vec3D(-10, 0, 0);

	HandleUserInput(ts);
	if (m_Player.m_Transform.location.length() < 5.0f)
		m_Player.FillReserves(m_SimulationSpeed * ts);
	
	if (m_Loose || m_Victory)
	{
		m_SceneRenderer.RenderScene();
		m_ImgProcessor->Blur(g_RendererBrightColAttchSlot, Renderer::s_BlurBuffer);
		m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, g_RendererBlurredSlot);

		if (m_Loose)
		{
			GameApplication::Game_DrawText("Game Over",
				Vec3D(700, 600, 0), // parameterize the screen coordinates
				Vec3D(0.9f, 0.3f, 0.3f),
				2.0f);
			GameApplication::Game_DrawText("press Esc to return to menu",
				Vec3D(650, 550, 0),
				Vec3D(0.9f, 0.3f, 0.3f),
				1.0f);
		}
		else if (m_Victory)
		{
			GameApplication::Game_DrawText("Victory",
				Vec3D(800, 600, 0), // parameterize the screen coordinates
				Vec3D(0.3f, 0.9f, 0.3f),
				2.0f);
			GameApplication::Game_DrawText("press Esc to return to menu",
				Vec3D(650, 550, 0),
				Vec3D(0.3f, 0.9f, 0.3f),
				1.0f);
		}


		GameApplication::Game_DrawText("Elapsed Game Time - " + std::to_string((int)(m_ElapsedTime / 1000.0f)), Vec3D(10, windowHeight - 70, 0), Vec3D(0.3f, 0.9f, 0.5f), 0.5f);
		GameApplication::Game_DrawText("Simulation Speed: " + std::to_string((int)(m_SimulationSpeed / 0.2f)), Vec3D(windowWidth - 230, windowHeight - 70, 0), Vec3D(0.3f, 0.9f, 0.5f), 0.5f);
		GameApplication::Game_DrawText("Asteroid Impacts - " + std::to_string(m_EarthHitCount) + " / " + std::to_string(m_MaxEarthHitCount),
			Vec3D(700, windowHeight - 70, 0),
			Vec3D(0.3f, 0.9f, 0.5f),
			1.0f);
		m_Player.DrawStatsOnScreen();

		return;
	}

	{
		static int fleetSpawnCounter = 0;

		int asteroid_count = 128;
		float spawn_frequency = 10000.0f;
		static float asteroid_spawn_timer = spawn_frequency; // in milli seconds
		asteroid_spawn_timer -= m_SimulationSpeed * ts;
		if (asteroid_spawn_timer < 0.0f)
		{
			fleetSpawnCounter++;
			asteroid_spawn_timer = spawn_frequency;
			Vec3D center = Vec3D(50, 0, 0);
			Vec3D velocity = -0.01 * center / center.length();
			for (int i = 0; i < asteroid_count; i++)
				m_EntityManager.SpawnAsteroid(500.0f * center / center.length(), velocity, 80.0f, 2.5f);

			m_EntityManager.SpawnPickupAsteroid(500.0f * center / center.length(), velocity, 80.0f, 2.5f, PickupType::HEALTH);

			if (fleetSpawnCounter % 4 == 0)
			{
				TransformComponent trf; trf.location = 500.0f * center / center.length(); trf.orientation = Mat_3D(Vec3D(0, -1, 0), Vec3D(0, 0, -1), Vec3D(1, 0, 0)); trf.scale = 0.5f;
				DynamicPropertiesComponent dynProp; dynProp.velocity = velocity;
				m_EntityManager.SpawnShipFormation(trf, dynProp);
			}
		}
	}

	PartialUpdate();
	// if the update-render order is swapped, something is un-initialized and the program fails at alpha mesh rendering
	m_SceneRenderer.RenderScene();
	// m_SceneUpdater.UpdateScene(m_SimulationSpeed * ts);
	UpdateScene(m_SimulationSpeed * ts);
//	m_CollidersOctTree->Update<ColliderComponent>(m_Scene.get());
//	m_MissillesOctTree->Update<TargetComponent, DynamicPropertiesComponent>(m_Scene.get());
	//m_AntiMissilleOctTree->Update<AntiMissilleComponent>(m_Scene.get());

	//	m_ImgProcessor->Blur(g_RendererBlurDepthSlot, Renderer::s_BlurBuffer); // this is not working, as expected
	m_ImgProcessor->Blur(g_RendererBrightColAttchSlot, Renderer::s_BlurBuffer);

	//	m_FbDisplay.Draw();
	m_FbDisplay.DrawCombined(g_RendererColorAttchSlot, g_RendererBlurredSlot);

	GameApplication::Game_DrawText("Elapsed Game Time - " + std::to_string((int)(m_ElapsedTime / 1000.0f)), Vec3D(10, windowHeight - 70, 0), Vec3D(0.3f, 0.9f, 0.5f), 0.5f);
	GameApplication::Game_DrawText("Simulation Speed: " + std::to_string((int)(m_SimulationSpeed / 0.2f)), Vec3D(windowWidth - 230, windowHeight - 70, 0), Vec3D(0.3f, 0.9f, 0.5f), 0.5f);
	GameApplication::Game_DrawText("Asteroid Impacts - " + std::to_string(m_EarthHitCount) + " / " + std::to_string(m_MaxEarthHitCount),
		Vec3D(700, windowHeight - 70, 0),
		Vec3D(0.3f, 0.9f, 0.5f),
		1.0f);
	m_Player.DrawStatsOnScreen();

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
	m_AudioManager.ContinueMusic();
	//m_Music.play();
	Renderer::SetZoomLevel(m_ZoomLevel);
}

void InGame_layer::DeActivate()
{
	Application& app = Application::Get();
	((GameApplication*)(&app))->ActitivateLayer(GameLayers::MENU_LAYER);
	m_AudioManager.PauseMusic();
	m_AudioManager.StopIntroSpeech();
	// m_Music.pause();
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

	Vec3D pos = acquisitionLocation;
	Vec3D dir = acquisitionDirection;

	entt::entity result = entt::null;

	float maxScalarProd = -1.0f;

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
}

void InGame_layer::ResetLayer()
{
	LOG_INFO("InGame_layer reset");

	m_Scene = std::make_shared<Scene>();

	SceneSerializer serializer(m_Scene);
	serializer.DeSerialize_file("assets/scenes/02_InGameLayer.yaml");

	m_EntityManager.SetScene(m_Scene.get());
	m_SceneRenderer.SetScene(m_Scene);

	m_Scene->m_Registry.on_destroy<VictoryComponent>().connect<&InGame_layer::OnVictoryComponentDestroyed>(this);
	
	m_ElapsedTime = 0.0f;
	m_SimulationSpeed = 0.2f;
	m_ZoomLevel = g_InitialZoomLevel;

	m_Player = Player();

	m_CameraContinuousRotation = false;
	
	m_EarthHitCount = 0;

	m_EntityManager.CreateStars();

	m_AudioManager.m_IntroPlayed = false;

	m_Loose = false;
	m_Victory = false;
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
		if (m_Loose || m_Victory)
			ResetLayer();
	}
	else if (event.key.code == sf::Keyboard::Key::Space)
		m_SimulationSpeed = 0.0f;
	else if (event.key.code == sf::Keyboard::Key::Num1)
		m_SimulationSpeed = 0.2f;
	else if (event.key.code == sf::Keyboard::Key::Num2)
		m_SimulationSpeed = 1.0f;
	else if (event.key.code == sf::Keyboard::Key::Num3)
		m_SimulationSpeed = 5.0f;
	else if (event.key.code == sf::Keyboard::Key::C) {
		m_CameraContinuousRotation = !m_CameraContinuousRotation;
		static int center_x = Application::Get().GetWindow().GetWidth() / 2;
		static int center_y = Application::Get().GetWindow().GetHeight() / 2;
		sf::Mouse::setPosition(sf::Vector2i(center_x, center_y), Application::Get().GetWindow().GetNativeWindow());
	}

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

	// m_Player.m_Transform = cam_trf;
	cam_trf = m_Player.m_Transform;

	// static float player_acceleration = 0.000003f;
	float player_acceleration = 0.000003f;
	float fuel_consumption_multiplier = 1.0f;
	if (Input::IsKeyPressed(sf::Keyboard::Key::LShift)) { player_acceleration *= 3; fuel_consumption_multiplier = 6.0f; }

	// moves
	if (Input::IsKeyPressed(sf::Keyboard::Key::W) && m_Player.m_Fuel > 0) { m_Player.m_DynamicProps.velocity += m_SimulationSpeed * ts * player_acceleration * m_Player.m_Transform.orientation.f3; m_Player.m_Fuel -= (fuel_consumption_multiplier * m_SimulationSpeed / 0.2f); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::S) && m_Player.m_Fuel > 0) { m_Player.m_DynamicProps.velocity -= m_SimulationSpeed * ts * player_acceleration * m_Player.m_Transform.orientation.f3; m_Player.m_Fuel -= (fuel_consumption_multiplier * m_SimulationSpeed / 0.2f); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::A) && m_Player.m_Fuel > 0) { m_Player.m_DynamicProps.velocity -= m_SimulationSpeed * ts * player_acceleration * m_Player.m_Transform.orientation.f1; m_Player.m_Fuel -= (fuel_consumption_multiplier * m_SimulationSpeed / 0.2f); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::D) && m_Player.m_Fuel > 0) { m_Player.m_DynamicProps.velocity += m_SimulationSpeed * ts * player_acceleration * m_Player.m_Transform.orientation.f1; m_Player.m_Fuel -= (fuel_consumption_multiplier * m_SimulationSpeed / 0.2f); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::R) && m_Player.m_Fuel > 0) { m_Player.m_DynamicProps.velocity += m_SimulationSpeed * ts * player_acceleration * m_Player.m_Transform.orientation.f2; m_Player.m_Fuel -= (fuel_consumption_multiplier * m_SimulationSpeed / 0.2f); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::F) && m_Player.m_Fuel > 0) { m_Player.m_DynamicProps.velocity -= m_SimulationSpeed * ts * player_acceleration * m_Player.m_Transform.orientation.f2; m_Player.m_Fuel -= (fuel_consumption_multiplier * m_SimulationSpeed / 0.2f); }

	// rotations
	if (Input::IsKeyPressed(sf::Keyboard::Key::Q)) { m_Player.m_Transform.orientation = Rotation(0.001f * ts, m_Player.m_Transform.orientation.f3) * m_Player.m_Transform.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::E)) { m_Player.m_Transform.orientation = Rotation(-0.001f * ts, m_Player.m_Transform.orientation.f3) * m_Player.m_Transform.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Up)) { m_Player.m_Transform.orientation = Rotation(0.001f * ts, m_Player.m_Transform.orientation.f1) * m_Player.m_Transform.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Down)) { m_Player.m_Transform.orientation = Rotation(-0.001f * ts, m_Player.m_Transform.orientation.f1) * m_Player.m_Transform.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Left)) { m_Player.m_Transform.orientation = Rotation(-0.001f * ts, m_Player.m_Transform.orientation.f2) * m_Player.m_Transform.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Right)) { m_Player.m_Transform.orientation = Rotation(0.001f * ts, m_Player.m_Transform.orientation.f2) * m_Player.m_Transform.orientation; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::P)) { ZoomIn(); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::O)) { ZoomOut(); }

	// if (Input::IsKeyPressed(sf::Keyboard::Key::Add)) { player_acceleration *= 1.1f; }
	// if (Input::IsKeyPressed(sf::Keyboard::Key::Subtract)) { player_acceleration /= 1.1f; }

	if (Input::IsKeyPressed(sf::Keyboard::Key::B)) { m_Player.m_DynamicProps.velocity = Vec3D(0,0,0); }

	static float cam_velocity = 0.001f;
	// moves, for debugging
	if (Input::IsKeyPressed(sf::Keyboard::Key::I)) { m_Player.m_Transform.location += cam_velocity * m_Player.m_Transform.orientation.f3; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::K)) { m_Player.m_Transform.location -= cam_velocity * m_Player.m_Transform.orientation.f3; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::J)) { m_Player.m_Transform.location -= cam_velocity * m_Player.m_Transform.orientation.f1; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::L)) { m_Player.m_Transform.location += cam_velocity * m_Player.m_Transform.orientation.f1; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Z)) { m_Player.m_Transform.location += cam_velocity * m_Player.m_Transform.orientation.f2; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::H)) { m_Player.m_Transform.location -= cam_velocity * m_Player.m_Transform.orientation.f2; }

	if (Input::IsKeyPressed(sf::Keyboard::Key::Add)) { cam_velocity *= 1.1f; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Subtract)) { cam_velocity /= 1.1f; }


	static int blueIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["BlueSphere"];
	static int yellowIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["YellowSphere"];
	static int explosionIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Explosion"];
	static int skip = 0;
	if (Input::IsMouseButtonPressed(sf::Mouse::Left) && m_Player.m_BulletCount > 0)
	{
		float phi = RORNG::runif()*2*3.1415926535, r = RORNG::runif();
		float x = cos(phi), y = sin(phi);
		m_EntityManager.ShootBullett(m_Player.m_Transform,
			m_Player.m_DynamicProps.velocity + (m_Player.m_Transform.orientation.f3+0.01f*r*(x* m_Player.m_Transform.orientation.f1+y* m_Player.m_Transform.orientation.f2)) * m_Player.m_BulletSpeed);
		// m_EntityManager.ShootBullett(m_Player.m_Transform, m_Player.m_DynamicProps.velocity + m_Player.m_Transform.orientation.f3 * m_Player.m_BulletSpeed);
		m_Player.m_BulletCount--;
		m_AudioManager.PlayShotSound();
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Right) && skip % 2 == 0 && m_Player.m_MissilleCount > 0)
	{
		m_EntityManager.LaunchMissile(yellowIdx, m_Player.m_Transform, DynamicPropertiesComponent(), GetClosestTarget(m_Player.m_Transform.location, m_Player.m_Transform.orientation.f3));
		m_Player.m_MissilleCount--;
		m_AudioManager.PlayShotSound();
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Middle) && skip % 2 == 1)
	{
		auto armed_ships = m_Scene->m_Registry.view<WeaponComponent, TransformComponent, DynamicPropertiesComponent>();
		for (auto armed_ship : armed_ships)
		{
			TransformComponent& shipTrf = armed_ships.get<TransformComponent>(armed_ship);
			DynamicPropertiesComponent& shipVelocity = armed_ships.get<DynamicPropertiesComponent>(armed_ship);
			// LaunchMissile(bulletIdx, shipTrf, GetTarget());
			if ((skip/2) % 2 == 0)
				m_EntityManager.LaunchMissile(yellowIdx, shipTrf, shipVelocity, GetTarget());
				//m_EntityManager.LaunchMissile(yellowIdx, shipTrf, GetClosestTarget(cam_trf.location, cam_trf.orientation.f3));
			else
				m_EntityManager.LaunchMissile(blueIdx, shipTrf, shipVelocity, GetTarget());
				//m_EntityManager.LaunchMissile(blueIdx, shipTrf, GetClosestTarget(cam_trf.location, cam_trf.orientation.f3));
		}
	}
	if (!Input::IsMouseButtonPressed(sf::Mouse::Left) && !Input::IsMouseButtonPressed(sf::Mouse::Right) && !Input::IsMouseButtonPressed(sf::Mouse::Middle))
		m_AudioManager.StopShotSound();

	skip++;


	if (Input::IsKeyPressed(sf::Keyboard::Key::Space))
	{
		TransformComponent& light_trf = m_Scene->GetLight();
		light_trf.location = m_Player.m_Transform.location;
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
			Vec3D rotationAxis = dy * m_Player.m_Transform.orientation.f1 + dx * m_Player.m_Transform.orientation.f2;
			Mat_3D rotationMatrix = Rotation(ts * (0.00025f * radiusFromCenter + 0.0000001f * radiusFromCenterSqr) / m_ZoomLevel, rotationAxis); // angle could be divided by zoom level to slow down turning when really zoomed into something

			m_Player.m_Transform.orientation = rotationMatrix * m_Player.m_Transform.orientation;
			sf::Mouse::setPosition(sf::Vector2i(center_x, center_y), Application::Get().GetWindow().GetNativeWindow());
		}
		else
		{
			float r_min_square = 50.0f * 50.0f;
			sf::Vector2i mousePos = Input::GetMousePosition();
			mousePos.x -= Application::Get().GetWindow().GetWidth() / 2;
			mousePos.y -= Application::Get().GetWindow().GetHeight() / 2;
			float radiusFromCenter_square = (mousePos.x * mousePos.x + mousePos.y * mousePos.y);
			Vec3D rotationAxis = mousePos.y * m_Player.m_Transform.orientation.f1 + mousePos.x * m_Player.m_Transform.orientation.f2;
			Mat_3D rotationMatrix = Rotation(0.00000001f * ts * std::max(0.0f, radiusFromCenter_square - r_min_square) / m_ZoomLevel, rotationAxis); // angle could be divided by zoom level to slow down turning when really zoomed into something

			m_Player.m_Transform.orientation = rotationMatrix * m_Player.m_Transform.orientation;
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

void InGame_layer::PartialUpdate()
{
	static int phaseCounter = 0;

	if (phaseCounter == 0)
	{
		EvaluateLossCondition();
	}
	else if (phaseCounter == 1)
	{
		m_CollidersOctTree->Update<ColliderComponent>(m_Scene.get());
	}
	else if (phaseCounter == 2)
	{
		m_MissillesOctTree->Update<TargetComponent, DynamicPropertiesComponent>(m_Scene.get());
	}
	else if (phaseCounter == 3)
	{
	}
	else if (phaseCounter == 4)
	{
	}
	else if (phaseCounter == 5)
	{
	}
	else if (phaseCounter == 6)
	{
	}
	else if (phaseCounter == 7)
	{
	}
	else if (phaseCounter == 8)
	{
	}
	else if (phaseCounter == 9)
	{
	}

	phaseCounter = (phaseCounter + 1) % 10;
}

void InGame_layer::UpdateScene(Timestep ts)
{
	m_Scene->m_MeshLibrary.Clear();

	OnPickupDestroyed();
	m_Player.TakePickUp(m_Scene, 2.0f, 0.05f);

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
			m_EntityManager.SpawnExplosion(trf, dyn, ColourComponent(0.8, 0.1f + float(rand() % 1000) / 5000.0f, 0.1f + float(rand() % 1000) / 5000.0f, 0.8f));
			m_Scene->m_Registry.destroy(entity);
			m_AudioManager.PlayExplosionSound();
			//m_ExplosionSound.play();
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

			static std::vector<entt::entity> missille_vicinity;
			Box3D box; box.center = missileTrf.location; box.radius = Vec3D(10, 10, 10);
			missille_vicinity.clear();
			m_CollidersOctTree->GetActiveTree()->QueryRange(box, missille_vicinity, 0);

			Vec3D avoidanceVector = Vec3D();

			for (int i = 0; i < missille_vicinity.size(); i++)
			{
				// check validity
				if (m_Scene->m_Registry.valid(missille_vicinity[i]))
				{
					TransformComponent& targetLoc = m_Scene->m_Registry.get<TransformComponent>(missille_vicinity[i]);
					DynamicPropertiesComponent& targetVelocity = m_Scene->m_Registry.get<DynamicPropertiesComponent>(missille_vicinity[i]);

					Vec3D dx = targetLoc.location - missileTrf.location;
					Vec3D dv = targetVelocity.velocity - missileVelocity.velocity;

					// avoidanceVector += dx / dx.lengthSquare();
					avoidanceVector += dx / dx.length() / (dx.length()- targetLoc.scale);

					// check collision:
					float lambda = dv.lengthSquare() < 0.00001f ? 0 : -(dx * dv) / (ts * dv.lengthSquare());
					Vec3D hit_location = dx + dv * ts * std::min(1.0f, std::max(lambda, 0.0f));
					float distance = hit_location.length();

					// delete bullet on hit (by setting its timer to 0, so it will be removed automatically)
					if (distance < (targetLoc.scale + missileTrf.scale))
					{
						TimerComponent& ttl = timed_entities.get<TimerComponent>(missile);
						ttl = 0.0f;
						HitPointComponent& targetHP = m_Scene->m_Registry.get<HitPointComponent>(missille_vicinity[i]);
						targetHP.HP -= 10.0f;

						missileTrf.scale /= 8;

						for (int k = 0; k < 15; k++)
						{
							m_EntityManager.SpawnDebris(missileTrf.location + hit_location - dx, targetVelocity.velocity, 0.05, m_BulletSpawnChance);
							m_EntityManager.SpawnDebris(missileTrf.location + hit_location - dx, missileVelocity.velocity / 4, 0.1, m_BulletSpawnChance);
							m_EntityManager.SpawnDebris(missileTrf.location + hit_location - dx, missileVelocity.velocity / 4, 0.1, m_BulletSpawnChance);
						}
					}
				}

			}
			TransformComponent& targetLoc = m_Scene->m_Registry.get<TransformComponent>(target.targetEntity);
			DynamicPropertiesComponent& targetVelocity = m_Scene->m_Registry.get<DynamicPropertiesComponent>(target.targetEntity);

			Vec3D dx = targetLoc.location - missileTrf.location;
			Vec3D dv = targetVelocity.velocity - missileVelocity.velocity;

			float dt = sqrt(dx.lengthSquare() / dv.lengthSquare());
			dx += dt * dv*0.9f;
			float accel = 0.0001f;

			// This is the place where I can add repulsive forces, so the missilles avoid nearby objects and focus on reaching the target!!!
			// missileVelocity.velocity += accel * ts * dx / dx.length();
			missileVelocity.velocity += accel * ts * (dx + 2 * avoidanceVector) / (dx + 2 * avoidanceVector).length();
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
		// auto start = std::chrono::high_resolution_clock::now();

		TransformComponent& bulletTrf = bullets.get<TransformComponent>(bullet);
		DynamicPropertiesComponent& bulletVelocity = bullets.get<DynamicPropertiesComponent>(bullet);

		static std::vector<entt::entity> bullet_vicinity;
		Box3D box; box.center = bulletTrf.location; box.radius = Vec3D(10, 10, 10);
		bullet_vicinity.clear();
		m_CollidersOctTree->GetActiveTree()->QueryRange(box, bullet_vicinity, 0);


		for (int i = 0; i < bullet_vicinity.size(); i++)
		{
			// check validity
			if (m_Scene->m_Registry.valid(bullet_vicinity[i]) && bullet_vicinity[i] != bullet) // do not let the entity collide with itself if it has a collider component
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
					// m_EntityManager.SpawnExplosion(bulletTrf, targetVelocity);
					for (int k = 0; k < 30; k++)
					{
						m_EntityManager.SpawnDebris(bulletTrf.location+ hit_location-dx, targetVelocity.velocity, 0.05, m_BulletSpawnChance);
						// m_EntityManager.SpawnDebris(hit_location, targetVelocity.velocity, 0.1);
					}
				}
			}

		}

		// based on ~5500 averaged bullet update time, updating one bullet takes ~ 2.8 micro seconds
		// auto finish = std::chrono::high_resolution_clock::now();
		// auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
		// std::cout << "bullet update took:" << duration.count() << "\n";

	}

	auto asteroids = m_Scene->m_Registry.view<TransformComponent, AsteroidComponent>();
	for (auto asteroid : asteroids)
	{
		TransformComponent& asteroidTrf = asteroids.get<TransformComponent>(asteroid);
		if (asteroidTrf.location.length() < 2.3f + asteroidTrf.scale) // 2.3 is the radius of the earth
		{
			m_EntityManager.SpawnExplosion(asteroidTrf, DynamicPropertiesComponent(), ColourComponent(0.8, 0.1f + float(rand() % 1000) / 5000.0f, 0.1f + float(rand() % 1000) / 5000.0f, 0.8f));
			m_Scene->m_Registry.destroy(asteroid);
			m_AudioManager.PlayExplosionSound();

			m_EarthHitCount++;
		}
	}

	UpdateEnemyShips(ts);

	auto anti_missilles = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, AntiMissilleComponent>();
	for (auto antiMissille : anti_missilles)
	{
		TransformComponent& antiMTrf = anti_missilles.get<TransformComponent>(antiMissille);

		static std::vector<entt::entity> antiM_vicinity;
		Box3D box; box.center = antiMTrf.location; box.radius = 2.0f * Vec3D(antiMTrf.scale, antiMTrf.scale, antiMTrf.scale);
		antiM_vicinity.clear();
		m_MissillesOctTree->GetActiveTree()->QueryRange(box, antiM_vicinity, 0);
		const float hit_chance = 0.1f;
		if (antiM_vicinity.size() > 0 && m_Scene->m_Registry.valid(antiM_vicinity[0]) && RORNG::runif() < hit_chance)
		{
			TimerComponent& missilleTimer = m_Scene->m_Registry.get<TimerComponent>(antiM_vicinity[0]);
			TransformComponent& missilleLoc = m_Scene->m_Registry.get<TransformComponent>(antiM_vicinity[0]);
			DynamicPropertiesComponent& missilleVel = m_Scene->m_Registry.get<DynamicPropertiesComponent>(antiM_vicinity[0]);
			TimerComponent& antiMTimer = m_Scene->m_Registry.get<TimerComponent>(antiMissille);
			m_EntityManager.SpawnExplosion(missilleLoc, missilleVel, ColourComponent(0.1f + float(rand() % 1000) / 5000.0f, 0.1f + float(rand() % 1000) / 5000.0f, 0.8f, 0.8f));

			missilleTimer = 0.0f;
			antiMTimer = 0.0f;
		}

	}

	// update wo collision
	auto grav_masses = m_Scene->m_Registry.view<TransformComponent, GravitationalMassComponent>();
	// first the player:
	Vec3D playerAcceleration = { 0,0,0 };
	for (auto grav_entity : grav_masses)
	{
		GravitationalMassComponent& grav_mass = grav_masses.get<GravitationalMassComponent>(grav_entity);
		TransformComponent& grav_trf = grav_masses.get<TransformComponent>(grav_entity);
		Vec3D dr = grav_trf.location - m_Player.m_Transform.location;
		playerAcceleration += grav_mass.gravitational_mass * dr / pow(dr.length(), 3);
	}
	m_Player.Update(ts, playerAcceleration);

	// check collisions of the player:
	static std::vector<entt::entity> player_vicinity;
	Box3D box; box.center = m_Player.m_Transform.location; box.radius = Vec3D(50, 50, 50);
	player_vicinity.clear();
	m_CollidersOctTree->GetActiveTree()->QueryRange(box, player_vicinity, 0);
	for (int i = 0; i < player_vicinity.size(); i++)
	{
		if (m_Scene->m_Registry.valid(player_vicinity[i]) && m_Scene->m_Registry.all_of<HitPointComponent>(player_vicinity[i]))
		{
			TransformComponent& transform = m_Scene->m_Registry.get<TransformComponent>(player_vicinity[i]);
			HitPointComponent& hitpoint = m_Scene->m_Registry.get<HitPointComponent>(player_vicinity[i]);
			if ((m_Player.m_Transform.location - transform.location).length() < m_Player.m_Transform.scale + transform.scale)
			{
				m_Player.m_Health -= hitpoint.HP;
				hitpoint.HP = -1;
			}
		}
	}

	// then the rest of the objects
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

	auto non_moving_view = m_Scene->m_Registry.view<TransformComponent, MeshIndexComponent>(entt::exclude<DynamicPropertiesComponent>);
	for (auto entity : non_moving_view)
	{
		TransformComponent& entity_trf = non_moving_view.get<TransformComponent>(entity);
		MeshIndexComponent& idx = non_moving_view.get<MeshIndexComponent>(entity);
		m_Scene->m_MeshLibrary.m_MeshTransforms[idx.idx].push_back(entity_trf);
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

	// add velocity markers:
	TransformComponent velocityMarkerLoc;
	// yellow -> forward velocity marker
	velocityMarkerLoc.location = m_Player.m_Transform.location + 0.5f * m_Player.m_DynamicProps.velocity / m_Player.m_DynamicProps.velocity.length();
	m_Scene->m_MeshLibrary.m_MeshTransforms[markerIdx].push_back(velocityMarkerLoc);
	m_Scene->m_MeshLibrary.m_ColourBuffers[markerColBufIdx].push_back(ColourComponent(1, 1, 0, 1));

	// orange -> backward velocity marker
	velocityMarkerLoc.location = m_Player.m_Transform.location - 0.5f * m_Player.m_DynamicProps.velocity / m_Player.m_DynamicProps.velocity.length();
	m_Scene->m_MeshLibrary.m_MeshTransforms[markerIdx].push_back(velocityMarkerLoc);
	m_Scene->m_MeshLibrary.m_ColourBuffers[markerColBufIdx].push_back(ColourComponent(1, 0.6f, 0, 1));

	// blue -> center marker for easier navigation
	velocityMarkerLoc.location = m_Player.m_Transform.location * (1.0f - 1.0f / m_Player.m_Transform.location.length());
	m_Scene->m_MeshLibrary.m_MeshTransforms[markerIdx].push_back(velocityMarkerLoc);
	m_Scene->m_MeshLibrary.m_ColourBuffers[markerColBufIdx].push_back(ColourComponent(0, 0, 1, 1));
}

void InGame_layer::OnPickupDestroyed(entt::registry& registry, entt::entity entity)
{

	if (registry.all_of<TransformComponent>(entity))
	{
		TransformComponent& trf = registry.get<TransformComponent>(entity);
		Vec3D center = trf.location;
		// Vec3D center = Vec3D(0, 50, 0);
		Vec3D velocity = -0.01 * center / center.length();
		m_EntityManager.SpawnAsteroid(500.0f * center / center.length(), velocity, 0.0f, 50.0f);
	}
	else
	{
		TransformComponent& trf = registry.get<TransformComponent>(entity);
		Vec3D center = Vec3D(0, 50, 0);
		Vec3D velocity = -0.01 * center / center.length();
		m_EntityManager.SpawnAsteroid(500.0f * center / center.length(), velocity, 50.0f, 2.3f);
	}

}

void InGame_layer::OnPickupDestroyed()
{
	static int plusIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Plus"];
	static int dropletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Droplet"];
	static int missilleIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Missille"];

	static int counter = 0;
	auto pickupEntities = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, HitPointComponent, HiddenPickupComponent>();
	for (auto entity : pickupEntities)
	{
		HitPointComponent& hpc = pickupEntities.get<HitPointComponent>(entity);

		if (hpc.HP < 0.0f)
		{
			counter++;
			TransformComponent& trf = pickupEntities.get<TransformComponent>(entity); trf.scale = 1.0f;
			DynamicPropertiesComponent& vel = pickupEntities.get<DynamicPropertiesComponent>(entity);
			HiddenPickupComponent& pup = pickupEntities.get<HiddenPickupComponent>(entity);

			Entity newEntity = m_Scene->CreateEntity("");
	
			newEntity.AddComponent<HiddenPickupComponent>(pup);
			newEntity.AddComponent<TransformComponent>(trf);
			if (counter % 3 == 0)
			{
				newEntity.AddComponent<MeshIndexComponent>(plusIdx);
				newEntity.AddComponent<PickupComponent>(PickupType::HEALTH);
			}
			else if (counter % 3 == 1)
			{
				newEntity.AddComponent<MeshIndexComponent>(dropletIdx);
				newEntity.AddComponent<PickupComponent>(PickupType::FUEL);
			}
			else if (counter % 3 == 2)
			{
				newEntity.AddComponent<MeshIndexComponent>(missilleIdx);
				newEntity.AddComponent<PickupComponent>(PickupType::AMMO);
			}

			newEntity.AddComponent<DynamicPropertiesComponent>(vel);
			// newEntity.AddComponent<HitPointComponent>(hitPoints);
			// newEntity.AddComponent<ColliderComponent>(ColliderComponent());
			newEntity.AddComponent<MarkerComponent>(MarkerComponent(0.0f, 1.0f, 1.0f, 1.0f));

			m_Scene->m_Registry.destroy(entity);
		}
	}

}


void InGame_layer::UpdateEnemyShips(Timestep ts)
{
	if (ts == 0.0f) { return; }

	auto enemyShips = m_Scene->m_Registry.view<TransformComponent, DynamicPropertiesComponent, EnemyShipComponent>();
	for (auto ship : enemyShips)
	{
		TransformComponent& shipTrf = enemyShips.get<TransformComponent>(ship);
		DynamicPropertiesComponent& shipVel = enemyShips.get<DynamicPropertiesComponent>(ship);
		EnemyShipComponent& weaponTimer = enemyShips.get<EnemyShipComponent>(ship);
		weaponTimer.shotTimer -= ts;

		// shoot at the player
		const float weaponCooldown = 1000.0f; // unit is milli seconds
		const float weaponRange = 250.0f;
		const float shotVelocity = 0.3f;
		float ds = (shipTrf.location - m_Player.m_Transform.location).length();
		if (weaponTimer.shotTimer < 0.0f && ds < weaponRange)
		{
			float dt = ds / shotVelocity;

			Vec3D futureLoc = m_Player.m_Transform.location + dt * m_Player.m_DynamicProps.velocity;
			// Vec3D dv = Vec3D(rand() % 2000 - 1000, rand() % 2000 - 1000, rand() % 2000 - 1000) / 1000.0f;
			Vec3D bulletVel = /* shipVel.velocity + */ shotVelocity * (futureLoc - shipTrf.location) / (futureLoc - shipTrf.location).length();
			TransformComponent bulletStartLoc = shipTrf;
			bulletStartLoc.location += bulletVel * (2.0f * shipTrf.scale / bulletVel.length()); // this is definitely better than the above line, the ships doesnt blow themselves up

			// static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];
			static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["RustySlug"];
			auto dp = DynamicPropertiesComponent(); dp.velocity = bulletVel; bulletStartLoc.scale = 0.1f;
			m_EntityManager.EmitMesh(bulletIdx, bulletStartLoc, dp, 15.0f, 10000.0f);

			weaponTimer.shotTimer = weaponCooldown;
		}

		static std::vector<entt::entity> ship_vicinity;
		Box3D box; box.center = shipTrf.location; box.radius = Vec3D(weaponRange, weaponRange, weaponRange);
		ship_vicinity.clear();
		m_MissillesOctTree->GetActiveTree()->QueryRange(box, ship_vicinity, 0);

		// shoot anti missille bulletts
		int counter = 0;
		const int target_limit = 2;
		// for (int i = 0; (i < ship_vicinity.size()) && (counter < target_limit); i++)
		for (int i = ship_vicinity.size() - 1; i > 0 && (counter < target_limit); i--)
		{
			// i think it can happen that the missille hits an ateroid and gets destroyed by the time we get here
			// and if i want to get the location of a destroyed entity it will cause memory access violation
			if (!m_Scene->m_Registry.valid(ship_vicinity[i]))
				continue;

			TransformComponent& missilleTrf = m_Scene->m_Registry.get<TransformComponent>(ship_vicinity[i]);
			DynamicPropertiesComponent& missilleVel = m_Scene->m_Registry.get<DynamicPropertiesComponent>(ship_vicinity[i]);

			TransformComponent bulletStartLoc = shipTrf;
			Vec3D v = missilleVel.velocity - shipVel.velocity;
			float u = 0.2f;
			float a = 0.0001f * (2 * RORNG::runif());
			Vec3D r0 = missilleTrf.location - shipTrf.location; // once the code broke here, I dont know why, so I put in that validity check, perhaps it will solve the issue
			if (v * r0 < 0.0f)
			{
				float t0 = r0.length() / (u + v.length());
				Vec3D ri = r0;
				for (int l = 0; l < 2; l++)
				{
					ri = r0 + v * t0 * (1 + t0 * a / v.length());
					// t0 = ri.length() / u;
					t0 = ri.length() / (u + v.length());
				}
				Vec3D futureLoc = missilleTrf.location + (1.0f + (rand() % 2000 - 1000) / 5000.0f) * t0 * missilleVel.velocity * (1.0f + t0 * a / v.length());
				Vec3D dv = Vec3D(rand() % 2000 - 1000, rand() % 2000 - 1000, rand() % 2000 - 1000) / 1000.0f;
				Vec3D bulletVel = shipVel.velocity + futureLoc - shipTrf.location + dv * u * 0.1f;
				// bulletStartLoc.location += ri * (1.5f * shipTrf.scale / ri.length());
				bulletStartLoc.location += bulletVel * (1.5f * shipTrf.scale / bulletVel.length()); // this is definitely better than the above line, the ships doesnt blow themselves up
				m_EntityManager.ShootBullett(bulletStartLoc, bulletVel / bulletVel.length() * u, true);
				counter++;
			}
		}
	}
}


void InGame_layer::OnVictoryComponentDestroyed()
{
	m_Victory = true;
}

void InGame_layer::EvaluateLossCondition()
{
	if (m_EarthHitCount >= m_MaxEarthHitCount || m_Player.m_Health <= 0.0f)
		m_Loose = true;
}

