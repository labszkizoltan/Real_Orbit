
#ifndef APPLICATION_H
#define APPLICATION_H

#include <core/pch/pch.h>
#include <string>

//#include <core/Core.h>
#include <core/Events/Event.h>
#include <core/Layer.h>
#include <core/LayerStack.h>
#include <core/Window.h>


class Application
{
public:
	Application(const std::string& name = "Application");
	~Application();

	void Run();
	void OnEvent();
	void OnUpdate(Timestep ts);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	inline Window& GetWindow() { return *m_Window; };

	void Close();

	static Application& Get() { return *s_Instance; };

private:
	bool OnWindowClose(Event& e);
	bool OnWindowResize(Event& e);
	bool OnLoosingFocus(Event& e);
	bool OnGainingFocus(Event& e);
	bool OnTextEntered(Event& e);
	bool OnKeyPressed(Event& e);
	bool OnKeyReleased(Event& e);
	bool MouseWheelScrolled(Event& e);
	bool OnMouseButtonPressed(Event& e);
	bool OnMouseButtonReleased(Event& e);
	bool OnMouseMoved(Event& e);
	bool OnMouseEntered(Event& e);
	bool OnMouseLeft(Event& e);


private:
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;
	bool m_Minimized = false;
	LayerStack m_LayerStack;
	float m_LastFrameTime = 0.0f;
	float m_ClearColorRed = 1.0f;

private:
	static Application* s_Instance;
};

// To be defined in CLIENT
Application* CreateApplication();


#endif // APPLICATION_H
