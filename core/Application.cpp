
#include "Application.h"


#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name)
{
	if (s_Instance) { std::cout << "Application already exists!\n"; __debugbreak(); }

	s_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));

//	m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

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

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

	// DY_CORE_TRACE("{0}", e);

	//for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
	for (std::vector<Layer*>::iterator it = m_LayerStack.end(); it != m_LayerStack.begin();)
	{
		(*--it)->OnEvent(e);
		if (e.Handled)
			break;
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

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
//	Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

	return false;
}

//#################################################################
//#################################################################
//#################################################################












