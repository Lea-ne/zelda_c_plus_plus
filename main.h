// include
#include <iostream>
#include <SFML/Graphics.hpp>

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <stdlib.h>
#include "input.h"


// contstante du programme
const int WIN_WIDTH = 800; // 25 tuiles de 32 pixels
const int WIN_HEIGHT = 576; // 18 tuiles de 32 pixels
const int SPRITE_SIZE = 32; // sprite de 32px
const int WALK_SPEED = 2;


// namespace
using namespace sf;
using namespace std;

// Prototype
void CheckBtn();
void AnimPlayer();


//void CheckCollisions();