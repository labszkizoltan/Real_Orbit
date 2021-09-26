
#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <functional>

#include "core/pch/pch.h"
#include "core/ROCore.h"
#include "core/events/Event.h"

//#include <SFML/Window.hpp>
// this somehow brings in a windows header, that defines a min/max macro, which then messes up functions in Rect.inl,
// so the solution is to define NOMINMAX before that windows header, but since I dont know where that is included,
// I just added it to the whole solution (properties->C/C++ -> preprocessor -> Preprocessor Definitions)
#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/RenderWindow.hpp>

struct WindowProps
{
	std::string Title;
	uint32_t Width;
	uint32_t Height;

	WindowProps(const std::string& title = "Real_Orbit_window",
		uint32_t width = 800,
		uint32_t height = 800)
		: Title(title), Width(width), Height(height) {}
};

// Interface representing a desktop sstem based Window
class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	Window(const WindowProps& props, const sf::ContextSettings& settings);
	~Window();

	bool PollEvent(sf::Event& e); // sf::Event is at a lower level of abstraction compared to other function parameters, this is working, but not ideal
	void OnUpdate();
	
	uint32_t GetWidth();
	uint32_t GetHeight();
	sf::RenderWindow& GetNativeWindow();

	// Window attributes
	void SetEventCallback(const EventCallbackFn& callback);
	void SetVSync(bool enabled);
	bool IsVSync() const;

	static Window* Create(const WindowProps& props, const sf::ContextSettings& settings);


private:
	sf::RenderWindow m_Window; // heres a link on how to construct an SFML window + an opengl context https://www.sfml-dev.org/tutorials/2.5/window-opengl.php
	bool m_Vsynced;
	WindowProps m_Props;
	EventCallbackFn m_EventFunction;
};











#endif // WINDOW_H
