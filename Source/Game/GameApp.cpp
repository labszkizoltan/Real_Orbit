
#include <core/Application.h>
#include <Game/Layers/TestLayer.h>
#include <Game/Layers/TestLayer2.h>


class TestApplication : public Application
{
public:
	TestApplication()
	{
//		PushLayer(new TestLayer());
		PushLayer(new TestLayer2());
	}

	~TestApplication()
	{
	}

};




Application* CreateApplication()
{
	return new TestApplication();
}



