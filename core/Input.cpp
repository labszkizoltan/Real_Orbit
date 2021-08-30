
#include "Input.h"
#include <core/Window.h>
#include <core/Application.h>

bool Input::IsKeyPressed(sf::Keyboard::Key keycode)
{
	return sf::Keyboard::isKeyPressed(keycode);
}

bool Input::IsMouseButtonPressed(sf::Mouse::Button button)
{
	return sf::Mouse::isButtonPressed(button);
}

sf::Vector2i Input::GetGlobalMousePosition()
{
	return sf::Mouse::getPosition();
}

sf::Vector2i Input::GetMousePosition()
{
	sf::Window& window = Application::Get().GetWindow().GetNativeWindow();
	return sf::Mouse::getPosition(window);
}

int Input::GetMouseX()
{
	return GetMousePosition().x;
}

int Input::GetMouseY()
{
	return GetMousePosition().y;
}
