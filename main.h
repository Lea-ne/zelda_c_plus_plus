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
#include "map.h"


// contstante du programme
const int WIN_WIDTH = 800; // 25 tuiles de 32 pixels
const int WIN_HEIGHT = 576; // 18 tuiles de 32 pixels
const int SPRITE_SIZE = 32; // sprite de 32px
const int WALK_SPEED = 2;
const int COL_COUNT = 25; // nombre de case en largeur
const int ROW_COUNT = 18;// nombre de case en ligne

// namespace
using namespace sf;
using namespace std;

// Prototype
void CheckBtn();
void AnimPlayer();


//void CheckCollisions();