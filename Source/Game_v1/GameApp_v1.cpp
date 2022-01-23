
#include "GameApp_v1.h"
#include <Game_v1/Layers/01_MenuLayer.h>
#include <Game_v1/Layers/02_InGame_layer.h>

GameApplication::GameApplication()
{
	Menu_layer* menuLayer = new Menu_layer();
	InGame_layer* ingameLayer = new InGame_layer();

	PushLayer(menuLayer);
	PushLayer(ingameLayer); // test rebuilding

	menuLayer->Activate();
	ingameLayer->DeActivate();
}

GameApplication::~GameApplication()
{
}

void GameApplication::ActitivateLayer(GameLayers layer)
{
	m_LayerStack.ActivateLayer((int)layer);
}



Application* CreateApplication()
{
	return new GameApplication();
}




