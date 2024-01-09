#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>

using namespace sf;


// class input
class Input
{
	struct Button { bool Left, Right, Up, Down, Attack, Escape; };

public:
	//Prototype de constructeur
	Input();

	//prototype get button
	Button GetButton(void) const;

	//prototpye
	void InputHandler(Event event, RenderWindow& window);

private:
	Button button;
};


#endif