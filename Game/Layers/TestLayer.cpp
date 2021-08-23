
#include "TestLayer.h"

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
	LOG_INFO("TestLayer updated");
}

void TestLayer::OnEvent(Event& event)
{
	LOG_INFO("TestLayer event received");
}
