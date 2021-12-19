
#include "TestLayer8.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>

#include <core/rendering/Renderer.h>
#include <core/rendering/drawables/ColouredMesh.h>
#include <core/rendering/drawables/Skybox.h>
#include <core/rendering/drawables/NormalMesh.h>

#include <core/scene/SceneSerializer.h>
#include <core/scene/Components.h>


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
	serializer.DeSerialize_file("D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/scenes/test_scene_3.yaml");

	m_SceneRenderer.SetScene(m_Scene);
	m_SceneUpdater.SetScene(m_Scene);



	m_FbDisplay.SetTexture(Renderer::GetColorAttachment());
//	m_FbDisplay.SetTexture(Renderer::GetBrightColorAttachment());
//	m_FbDisplay.SetTexture(Renderer::GetDepthAttachment());
//	m_FbDisplay.SetTexture(Renderer::s_DepthBuffer->GetDepthAttachment());
	


}

void TestLayer8::OnDetach()
{
	LOG_INFO("TestLayer8 detached");
}

void TestLayer8::OnUpdate(Timestep ts)
{
	HandleUserInput(ts);

	m_SceneRenderer.RenderScene();
	m_SceneUpdater.UpdateScene(m_SimulationSpeed*ts);

	m_FbDisplay.Draw();
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
}

entt::entity TestLayer8::GetTarget()
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
}

void TestLayer8::LaunchMissile(int meshIdx, TransformComponent transform, entt::entity target)
{
	DynamicPropertiesComponent dynProps;
	dynProps.inertial_mass = 1.0f;
//	dynProps.velocity = 0.01f * (transform.orientation.f1 + transform.orientation.f3);
	dynProps.velocity = 0.05f * transform.orientation.f3;
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
		static int current_slot = 0;
		LOG_WARN("Display Texture Slot was changed to: {0}", current_slot % 32);
		m_FbDisplay.SetDisplayedSlot(current_slot%32);
		current_slot++;
	}

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

	if (Input::IsKeyPressed(sf::Keyboard::Key::Add)) { cam_velocity *= 1.1f; }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Subtract)) { cam_velocity /= 1.1f; }

	static int orangeIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["OrangeSphere"];
	static int bulletIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["Bullet"];
	static int blueIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["BlueSphere"];
	static int yellowIdx = m_Scene->GetMeshLibrary().m_NameIndexLookup["YellowSphere"];
	static int skip = 0;
	if(Input::IsMouseButtonPressed(sf::Mouse::Left))
	{
//		EmitMesh(orangeIdx, cam_trf);
		if(skip%4==0)
			EmitMesh(bulletIdx, cam_trf);
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Right) && skip%2 == 0) 
	{
		LaunchMissile(blueIdx, cam_trf, GetTarget());
	}
	if (Input::IsMouseButtonPressed(sf::Mouse::Middle) && skip % 2 == 1)
	{
		LaunchMissile(yellowIdx, cam_trf, GetTarget());
	}
	skip++;

	if (Input::IsKeyPressed(sf::Keyboard::Key::Space))
	{
		TransformComponent& light_trf = m_Scene->GetLight();
		light_trf.location = cam_trf.location;
	}

	// mouse related
	if (m_InFocus)
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
