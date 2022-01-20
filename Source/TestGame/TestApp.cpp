
#include <core/Application.h>
//#include <TestGame/Layers/TestLayer.h>
//#include <TestGame/Layers/TestLayer2.h>
//#include <TestGame/Layers/TestLayer3_entities.h>
//#include <TestGame/Layers/TestLayer4_perspCamTest.h>
//#include <TestGame/Layers/TestLayer5.h>
//#include <TestGame/Layers/TestLayer6.h>
//#include <TestGame/Layers/TestLayer7.h>
#include <TestGame/Layers/TestLayer8.h>


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
//		PushLayer(new TestLayer7()); // test instance rendering
		PushLayer(new TestLayer8()); // test rebuilding
	}

	~TestApplication()
	{
	}


};



/*
Application* CreateApplication()
{
	return new TestApplication();
}
*/



