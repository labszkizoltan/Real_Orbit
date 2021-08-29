
#include "TestLayer2.h"
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
}

void TestLayer2::OnDetach()
{
	LOG_INFO("TestLayer2 detached");
}

void TestLayer2::OnUpdate(Timestep ts)
{
	//	LOG_INFO("TestLayer2 updated");
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
