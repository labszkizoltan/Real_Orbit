
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <functional>

#include "core/pch/pch.h"
#include "core/Core.h"
#include "core/events/Event.h"


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

	virtual ~Window() {}

	virtual void OnUpdate() = 0;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	// Window attributes
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	virtual void* GetNativeWindow() const = 0;

	static Window* Create(const WindowProps& props = WindowProps());
	//		static Scope<Window> Create(const WindowProps& props = WindowProps());

};




#endif // WINDOW_H
