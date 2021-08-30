
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <functional>

#include "core/pch/pch.h"
#include "core/Core.h"
#include "core/events/Event.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

struct WindowProps
{
	std::string Title;
	uint32_t Width;
	uint32_t Height;

	WindowProps(const std::string& title = "Real_Orbit_window", // std::string("Real_Orbit_window")
		uint32_t width = 1280,
		uint32_t height = 720)
		: Title(title), Width(width), Height(height) {}
};

// Interface representing a desktop sstem based Window
class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	Window(const WindowProps& props);
	~Window();

	bool PollEvent(sf::Event& e); // sf::Event is at a lower level of abstraction compared to other function parameters, this is working, but not ideal
	void OnUpdate();
	
	uint32_t GetWidth();
	uint32_t GetHeight();
	sf::Window& GetNativeWindow();

	// Window attributes
	void SetEventCallback(const EventCallbackFn& callback);
	void SetVSync(bool enabled);
	bool IsVSync() const;

//	virtual void* GetNativeWindow() const = 0;

	static Window* Create(const WindowProps& props = WindowProps());
	//		static Scope<Window> Create(const WindowProps& props = WindowProps());

private:
	sf::Window m_Window; // heres a link on how to construct an SFML window + an opengl context https://www.sfml-dev.org/tutorials/2.5/window-opengl.php
	bool m_Vsynced;
	WindowProps m_Props;
	EventCallbackFn m_EventFunction;

};




/*
// this is the window class from the Cherno engine, but this is overkill for what I want to do
// this is too much abstraction for me, and couldt make it work so far, most likely I will create
// a non abstracted window class based upon SFML

// Interface representing a desktop sstem based Window
class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	Window(const WindowProps& props);
	virtual ~Window() {}

	virtual void OnUpdate() = 0;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	// Window attributes
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	virtual void* GetNativeWindow() const = 0;

//	static Window* Create(const WindowProps& props = WindowProps());
	//		static Scope<Window> Create(const WindowProps& props = WindowProps());

};

*/





#endif // WINDOW_H
