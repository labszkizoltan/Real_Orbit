
#ifndef INPUT_H
#define INPUT_H

#include <utility> // needed for std::pair<>
#include <SFML/Window.hpp> // needed for sf::Keyboard::Key and sf::Vector2i

class Input
{
public:
	static bool IsKeyPressed(sf::Keyboard::Key keycode);
	static bool IsMouseButtonPressed(sf::Mouse::Button button);
	static sf::Vector2i GetGlobalMousePosition();
	static sf::Vector2i GetMousePosition();
	static int GetMouseX();
	static int GetMouseY();

};







#endif // INPUT_H
