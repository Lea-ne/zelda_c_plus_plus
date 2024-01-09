// Inclusion des fonctionnalités
#include "main.h"

// Variables globales
#pragma region Global Variable
RenderWindow window;

// Préparation affichage personnage 2D
Texture heroTexture;
Sprite heroSprite;


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

#pragma endregion




// Fonction main, point de départ du programme
int main()
{
    // Création d'une fenêtre type RenderWindow
    window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Mon jeu SFML", Style::Default);
    window.setVerticalSyncEnabled(true);

    // Chargement du visuel du heros
    if (!heroTexture.loadFromFile("resources/sprite/hero_sheet.png"))
        cout << "Erreur chargement texture héros" << endl;
    // On applique la texture à notre Sprite
    heroSprite.setTexture(heroTexture);
    // On découpe pour afficher 1 case de 32x32 px
    heroSprite.setTextureRect(IntRect(heroAnim.x * SPRITE_SIZE, heroAnim.y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE));

    



    //cgargement map depuis un fichier
    ifstream ifs("resources/map/map1.txt");
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
    ifstream ifsCol("resources/map/map1-colision.txt");
    // permet de récupérer tout les charactère a stocké
    string contentCol((istreambuf_iterator<char>(ifsCol)), istreambuf_iterator<char>());
    cout << "map chargé collision" << endl << contentCol << endl;

    // decouple de la chaine en char
    auto explodedCol = explode(contentCol, ' ');
    // boucler sur chaque char, le convertir en nt, le stocket dans le tableau de int
    for (int i = 0; i < 450 && i < explodedCol.size(); i++)
    {
        // stoi = convertir string en int
        levelLoadedCollision[i] = stoi(explodedCol[i]);
    }



    //instance de map + chargement 
    Map map;
    if (!map.load("resources/tiles/tileset.png", Vector2u(SPRITE_SIZE, SPRITE_SIZE), levelLoaded, COL_COUNT, ROW_COUNT));






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
        window.clear(Color::Yellow);

        // C'est ici que l'on dessine les éléments du jeu
       
        window.draw(map);
        window.draw(heroSprite);
        
        

        // Dessiner à l'écran tous les éléments
        window.display();


       

    }

    // Fin du programme
    return 0;
}


void CheckBtn()
{
    if (!needResetAnim) // Si pas d'ttaque en cours
    {
        if (input.GetButton().Left == true)
        {
            heroAnim.y = Left;
            heroSprite.move(-WALK_SPEED, 0);
            heroIdle = false;
        }
        else if (input.GetButton().Right == true)
        {
            heroAnim.y = Right;
            heroSprite.move(WALK_SPEED, 0);
            heroIdle = false;
        }
        else if (input.GetButton().Up == true)
        {
            heroAnim.y = Up;
            heroSprite.move(0, -WALK_SPEED);
            heroIdle = false;
        }
        else if (input.GetButton().Down == true)
        {
            heroAnim.y = Down;
            heroSprite.move(0, WALK_SPEED);
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


