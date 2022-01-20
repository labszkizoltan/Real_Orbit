
#include <core/Application.h>
#include <Game_v1/Layers/InGame_layer.h>


class GameApplication : public Application
{
public:
	GameApplication()
	{
		PushLayer(new InGame_layer()); // test rebuilding
	}

	~GameApplication()
	{
	}


};




Application* CreateApplication()
{
	return new GameApplication();
}



