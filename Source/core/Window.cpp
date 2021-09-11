
#include "Window.h"

Window::Window(const WindowProps& props, const sf::ContextSettings& settings)
	: m_Window(sf::VideoMode(props.Width, props.Height), "OpenGL", sf::Style::Default, settings), m_Vsynced(true), m_Props(props)
{
	m_Window.setVerticalSyncEnabled(m_Vsynced);

	// activate the window
	m_Window.setActive(true);
}

Window::~Window()
{
	m_Window.close();
}

bool Window::PollEvent(sf::Event& e)
{
	return m_Window.pollEvent(e);
}

void Window::OnUpdate()
{
}

uint32_t Window::GetWidth()
{
	return m_Props.Width;
}

uint32_t Window::GetHeight()
{
	return m_Props.Height;
}

sf::RenderWindow& Window::GetNativeWindow()
{
	return m_Window;
}

void Window::SetEventCallback(const EventCallbackFn& callback)
{
	m_EventFunction = callback;
}

void Window::SetVSync(bool enabled)
{
}

bool Window::IsVSync() const
{
	return m_Vsynced;
}

// this function is called in the Application constructor
Window* Window::Create(const WindowProps& props, const sf::ContextSettings& settings)
{
	return new Window(props, settings);
}


