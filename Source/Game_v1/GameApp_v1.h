
#ifndef GAME_APP_H
#define GAME_APP_H

#include <core/Application.h>

enum class GameLayers { MENU_LAYER, INGAME_LAYER };

class GameApplication : public Application
{
public:
	GameApplication();
	~GameApplication();

	void ActitivateLayer(GameLayers layer);
};

Application* CreateApplication();

#endif // GAME_APP_H
