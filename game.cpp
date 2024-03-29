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
Vector2i monsterAnim(0, 0);
Clock monsterClock;
bool monsterDestroyed = false;





Texture aTexture;
Sprite arrowSprite;
bool arrowActive = false;
int arrowDir;
Clock arrowClock;
//sword
bool swordActive = false;


Text text;
Font font;
int score = 0;

Texture heartTexture;
Sprite heartSprite[3] = {};
int life = 3;
bool isInvincible = false;
Clock invincibleClock;


#pragma endregion




// Fonction main, point de départ du programme
int main()
{
    
    
    Start();
    



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

        // Gestion des inputs, annimation and monster(slime)
        CheckBtn();
        AnimPlayer();
        HandleMonster();
        
        // Couleur de la fenêtre en noir
        window.clear(Color::Black);

        // C'est ici que l'on dessine les éléments du jeu
        windowDraw();

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
                    if (canShowDebugCollision)
                        window.draw(rect[(i + j * COL_COUNT)]);
                }
            }
        }


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
            // Attaque à l'épée
            needResetAnim = true; // Après attaque, retourner sur anim walk
            heroAnim.x = 0; // Retourner à la col 0 (Jouer l'anim depuis le début)
            // On passe de la ligne walk à la ligne atk sur la texture
            heroAnim.y += 4; // On descend de 4 lignes sur notre sprite sheet
            // Todo: Envoyer un projectile invisible avec durée de vie très très courte**

            if (!swordActive)
            {
                swordActive = true;
                arrowSprite.setPosition(heroSprite.getPosition().x + 16, heroSprite.getPosition().y + 16);
                arrowSprite.setScale(0.75f, 0.75f);
                arrowSprite.setOrigin(16, 16);
                arrowDir = heroAnim.y;
                arrowClock.restart();
            }

        }
        if (input.GetButton().Magie == true)
        {
            needResetAnim = true;
            heroAnim.x = 0;
            heroAnim.y += 4;

            // Attaque à distance
            if (!arrowActive)
            {
                arrowActive = true;
                arrowSprite.setPosition(heroSprite.getPosition().x + 16, heroSprite.getPosition().y + 16);
                arrowSprite.setScale(0.75f, 0.75f);
                arrowSprite.setOrigin(16, 16);
                arrowDir = heroAnim.y;
                arrowClock.restart();
            }
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

// fonction de tir + déplacement + collision : projectile / felche
void HandleBullet()
{
    if (arrowActive)
    {
        // dans quel direction ?
        switch (arrowDir)
        {
        case Down_Atk:

            arrowSprite.setRotation(270);
            arrowSprite.move(0, BULLET_SPEED);
            break;

        case Up_Atk:

            arrowSprite.setRotation(90);
            arrowSprite.move(0, -BULLET_SPEED);
            break;

        case Left_Atk:

            arrowSprite.setRotation(0);
            arrowSprite.move(-BULLET_SPEED, 0);
            break;

        case Right_Atk:

            arrowSprite.setRotation(180);
            arrowSprite.move(BULLET_SPEED, 0);
            break;
        }

        // on affiche la fleche
        window.draw(arrowSprite);

        // gerer lla réinitialisation

        if (arrowClock.getElapsedTime().asSeconds() > 1.2f)
        {
            arrowActive = false;
        }

        // gerer la collision entre la felche et le monstre

        //hit box pour la fleche
        FloatRect arrowHitBox;
        arrowHitBox = arrowSprite.getGlobalBounds();

        //hit box pour le monstres
        FloatRect slimeHitBox;
        slimeHitBox = spriteSlime.getGlobalBounds();

        
        // Vérification de l'intersection entre les boîtes englobantes
        

        // on masque la felche et le monstre
        if (arrowHitBox.intersects(slimeHitBox))
        {
            arrowActive = false;
            spriteSlime.setPosition(10000, 10000);
            score++;
        }
    }
}

void HandleSword() {

    if (swordActive)
    {
        // dans quel direction ?
        switch (arrowDir)
        {
        case Down_Atk:

            arrowSprite.setRotation(270);
            arrowSprite.move(0, BULLET_SPEED);
            break;

        case Up_Atk:

            arrowSprite.setRotation(90);
            arrowSprite.move(0, -BULLET_SPEED);
            break;

        case Left_Atk:

            arrowSprite.setRotation(0);
            arrowSprite.move(-BULLET_SPEED, 0);
            break;

        case Right_Atk:

            arrowSprite.setRotation(180);
            arrowSprite.move(BULLET_SPEED, 0);
            break;
        }

        
        // gerer lla réinitialisation

        if (arrowClock.getElapsedTime().asSeconds() > 0.08f)
        {
            swordActive = false;
        }

        // gerer la collision entre la felche et le monstre

        //hit box pour la fleche
        FloatRect arrowHitBox;
        arrowHitBox = arrowSprite.getGlobalBounds();

        //hit box pour le monstres
        FloatRect slimeHitBox;
        slimeHitBox = spriteSlime.getGlobalBounds();


        // on masque la felche et le monstre
        if (arrowHitBox.intersects(slimeHitBox))
        {
            swordActive = false;
            spriteSlime.setPosition(10000, 10000);
            score++;
        }
    }
        
}

// Ajouter un point
void addPoint()
{   
    string scoreStr = to_string(score);
    text.setString(scoreStr);
}

// Dessin sur ma fenetre
void windowDraw()
{
    window.draw(mapInstance);
    window.draw(heroSprite);
    window.draw(goToMap2);
    window.draw(goToMap1);
    window.draw(spriteSlime);
    window.draw(text);

    for (int i = 0; i < life; i++)
    {
        window.draw(heartSprite[i]);
    }

    SimpleCollisions();
    HandleBullet();
    HandleSword();
    addPoint();
}


void HandleMonster()
{
    if (!monsterDestroyed)
    {
        // Déplacement du mob
        spriteSlime.move(MONSTER_SPEED, 0);
        if (spriteSlime.getPosition().x > 600 || spriteSlime.getPosition().x < 100) MONSTER_SPEED *= -1;

        // Animation du mob
        spriteSlime.setTextureRect(IntRect(monsterAnim.x * SPRITE_SIZE, monsterAnim.y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE));
        if (monsterClock.getElapsedTime().asSeconds() > 0.2f)
        {
            if (monsterAnim.x * SPRITE_SIZE >= slimeTexture.getSize().x - SPRITE_SIZE)
            {
                monsterAnim.x = 0;
            }
            else
            {
                monsterAnim.x++;
            }
            monsterClock.restart();
        }

        FloatRect heroHitBox = heroSprite.getGlobalBounds();
        FloatRect slimeHitBox = spriteSlime.getGlobalBounds();

        if (heroHitBox.intersects(slimeHitBox) && !isInvincible)
        {
            // if we are invincible we take damage
            isInvincible = true;
            life--;
            invincibleClock.restart();
        }

        if (invincibleClock.getElapsedTime().asSeconds() > 2 && isInvincible)
        {
            isInvincible = false;
        }
    }
}


void Start()
{
    // Création d'une fenêtre type RenderWindow
    window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Mon jeu SFML", Style::Default);
    window.setVerticalSyncEnabled(true);

    // Visual texture of hero
    if (!heroTexture.loadFromFile("resources/hero/hero_sheet.png"))
    {
        cout << "Erreur chargement texture héros" << endl;
    }
    // Visual texture of slime
    if (!slimeTexture.loadFromFile("resources/monster/slime.png")) {
        std::cout << "Erreur chargement texture monstre" << std::endl;
    }
    // Visual texture of arrow
    if (!aTexture.loadFromFile("resources/hero/arrow.png")) {
        std::cout << "Erreur chargement texture arc" << std::endl;
    }
    //Visual texture of font
    if (!font.loadFromFile("resources/font/poppins.ttf"))
    {
        cout << "Erreur chargement font" << endl;
    }
    // visual texture of heart
    if (!heartTexture.loadFromFile("resources/hero/heart.png"))
    {
        cout << "Erreur chargement texture coeur" << endl;
    }



    // visual definition of hero
    heroSprite.setTexture(heroTexture);
    heroSprite.setTextureRect(IntRect(heroAnim.x * SPRITE_SIZE, heroAnim.y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE)); // On découpe pour afficher 1 case de 32x32 px


    // visual definition of monster
    spriteSlime.setTexture(slimeTexture);
    spriteSlime.setTextureRect(sf::IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));
    spriteSlime.setPosition(5 * SPRITE_SIZE, 5 * SPRITE_SIZE);
    spriteSlime.setScale(0.6f, 0.6f);


    // visual definition of arrow
    arrowSprite.setTexture(aTexture);
    arrowSprite.setTextureRect(sf::IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));


    // visual definition of text
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(Color::Black);
    text.setPosition(10, 10);
    text.setStyle(Text::Bold);


    // Visual definition of the heart
    for (int i = 0; i < 3; i++)
    {
        heartSprite[i].setTexture(heartTexture);
        heartSprite[i].setColor(Color(255, 255, 255, 150));
        heartSprite[i].setPosition(5 + (i * 35), 5);
    }



    // Loading the map
    UpdateMap();

    // Portal in map 2
    goToMap2.setFillColor(Color(250, 0, 0, 100));
    goToMap2.setPosition(SPRITE_SIZE * 6, 0);

    // Portal in map 1
    goToMap1.setFillColor(Color(250, 0, 0, 100));
    goToMap1.setPosition(SPRITE_SIZE * 6, SPRITE_SIZE * 17);
}