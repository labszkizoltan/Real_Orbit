
#include <core/Application.h>
#include <Game/Layers/TestLayer.h>


class TestApplication : public Application
{
public:
	TestApplication()
	{
		PushLayer(new TestLayer());
	}

	~TestApplication()
	{
	}
};




Application* CreateApplication()
{
	return new TestApplication();
}



