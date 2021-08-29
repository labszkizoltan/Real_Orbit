
#include "TestLayer.h"
#include <SFML/Window/Event.hpp>

TestLayer::TestLayer()
	: Layer("Sandbox2D")
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

//    sf::Event Event;
//    while (App.GetEvent(Event))
//    {
//        // Window closed
//        if (Event.Type == sf::Event::Closed)
//            App.Close();
//
//        // Escape key pressed
//        if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
//            App.Close();
//    }



}
