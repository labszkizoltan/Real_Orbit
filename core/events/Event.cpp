
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





