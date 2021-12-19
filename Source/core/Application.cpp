
#include "Application.h"
#include <core/rendering/Renderer.h>
#include <core/OpenGLContext.h>
#include <SFML/System/Time.hpp>

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name)
{
	if (s_Instance) { std::cout << "Application already exists!\n"; __debugbreak(); }

	s_Instance = this;

	WindowProps properties;
	properties.Title = name;
	properties.Width = 1200;
	properties.Height = 800;
	properties.Style = sf::Style::Default;
	properties.ShowMouse = true;
//	properties.Style = sf::Style::Fullscreen;
//	properties.ShowMouse = false;

	// need to create an OpenGL context without a window to query OpenGL version, then destroy the context
	int versionMajor = 4, versionMinor = 6;
	OpenGLContext::GetMinorMajorVersion(versionMinor, versionMajor);

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = versionMajor;
	settings.minorVersion = versionMinor;

//	m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
	m_Window = std::unique_ptr<Window>(Window::Create(properties, settings));

	LOG_CORE_INFO("SFML window created with OpenGL version Major / Minor: {0} / {1}", settings.majorVersion, settings.minorVersion);

	int renderInitStatus = Renderer::Init();
	LOG_CORE_INFO("Renderer Initialization status: {0}", renderInitStatus);
}

Application::~Application()
{
//	Renderer::Shutdown(); // this function doesnt exist in my code yet
}

void Application::Run()
{
	sf::Clock clock;
	sf::Time loopStart = clock.getElapsedTime();
	sf::Time loopFinish = clock.getElapsedTime();
	sf::Time ts = loopFinish - loopStart;
	Timestep timestep = 1000.0f * (float)ts.asSeconds(); // convert to mili seconds

	while (m_Running)
	{
		ts = loopFinish - loopStart;
		timestep = 1000.0f * (float)ts.asSeconds();
		
		loopStart = clock.getElapsedTime();

		OnEvent();
		OnUpdate(timestep);

//		m_Window->GetNativeWindow().display();
//		m_Window->GetNativeWindow().display(); // why do I need two??? probably adding the second one solves a buffer swapping issue under the hood, that caused flickering

		loopFinish = clock.getElapsedTime();
	}
}

void Application::OnEvent()
{
	Event e;
	while (m_Window->PollEvent(e.GetEvent()))
	{
		e.Dispatch<sf::Event::EventType::Closed>				(BIND_EVENT_FN(OnWindowClose));
		e.Dispatch<sf::Event::EventType::Resized>				(BIND_EVENT_FN(OnWindowResize));

//		these dispatches are only here for debugging, at application level only a few of them should be present
/*
		e.Dispatch<sf::Event::EventType::LostFocus>				(BIND_EVENT_FN(OnLoosingFocus));
		e.Dispatch<sf::Event::EventType::GainedFocus>			(BIND_EVENT_FN(OnGainingFocus));
		e.Dispatch<sf::Event::EventType::TextEntered>			(BIND_EVENT_FN(OnTextEntered));
		e.Dispatch<sf::Event::EventType::KeyPressed>			(BIND_EVENT_FN(OnKeyPressed));
		e.Dispatch<sf::Event::EventType::KeyReleased>			(BIND_EVENT_FN(OnKeyReleased));
		e.Dispatch<sf::Event::EventType::MouseWheelScrolled>	(BIND_EVENT_FN(MouseWheelScrolled));
		e.Dispatch<sf::Event::EventType::MouseButtonPressed>	(BIND_EVENT_FN(OnMouseButtonPressed));
		e.Dispatch<sf::Event::EventType::MouseButtonReleased>	(BIND_EVENT_FN(OnMouseButtonReleased));
		e.Dispatch<sf::Event::EventType::MouseMoved>			(BIND_EVENT_FN(OnMouseMoved));
		e.Dispatch<sf::Event::EventType::MouseEntered>			(BIND_EVENT_FN(OnMouseEntered));
		e.Dispatch<sf::Event::EventType::MouseLeft>				(BIND_EVENT_FN(OnMouseLeft));
*/

		if (!m_Minimized)
			m_LayerStack.OnEvent(e);
	}
}

void Application::OnUpdate(Timestep ts)
{
	if (!m_Minimized)
		m_LayerStack.OnUpdate(ts);

	m_Window->OnUpdate();

	m_Window->GetNativeWindow().display();
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PushOverlay(overlay);
	overlay->OnAttach();
}

void Application::Close()
{
	m_Running = false;
}

bool Application::OnWindowClose(Event& e)
{
	LOG_CORE_INFO("Window Close event captured");
	m_Running = false;
	return true;
}

bool Application::OnWindowResize(Event& e)
{
	sf::Event& event = e.GetEvent();

	LOG_CORE_INFO("Window Resize event captured: width - {0}, height - {1}", event.size.width, event.size.height);

	// adjust the viewport when the window is resized
	glViewport(0, 0, event.size.width, event.size.height);
	Renderer::SetAspectRatio((float)event.size.width / (float)event.size.height);

	// Temporarly setting this to false, otherwise this event cant propagate to TestLayer2 and the aspect ratio cannot be changed upon window resizing
//	return true;
	return false;
}

bool Application::OnLoosingFocus(Event& e)
{
	LOG_CORE_INFO("Window lost focus");
	return true;
}

bool Application::OnGainingFocus(Event& e)
{
	LOG_CORE_INFO("Window gained focus");
	return false;
}

bool Application::OnTextEntered(Event& e)
{
	LOG_CORE_INFO("Text entered");
	return false;
}

bool Application::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Key pressed {0}", event.key.code);
	return false;
}

bool Application::OnKeyReleased(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Key released {0}", event.key.code);
	return false;
}

bool Application::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Mouse scrolled. Delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);

	return false;
}

bool Application::OnMouseButtonPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Mouse pressed");
	return false;
}

bool Application::OnMouseButtonReleased(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Mouse released");
	return false;
}

bool Application::OnMouseMoved(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Mouse moved");
	return false;
}

bool Application::OnMouseEntered(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Mouse entered");
	return false;
}

bool Application::OnMouseLeft(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_CORE_INFO("Mouse left");
	return false;
}


//#################################################################
//#################################################################
//#################################################################
