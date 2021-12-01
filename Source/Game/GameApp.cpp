
#include <core/Application.h>
#include <Game/Layers/TestLayer.h>
#include <Game/Layers/TestLayer2.h>
#include <Game/Layers/TestLayer3_entities.h>
#include <Game/Layers/TestLayer4_perspCamTest.h>
#include <Game/Layers/TestLayer5.h>
#include <Game/Layers/TestLayer6.h>
#include <Game/Layers/TestLayer7.h>


class TestApplication : public Application
{
public:
	TestApplication()
	{
//		PushLayer(new TestLayer());
//		PushLayer(new TestLayer2());
//		PushLayer(new TestLayer3());
//		PushLayer(new TestLayer4());
//		PushLayer(new TestLayer5());
//		PushLayer(new TestLayer6()); // test scene serialization and scene rendering
		PushLayer(new TestLayer7()); // test instance rendering
	}

	~TestApplication()
	{
	}


};




Application* CreateApplication()
{
	return new TestApplication();
}



