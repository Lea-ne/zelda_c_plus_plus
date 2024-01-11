// Inclusion des fonctionnalités
#include "main.h"

// Variables globales
#pragma region Global Variable
RenderWindow window;

// Préparation affichage personnage 2D
Texture heroTexture;


// enum des actions / directions
Input input;
enum Dir { Down, Right, Up, Left, Down_Atk, Right_Atk, Up_Atk, Left_Atk };

// Définir l'animation (la ligne) à jouer
Vector2i heroAnim(0, Down); 
Clock heroAnimClock; // Timer / clock
bool heroIdle = true; // bool le perso est il à l'arrêt ?
bool needResetAnim = false; // Gestion de l'attaque à l'épée


//map
int levelLoaded[450]; // tableau représentant la map -> 450 = 25 * 18 


// collision de la map
int levelLoadedCollision[450]; //array for the collision
// pour debug collision
RectangleShape rect[450];

bool canShowDebugCollision = false;


Texture slimeTexture;
Sprite spriteSlime;

#pragma endregion




// Fonction main, point de départ du programme
int main()
{
    // Création d'une fenêtre type RenderWindow
    window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Mon jeu SFML", Style::Default);
    window.setVerticalSyncEnabled(true);

    // Chargement du visuel du heros
    if (!heroTexture.loadFromFile("resources/hero/hero_sheet.png"))
        cout << "Erreur chargement texture héros" << endl;
    // On applique la texture à notre Sprite
    heroSprite.setTexture(heroTexture);
    // On découpe pour afficher 1 case de 32x32 px
    heroSprite.setTextureRect(IntRect(heroAnim.x * SPRITE_SIZE, heroAnim.y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE));

    
    //monstree
    if (!slimeTexture.loadFromFile("resources/monster/slime.png")) {
        std::cout << "Erreur chargement texture monstre" << std::endl;
    }

    spriteSlime.setTexture(slimeTexture);
    spriteSlime.setTextureRect(sf::IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));
    spriteSlime.setPosition(5 * SPRITE_SIZE, 5 * SPRITE_SIZE);
    spriteSlime.setScale(0.6f, 0.6f);





    // Chargement map
    UpdateMap();

    // Portail vers map 2
    goToMap2.setFillColor(Color(250, 0, 0, 100));
    goToMap2.setPosition(SPRITE_SIZE * 6, 0);
    // Portail vers map 1
    goToMap1.setFillColor(Color(250, 0, 0, 100));
    goToMap1.setPosition(SPRITE_SIZE * 6, SPRITE_SIZE * 17);
    

    



    // Update
   while (window.isOpen())
    {
        Event event; 
        // On boucle sur les événements
        while (window.pollEvent(event))
        {
            // Gestion des inputs / events
            input.InputHandler(event, window);
        }
        // Gestion des inputs
        CheckBtn();
        // Animation du perso
        AnimPlayer();
        
        // Couleur de la fenêtre en noir
        window.clear(Color::Black);

        // C'est ici que l'on dessine les éléments du jeu
        window.draw(mapInstance);
        window.draw(heroSprite);
        window.draw(goToMap2);
        window.draw(goToMap1);
        window.draw(spriteSlime);


        // Pour débug visuellement les collisions de la map
        for (unsigned int j = 0; j < ROW_COUNT; ++j)
        { // On boucle sur chaque ligne et col
            for (unsigned int i = 0; i < COL_COUNT; ++i)
            {
                if (levelLoadedCollision[(i + j * COL_COUNT)] == 1)
                {
                    Vector2f pos = Vector2f(i * SPRITE_SIZE, j * SPRITE_SIZE);
                    rect[(i + j * COL_COUNT)].setPosition(pos);
                    rect[(i + j * COL_COUNT)].setSize(Vector2f(SPRITE_SIZE, SPRITE_SIZE));
                    rect[(i + j * COL_COUNT)].setFillColor(Color(250, 0, 0, 100));
                    if(canShowDebugCollision)
                        window.draw(rect[(i + j * COL_COUNT)]);
                }
            }
        }
        // Fin débug collisions

        // Simple collisions
        SimpleCollisions();

        // Dessiner à l'écran tous les éléments
        window.display();

    }

    // Fin du programme
   return 0;
}


void CheckBtn()
{

    int hPosX = round(heroSprite.getPosition().x / SPRITE_SIZE);
    int hPosY = round(heroSprite.getPosition().y / SPRITE_SIZE);
    
    if (canShowDebugCollision)
    {
        cout << levelLoaded[(hPosX + hPosY * COL_COUNT)] << endl;
        cout << hPosX << " , " << hPosY << endl;
    }


    if (!needResetAnim) // Si pas d'ttaque en cours
    {
        if (input.GetButton().Left == true)
        {
            heroAnim.y = Left;

            //verification si a notre gauche on a une case solide ou pas
            hPosX = round((heroSprite.getPosition().x - (WALK_SPEED) * 3) / SPRITE_SIZE);
            if (levelLoadedCollision[hPosX + hPosY * COL_COUNT] == 0 && hPosY > -1)
            {
                // on se déplacce vers la gauche
                heroSprite.move(-WALK_SPEED, 0);
            }
            heroIdle = false;
        }


        else if (input.GetButton().Right == true)
        {
            heroAnim.y = Right;

            hPosX = round((heroSprite.getPosition().x + (WALK_SPEED) * 3) / SPRITE_SIZE);
            if (levelLoadedCollision[hPosX + hPosY * COL_COUNT] == 0 && hPosY < COL_COUNT)
            {
                // on se déplacce vers la droite
                heroSprite.move(WALK_SPEED, 0);
            }
            
            heroIdle = false;
        }


        else if (input.GetButton().Up == true)
        {
            heroAnim.y = Up;
            hPosY = round((heroSprite.getPosition().y - (WALK_SPEED) * 2.5) / SPRITE_SIZE);
            if (levelLoadedCollision[hPosX + hPosY * COL_COUNT] == 0 && hPosY > -1)
            {
                // on se déplacce vers la droite
                heroSprite.move(0, -WALK_SPEED);
            }
            
            heroIdle = false;
        }


        else if (input.GetButton().Down == true)
        {
            heroAnim.y = Down;
            hPosY = round((heroSprite.getPosition().y + (WALK_SPEED) * 4) / SPRITE_SIZE);
            if (levelLoadedCollision[hPosX + hPosY * COL_COUNT] == 0 && hPosY < ROW_COUNT)
            {
                // on se déplacce vers la droite
                heroSprite.move(0, WALK_SPEED);
            }
            
            heroIdle = false;
        }


        else // Perso en IDLE
        {
            heroIdle = true;
        }
        if (input.GetButton().Attack == true)
        {
            needResetAnim = true; // Après attaque, retourner sur anim walk
            heroAnim.x = 0; // Retourner à la col 0 (Jouer l'anim depuis le début)
            // On passe de la ligne walk à la ligne atk sur la texture
            heroAnim.y += 4; // On descend de 4 lignes sur notre sprite sheet
        }
    }

    if (input.GetButton().Escape == true)
    {
        window.close();
    }

    canShowDebugCollision = input.GetButton().Space;

}

void AnimPlayer()
{
    // Modifie le Sprite à l'écran
    heroSprite.setTextureRect(IntRect(heroAnim.x * SPRITE_SIZE, heroAnim.y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE));
    // Animer en boucle la col (0 -> 3)
    // Si il s'est écoupé 0.1 seconde (1/10 de seconde)
    if (heroAnimClock.getElapsedTime().asSeconds() > 0.1f)
    { // Anim de 10 FPS
        // On est au bout de la texture (en largeur)
        if (heroAnim.x * SPRITE_SIZE >= heroTexture.getSize().x - SPRITE_SIZE)
        {
            heroAnim.x = 0;
            // Si l'anim d'atk tourne
            if (needResetAnim)
            {
                // On l'arrête & on retourne sur l'anim de marche
                needResetAnim = false;
                heroAnim.y -= 4;
            }
        }
        // On se décale de 1 col
        else
        {
            // Si le personnage n'est pas à l'arrêt = !idle
            if (!heroIdle || needResetAnim)
                heroAnim.x++;
        }
        heroAnimClock.restart();
    }
}


vector<string> explode(string const& s, char delim)

{
    vector<string> result;
    istringstream iss(s);

    for (string token; getline(iss, token, delim); )
    {
        result.push_back(move(token));
    }
    return result;
}


void UpdateMap() {


    //cgargement map depuis un fichier
    ifstream ifs("resources/map/map" + to_string(actualMap) + ".txt");
    // permet de récupérer tout les charactère a stocké
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    cout << "map chargé" << endl << content << endl;

    // decouple de la chaine en char
    auto exploded = explode(content, ' ');
    // boucler sur chaque char, le convertir en nt, le stocket dans le tableau de int
    for (int i = 0; i < 450 && i < exploded.size(); i++)
    {
        // stoi = convertir string en int
        levelLoaded[i] = stoi(exploded[i]);
    }




    //chargement des collisoin de la map depuis le fichier collision
    ifstream ifsCol("resources/map/map" + to_string(actualMap) + "-colision.txt");
    // permet de récupérer tout les charactère a stocké
    string contentCol((istreambuf_iterator<char>(ifsCol)), istreambuf_iterator<char>());
    cout << "map charge collision" << endl << contentCol << endl;

    // decouple de la chaine en char
    auto explodedCol = explode(contentCol, ' ');
    // boucler sur chaque char, le convertir en nt, le stocket dans le tableau de int
    for (int i = 0; i < 450 && i < explodedCol.size(); i++)
    {
        // stoi = convertir string en int
        levelLoadedCollision[i] = stoi(explodedCol[i]);
    }



    //instance de map + chargement 
    if (!mapInstance.load("resources/tiles/tileset.png", Vector2u(SPRITE_SIZE, SPRITE_SIZE), levelLoaded, COL_COUNT, ROW_COUNT));

}