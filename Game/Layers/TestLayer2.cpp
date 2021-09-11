
#include "TestLayer2.h"
#include <core/Application.h>
#include <SFML/Window/Event.hpp>

#define BIND_EVENT_FN(x) std::bind(&TestLayer2::x, this, std::placeholders::_1)

TestLayer2::TestLayer2()
	: Layer("TestLayer2")
{
	LOG_INFO("TestLayer2 constructed");
}

void TestLayer2::OnAttach()
{
	LOG_INFO("TestLayer2 attached");

	// Create a graphical text to display
	if (!m_Font.loadFromFile("Game/assets/arial.ttf"))
		LOG_ERROR("TestLayer2 OnAttach: failed to load font");

	// Load a sprite to display
	sf::Texture texture;
	if (!texture.loadFromFile("Game/assets/saucer_texture.png"))
		LOG_ERROR("TestLayer2 OnAttach: failed to load texture");
	m_Sprite = sf::Sprite(texture);
}

void TestLayer2::OnDetach()
{
	LOG_INFO("TestLayer2 detached");
}

void TestLayer2::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer2 updated");
	if (Input::IsKeyPressed(sf::Keyboard::Key::Left)) { LOG_INFO("TestLayer2 OnUpdate: left arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Right)) { LOG_INFO("TestLayer2 OnUpdate: right arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Up)) { LOG_INFO("TestLayer2 OnUpdate: Up arrow is pressed"); }
	if (Input::IsKeyPressed(sf::Keyboard::Key::Down)) { LOG_INFO("TestLayer2 OnUpdate: down arrow is pressed"); }

	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Left)) { LOG_INFO("TestLayer2 OnUpdate: Left mouse button is pressed"); }
	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Right)) { LOG_INFO("TestLayer2 OnUpdate: Right mouse button is pressed"); }
	if (Input::IsMouseButtonPressed(sf::Mouse::Button::Middle))
	{
		LOG_INFO("TestLayer2 OnUpdate: Middle mouse button is pressed");
		sf::Vector2i mouse_pos = Input::GetMousePosition();
		LOG_INFO("TestLayer2 OnUpdate: window mouse position: x: {0}, y: {1}", mouse_pos.x, mouse_pos.y);
		sf::Vector2i global_mouse_pos = Input::GetGlobalMousePosition();
		LOG_INFO("TestLayer2 OnUpdate: window mouse position: x: {0}, y: {1}", global_mouse_pos.x, global_mouse_pos.y);
	}

	// try to display some text
	/*
	Window& window = Application::Get().GetWindow();
	window.GetNativeWindow().pushGLStates();
	window.GetNativeWindow().resetGLStates();
	sf::Text text("Hello SFML " + std::to_string((int)(m_ElapsedTime/1000)), m_Font, 50);
	window.GetNativeWindow().draw(text);
//	window.GetNativeWindow().draw(m_Sprite);
	window.GetNativeWindow().popGLStates();
	*/

	m_ElapsedTime += ts;
}

void TestLayer2::OnEvent(Event& event)
{
	LOG_INFO("TestLayer2 event received");

	event.Dispatch<sf::Event::EventType::LostFocus>				(BIND_EVENT_FN(OnLoosingFocus));
	event.Dispatch<sf::Event::EventType::GainedFocus>			(BIND_EVENT_FN(OnGainingFocus));
	event.Dispatch<sf::Event::EventType::KeyPressed>			(BIND_EVENT_FN(OnKeyPressed));
	event.Dispatch<sf::Event::EventType::KeyReleased>			(BIND_EVENT_FN(OnKeyReleased));
	event.Dispatch<sf::Event::EventType::MouseWheelScrolled>	(BIND_EVENT_FN(MouseWheelScrolled));
	event.Dispatch<sf::Event::EventType::MouseButtonPressed>	(BIND_EVENT_FN(OnMouseButtonPressed));
	event.Dispatch<sf::Event::EventType::MouseButtonReleased>	(BIND_EVENT_FN(OnMouseButtonReleased));
}


/***********************************
***** Private member functions *****
************************************/

bool TestLayer2::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer2 received LostFocus event");
	return false;
}

bool TestLayer2::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer2 received GainFocus event");
	return false;
}

bool TestLayer2::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer2 received OnTextEntered event");
	return false;
}

bool TestLayer2::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);
	return false;
}

bool TestLayer2::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer2 received OnKeyReleased event");
	return false;
}

bool TestLayer2::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer2 received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer2::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseButtonPressed event");
	return false;
}

bool TestLayer2::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseButtonReleased event");
	return false;
}

bool TestLayer2::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseMoved event");
	return false;
}

bool TestLayer2::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseEntered event");
	return false;
}

bool TestLayer2::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer2 received OnMouseLeft event");
	return false;
}
