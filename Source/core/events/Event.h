
#ifndef EVENT_H
#define EVENT_H

#include <core/ROCore.h>
#include <core/pch/pch.h>
#include <SFML/Window/Event.hpp>


class Event
{
    using EventFn = std::function<bool(Event&)>;

public:
    Event() = default;
    Event(sf::Event e);
	~Event() = default;

    sf::Event& GetEvent();
    sf::Event::EventType GetEventType();
    bool IsHandled();
    void SetHandled(bool handlingStatus);

    template <sf::Event::EventType T>
    bool Dispatch(EventFn func)
    {
        if (EligibleForEventFnEvaluation<T>())
        {
            m_Handled = func(*this);
            return true;
        }
        return false;
    }

private:
    template <sf::Event::EventType T>
    bool EligibleForEventFnEvaluation()
    {
        return (!m_Handled && (GetEventType() == T));
    }

private:
	sf::Event m_Event;
    bool m_Handled = false;
};


#endif // EVENT_H
