
#include "core/events/Event.h"

Event::Event(sf::Event e)
	: m_Event(e)
{
}

sf::Event& Event::GetEvent()
{
	return m_Event;
}

sf::Event::EventType Event::GetEventType()
{
	return m_Event.type;
}

bool Event::IsHandled()
{
	return m_Handled;
}

void Event::SetHandled(bool handlingStatus)
{
	m_Handled = handlingStatus;
}





