#include "input.h"

// constructeur

Input::Input()
{
	button.Left = button.Right = button.Up = button.Down = button.Attack = button.Escape = button.Space = false;

}


// fonction qui accede a button (struct) et nous donne l'état du bouton
Input::Button Input::GetButton(void) const
{
    return button;
}


// fonction input
void Input::InputHandler(Event event, RenderWindow& window)
{
    //fermer la fenetre si on clique sur la croix
    if (event.type == Event::Closed)
    {
        window.close();
    }


    // Keyboard input press
    if (event.type == Event::KeyPressed)
    {
        switch (event.key.code) // code de la touche utilisée
        {
        case Keyboard::Left:
            button.Left = true;
            break;
        case Keyboard::Right:
            button.Right = true;
            break;
        case Keyboard::Down:
            button.Down = true;
            break;
        case Keyboard::Up:
            button.Up = true;
            break;
        case Keyboard::Escape:
            button.Escape = true;
            break;
        case Keyboard::Space:
            button.Space = true;
            break;

        default:
            break;
        }
    }


    // key release
    if (event.type == Event::KeyReleased)
    {
        switch (event.key.code) // code de la touche utilisée
        {
        case Keyboard::Left:
            button.Left = false;
            break;
        case Keyboard::Right:
            button.Right = false;
            break;
        case Keyboard::Down:
            button.Down = false;
            break;
        case Keyboard::Up:
            button.Up = false;
            break;
        case Keyboard::Escape:
            button.Escape = false;
            break;
        case Keyboard::Space:
            button.Space = false;
            break;

        default:
            break;
        }
    }


    // for the mouse input
    if (event.type == Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == Mouse::Left)
        {
            button.Attack = true;
        }
    }
    // mouse release
    if (event.type == Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == Mouse::Left)
        {
            button.Attack = false;
        }
    }
}

