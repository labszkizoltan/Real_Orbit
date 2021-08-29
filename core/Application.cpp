
#include "Application.h"


#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name)
{
//	{
//		if (s_Instance)
//		{
//			LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();
//		}
//	}

	if (s_Instance) { std::cout << "Application already exists!\n"; __debugbreak(); }

	s_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));

//	m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
//	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

//	Renderer::Init();
}

Application::~Application()
{
//	Renderer::Shutdown(); // this function doesnt exist in my code yet
}

void Application::Run()
{
	while (m_Running)
	{
//		auto now = std::chrono::high_resolution_clock::now();

//		float time = (float)glfwGetTime(); // Platform::GetTime() so this class doesnt depend on glfw
		float time = 0.0f; // temporarly have zero time steps
		Timestep timestep = time - m_LastFrameTime; // timestep can be initialized like this, because its constructor takes in only one float, implicit cast is possible
		m_LastFrameTime = time;

		OnEvent();

		if (!m_Minimized)
		{
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}
		}

		m_Window->OnUpdate();
	}
}

void Application::OnEvent()
{
	Event e;
	while (m_Window->PollEvent(e.GetEvent()))
	{
//		e.Dispatch<sf::Event::EventType::Closed>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		e.Dispatch<sf::Event::EventType::Closed>				(BIND_EVENT_FN(OnWindowClose));
		e.Dispatch<sf::Event::EventType::Resized>				(BIND_EVENT_FN(OnWindowResize));
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
	}



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

	//	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	//	{
	//		m_Minimized = true;
	//		return false;
	//	}
	//
	//	m_Minimized = false;
	//	Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

	return true;
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
	LOG_CORE_INFO("Key pressed");
	return false;
}

bool Application::OnKeyReleased(Event& e)
{
	LOG_CORE_INFO("Key released");
	return false;
}

bool Application::MouseWheelScrolled(Event& e)
{
	LOG_CORE_INFO("Mouse scrolled");
	return false;
}

bool Application::OnMouseButtonPressed(Event& e)
{
	LOG_CORE_INFO("Mouse pressed");
	return false;
}

bool Application::OnMouseButtonReleased(Event& e)
{
	LOG_CORE_INFO("Mouse released");
	return false;
}

bool Application::OnMouseMoved(Event& e)
{
	LOG_CORE_INFO("Mouse moved");
	return false;
}

bool Application::OnMouseEntered(Event& e)
{
	LOG_CORE_INFO("Mouse entered");
	return false;
}

bool Application::OnMouseLeft(Event& e)
{
	LOG_CORE_INFO("Mouse left");
	return false;
}


//#################################################################
//#################################################################
//#################################################################
