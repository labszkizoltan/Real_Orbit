
#ifndef GAME_APP_H
#define GAME_APP_H

#include <core/Application.h>
#include <core/rendering/drawables/text_rendering/ROFont.h>

enum class GameLayers { MENU_LAYER, INGAME_LAYER, INGAME_LAYER2, MARS_MISSION_LAYER };

class GameApplication : public Application
{
public:
	GameApplication();
	~GameApplication();

	void ActitivateLayer(GameLayers layer);

	//void SetVolume(float volume);
	static void Game_DrawText(const std::string& text, Vec3D screen_coord, Vec3D letter_colour, float scale);


private:
	static std::unique_ptr<ROFont> s_Font;

};

Application* CreateApplication();

#endif // GAME_APP_H
