
#include "TestLayer.h"
#include <SFML/Window/Event.hpp>

#define BIND_EVENT_FN(x) std::bind(&TestLayer::x, this, std::placeholders::_1)

TestLayer::TestLayer()
	: Layer("TestLayer")
{
	LOG_INFO("TestLayer constructed");
}

void TestLayer::OnAttach()
{
	LOG_INFO("TestLayer attached");
}

void TestLayer::OnDetach()
{
	LOG_INFO("TestLayer detached");
}

void TestLayer::OnUpdate(Timestep ts)
{
//	LOG_INFO("TestLayer updated");
}

void TestLayer::OnEvent(Event& event)
{
	LOG_INFO("TestLayer event received");

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

bool TestLayer::OnLoosingFocus(Event& e)
{
	LOG_INFO("TestLayer received LostFocus event");
	return false;
}

bool TestLayer::OnGainingFocus(Event& e)
{
	LOG_INFO("TestLayer received GainFocus event");
	return false;
}

bool TestLayer::OnTextEntered(Event& e)
{
	LOG_INFO("TestLayer received OnTextEntered event");
	return false;
}

bool TestLayer::OnKeyPressed(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer received KeyPressed evet: {0}", event.key.code);
	return true; // setting the flag to handled, so the subsequent layers wont process this event when they receive it
}

bool TestLayer::OnKeyReleased(Event& e)
{
	LOG_INFO("TestLayer received OnKeyReleased event");
	return false;
}

bool TestLayer::MouseWheelScrolled(Event& e)
{
	sf::Event& event = e.GetEvent();
	LOG_INFO("TestLayer received MouseWheelScrolled evet: delta: {0}, x: {1}, y: {2}", event.mouseWheelScroll.delta, event.mouseWheelScroll.x, event.mouseWheelScroll.y);
	return false;
}

bool TestLayer::OnMouseButtonPressed(Event& e)
{
	LOG_INFO("TestLayer received OnMouseButtonPressed event");
	return false;
}

bool TestLayer::OnMouseButtonReleased(Event& e)
{
	LOG_INFO("TestLayer received OnMouseButtonReleased event");
	return false;
}

bool TestLayer::OnMouseMoved(Event& e)
{
	LOG_INFO("TestLayer received OnMouseMoved event");
	return false;
}

bool TestLayer::OnMouseEntered(Event& e)
{
	LOG_INFO("TestLayer received OnMouseEntered event");
	return false;
}

bool TestLayer::OnMouseLeft(Event& e)
{
	LOG_INFO("TestLayer received OnMouseLeft event");
	return false;
}
