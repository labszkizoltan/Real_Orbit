
#include <core/Application.h>
#include <Game/Layers/TestLayer.h>
#include <Game/Layers/TestLayer2.h>
#include <Game/Layers/TestLayer3_entities.h>


class TestApplication : public Application
{
public:
	TestApplication()
	{
//		PushLayer(new TestLayer());
//		PushLayer(new TestLayer2());
		PushLayer(new TestLayer3());
	}

	~TestApplication()
	{
	}

};




Application* CreateApplication()
{
	return new TestApplication();
}



