
#include "GameApp_v1.h"
#include <Game_v1/Layers/01_MenuLayer.h>
#include <Game_v1/Layers/02_InGame_layer.h>
#include <Game_v1/Layers/03_InGame_layer.h>

std::unique_ptr<ROFont> GameApplication::s_Font = nullptr;

GameApplication::GameApplication()
{
	s_Font = std::make_unique<ROFont>("assets/fonts/ImageJ_alphabet_3.png", "assets/fonts/ImageJ_alphabet_3_description.txt");

	Menu_layer* menuLayer = new Menu_layer();
	InGame_layer* ingameLayer = new InGame_layer();
	InGame_layer2* ingameLayer2 = new InGame_layer2();

	PushLayer(menuLayer);
	PushLayer(ingameLayer);
	PushLayer(ingameLayer2);

	menuLayer->Activate();
	ingameLayer->DeActivate();
	ingameLayer2->DeActivate();
}

GameApplication::~GameApplication()
{
}

void GameApplication::ActitivateLayer(GameLayers layer)
{
	m_LayerStack.ActivateLayer((int)layer);
}

void GameApplication::Game_DrawText(const std::string& text, Vec3D screen_coord, Vec3D letter_colour, float scale)
{
	s_Font->RODrawText(text, screen_coord, letter_colour, scale);
}



Application* CreateApplication()
{
	return new GameApplication();
}




