
#ifndef EVENT_H
#define EVENT_H

#include <core/Core.h>
#include <core/pch/pch.h>
#include <SFML/Window/Event.hpp>

//
//// copy pasted from SFML/Window/Event.hpp and made this an enum class
//enum class EventType
//{
//    Closed,                 ///< The window requested to be closed (no data)
//    Resized,                ///< The window was resized (data in event.size)
//    LostFocus,              ///< The window lost the focus (no data)
//    GainedFocus,            ///< The window gained the focus (no data)
//    TextEntered,            ///< A character was entered (data in event.text)
//    KeyPressed,             ///< A key was pressed (data in event.key)
//    KeyReleased,            ///< A key was released (data in event.key)
//    MouseWheelMoved,        ///< The mouse wheel was scrolled (data in event.mouseWheel) (deprecated)
//    MouseWheelScrolled,     ///< The mouse wheel was scrolled (data in event.mouseWheelScroll)
//    MouseButtonPressed,     ///< A mouse button was pressed (data in event.mouseButton)
//    MouseButtonReleased,    ///< A mouse button was released (data in event.mouseButton)
//    MouseMoved,             ///< The mouse cursor moved (data in event.mouseMove)
//    MouseEntered,           ///< The mouse cursor entered the area of the window (no data)
//    MouseLeft,              ///< The mouse cursor left the area of the window (no data)
//    JoystickButtonPressed,  ///< A joystick button was pressed (data in event.joystickButton)
//    JoystickButtonReleased, ///< A joystick button was released (data in event.joystickButton)
//    JoystickMoved,          ///< The joystick moved along an axis (data in event.joystickMove)
//    JoystickConnected,      ///< A joystick was connected (data in event.joystickConnect)
//    JoystickDisconnected,   ///< A joystick was disconnected (data in event.joystickConnect)
//    TouchBegan,             ///< A touch event began (data in event.touch)
//    TouchMoved,             ///< A touch moved (data in event.touch)
//    TouchEnded,             ///< A touch event ended (data in event.touch)
//    SensorChanged,          ///< A sensor value changed (data in event.sensor)
//
//    Count                   ///< Keep last -- the total number of event types
//};
//

class Event
{
    using EventFn = std::function<bool(Event&)>;

public:
    Event() = default;
    Event(sf::Event e);
	~Event() = default;

    sf::Event& GetEvent();
    sf::Event::EventType GetEventType();

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


/*
template <>
bool Event::Dispatch<sf::Event::EventType::Closed>(Event::EventFn func)
{
    if (EligibleForEventFnEvaluation<sf::Event::EventType::Closed>())
    {
        m_Handled = func(*this);
        return true;
    }
    return false;
}
*/




#endif // EVENT_H
