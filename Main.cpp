#include <SFML/Graphics.hpp>
#include <time.h>
#include "kintamieji.h"
#include "Funkcija.h"
#include <vector>
#include <string>
#include "SFML\Audio.hpp"


using namespace sf;

class Entity
{
protected:
    sf::Texture t;
    sf::Sprite s;

public:
    Entity(const std::string& textureFilename)
    {
        t.loadFromFile("Resources/" + textureFilename);
        s.setTexture(t);
        s.setPosition(350, 200);
        s.setScale(0.5, 0.5);
    }

    ~Entity() {}

    sf::FloatRect getBounds()
    {
        return s.getGlobalBounds();
    }

    void draw(sf::RenderWindow* window)
    {
        window->draw(s);
    }
};

class Bananas : public Entity
{
private:
    sf::Vector2f initialPos;
    bool active=true;

public:
    Bananas(std::string textureFilename) : Entity(textureFilename) {
        respawn();
    }

    ~Bananas() {}

    bool isActive() const {
        return active;
    }

    void updatePosition(float x, float y) {
        s.setPosition(initialPos.x + x, initialPos.y + y);
    }

    bool checkCollisionWith(FloatRect collision) {
        return s.getGlobalBounds().intersects(collision);
    }

    void respawn() {
        s.setPosition(350, 200);
        /*int x = rand() % (I - 4) + 2; // Adjusted range to avoid edges
        int y = rand() % (P - 4) + 2; // Adjusted range to avoid edges
        initialPos = sf::Vector2f(x * Kubelis, y * Kubelis);
        updatePosition(0, 0);
        active = true;*/
    }
};
class Judejimas 
{
    private:
    int dx0 = 0, dy0 = 0, rotation = 0;
    Sprite* sTop;

    public:
    Judejimas(Sprite* sTop) : sTop(sTop) {}

    void update() 
    {
        if (Keyboard::isKeyPressed(Keyboard::Left)) 
        {
            dx0 = -1;
            dy0 = 0;
            sTop->setRotation(ROTATION_LEFT);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right)) 
        {
            dx0 = 1;
            dy0 = 0;
            sTop->setRotation(ROTATION_RIGHT);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up)) 
        {
            dx0 = 0;
            dy0 = -1;
            sTop->setRotation(ROTATION_UP);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down)) 
        {
            dx0 = 0;
            dy0 = 1;
            sTop->setRotation(ROTATION_DOWN);
        }
    }

    int getDx0() const 
    { 
        return dx0; 
    }
    int getDy0() const 
    { 
        return dy0; 
    }
};

void drop(const int& y, int x) //likusius langelius pazymi -1 nuo 2
{
    if (Langas[y][x] == 0)
    {
        Langas[y][x] = -1;
    }
    if (Langas[y - 1][x] == 0)
    {
        drop(y - 1, x);
    }
    if (Langas[y + 1][x] == 0)
    {
        drop(y + 1, x);
    }
    if (Langas[y][x - 1] == 0)
    {
        drop(y, x - 1);
    }
    if (Langas[y][x + 1] == 0)
    {
        drop(y, x + 1);
    }
}

bool tiesa(int a)
{
    if (a == 1)
    {
        return true;
    }
}

int main()
{
    RenderWindow window(VideoMode(I * Kubelis, P * Kubelis), "Pacxon!"); //sukuria langa
    window.setFramerateLimit(200);
    SoundSystem::Instance().playAmbiance();
    srand(time(0));

    Texture t1, t2, t3, t4;
    t1.loadFromFile("Resources/tiles.png");
    t2.loadFromFile("Resources/gameover.png");
    t3.loadFromFile("Resources/Melynas.png");
    t4.loadFromFile("Resources/Pacman.png");
    Bananas* banana= new Bananas("banana.png");
    
        banana->draw(&window);
    

    Sprite sTile(t1), sGameover(t2), sEnemy(t3), sTop(t4);
    Judejimas input(&sTop);
    sGameover.setPosition(350, 200);
    sEnemy.setOrigin(20, 20);
    //sBanana.setPosition(350, 200);
    //sBanana.setOrigin(sBanana.getLocalBounds().width / 2, sBanana.getLocalBounds().height / 2);
    //sBanana.setScale(0.05, 0.05);
    //window.draw(sBanana);

    const int enemyCount = 4;
    std::vector <Enemy> a(10);
 
    bool Game = true;
    int x0 = 0, y0 = 0, dx0 = 0, dy0 = 0;
    float timer = 0, delay = 0.07;
    Clock clock;

    for (int i = 0; i < P; i++)
    {
        for (int j = 0; j < I; j++)
        {
            if (i == 0 || j == 0 || i == P - 1 || j == I - 1)
            {
                Langas[i][j] = 1; // jeigu sonines sienos tai 1
            }
        }
    }
       
    while (window.isOpen()) //kol langas atidarytas
    {
        bool powerDown = false;
        input.update();
        dx0 = input.getDx0();
        dy0 = input.getDy0();
        float time = clock.getElapsedTime().asSeconds(); //matuoja laika sekundem
        clock.restart(); //nunulina laikrodi
        timer += time; //prideda sekundes

        /*if (banana->isActive()) {
            banana->draw(&window);
        }*/

        Event e; //sukuriamas ivykis
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                //jeigu ivykis closed tada uzdarom langa
                window.close();
            }

            if (e.type == Event::KeyPressed) //jeigu paspaustas mygtukas

                if (e.key.code == Keyboard::Escape)
                {    
                    SoundSystem::Instance().stopGM();
                    x0 = 0;
                    y0 = 0;
                    dx0 = 0;
                    dy0 = 0;
                    timer = 0;
                    delay = 0.07;
                    Game = true;
                    for (int i = 0; i < P; i++) 
                    {
                        for (int j = 0; j < I; j++) 
                        {
                            if (i == 0 || j == 0 || i == P - 1 || j == I - 1) 
                            {
                                Langas[i][j] = 1;
                            }
                            else 
                            {
                                Langas[i][j] = 0;
                            }
                        }
                    }
                    SoundSystem::Instance().playAmbiance();

                }
            if (!Game)
            {
                SoundSystem::Instance().stopAmbiance();
                SoundSystem::Instance().playGM();
                window.draw(sGameover);
            }
        }

        if (!Game)
        {
            continue; //jeigu suklydo
        }

        if (timer > delay)
        {
            /* if (banana->checkCollisionWith(sTop.getGlobalBounds()))
            {
                // Handle collision with banana
                // e.g., increase score, slow down enemies, etc.
                banana->respawn();
                for (int i = 0; i < enemyCount; i++)
                {
                    a[i].powerDown();
                }
            }*/
            x0 += dx0; //pridedam gyvateles zingsni
            y0 += dy0;

            if (x0 < 0)
            {
                x0 = 0;
            }
            if (x0 > I - 1)
            {
                x0 = I - 1;
            }
            if (y0 < 0)
            {
                y0 = 0;
            }
            if (y0 > P - 1)
            {
                y0 = P - 1;  //apsaugom nuo isejimo uz ribu
            }

            if (Langas[y0][x0] == 2)
            {
                Game = false; //jei uzlipam ant nebaigtos gyvateles zaidimas baigtas
            }
            if (Langas[y0][x0] == 0)
            {
                Langas[y0][x0] = 2; //kol piesiam gyvatele pazymim 2
            }
            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++)
        {
            a[i].move(); //vaiduokliu judejimas
        }

        if (tiesa(Langas[y0][x0]) == true) //jeigu paliecia violetini kuba
        {
            dx0 = dy0 = 0;
            for (int i = 0; i < enemyCount; i++)
            {
                drop(a[i].y / Kubelis, a[i].x / Kubelis); //nusiuncia vaiduokliu koordinates ir visus elementus kurie patenka i salygas pavercia -1
            }
            for (int i = 0; i < P; i++)
            {
                for (int j = 0; j < I; j++)
                {
                    if (Langas[i][j] == -1)
                    {
                        Langas[i][j] = 0; //likusius langelius pavercia atgal i 0
                    }
                    else
                    {
                        Langas[i][j] = 1; //uzimtus langelius pavercia vienetais
                    }
                }             
            } 
        }
        for (int i = 0; i < enemyCount; i++)
        {
            if (Langas[a[i].y / Kubelis][a[i].x / Kubelis] == 2) Game = false;
        }
        /*if (banana->isActive()) {
            banana->draw(&window);
        }*/
        //Teksturos
        window.clear();

        for (int i = 0; i < P; i++)
        {
            for (int j = 0; j < I; j++)
            {
                if (Langas[i][j] == 0)
                {
                    continue;
                }
                if (Langas[i][j] == 1)
                {
                    sTile.setTextureRect(IntRect(18, 0, Kubelis, Kubelis)); //sonines sienas nudazo pasirinkta spalva
                }
                if (Langas[i][j] == 2)
                {
                    sTile.setTextureRect(IntRect(90, 0, Kubelis, Kubelis)); //gyvateles spalva
                }
                sTile.setPosition(j * Kubelis, i * Kubelis);
                window.draw(sTile);
            }
        }
            
        //uzdeda gyvateles gale pacman
        sTop.setOrigin(sTop.getLocalBounds().width / 2, sTop.getLocalBounds().height / 2);
        sTop.setPosition((x0 + 0.5f) * Kubelis, (y0 + 0.5f) * Kubelis);
        sTop.setScale(1.5, 1.5);
        window.draw(sTop);

        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y); //keicia pozicija pagal judejima
            window.draw(sEnemy);
        }

        if (!Game)
        {
            SoundSystem::Instance().stopAmbiance();
            SoundSystem::Instance().playGM();
            window.draw(sGameover);
        }
        /*if (banana->isActive()) {
            banana->draw(&window);
        }*/
        window.display();
    }
    return 0;
}
