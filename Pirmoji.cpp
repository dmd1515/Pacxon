#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int P = 40; //lauko dydis
const int I = 70;

int Langas[P][I] = { 0 };
int Kubelis = 18; //Kubo dydis

struct Enemy
{
    int x, y, dx, dy;

    Enemy()
    {
        x = 350; //pozicija
        y = 200;
        dx = 4 - rand() % 4;
        dy = 4 - rand() % 4;
    }

    void move()
    {
        x += dx; if (Langas[y / Kubelis][x / Kubelis] == 1) { dx = -dx; x += dx; } // jeigu paliecia kubeli apsisuka
        y += dy; if (Langas[y / Kubelis][x / Kubelis] == 1) { dy = -dy; y += dy; }
    }
};

void drop(int y, int x) //likusius langelius pazymi -1 nuo 2
{
    if (Langas[y][x] == 0) Langas[y][x] = -1;
    if (Langas[y - 1][x] == 0) drop(y - 1, x);
    if (Langas[y + 1][x] == 0) drop(y + 1, x); 
    if (Langas[y][x - 1] == 0) drop(y, x - 1);
    if (Langas[y][x + 1] == 0) drop(y, x + 1);
}

int main()
{
    RenderWindow window(VideoMode(I * Kubelis, P * Kubelis), "Pacxon!"); //sukuria langa
    window.setFramerateLimit(70);

    srand(time(0));

    Texture t1, t2, t3, t4;
    t1.loadFromFile("Resources/tiles.png");
    t2.loadFromFile("Resources/gameover.png");
    t3.loadFromFile("Resources/Melynas.png");
    t4.loadFromFile("Resources/Pacman.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3), sTop(t4);
    sGameover.setPosition(350, 200);
    sEnemy.setOrigin(20, 20);

    const int enemyCount = 4;
    Enemy a[10];

    bool Game = true;
    int x0 = 0, y0 = 0, dx0 = 0, dy0 = 0;
    float timer = 0, delay = 0.07;
    Clock clock;

    for (int i = 0; i < P; i++)
        for (int j = 0; j < I; j++)
            if (i == 0 || j == 0 || i == P - 1 || j == I - 1)  Langas[i][j] = 1; // jeigu sonines sienos tai 1

    while (window.isOpen()) //kol langas atidarytas
    {
        float time = clock.getElapsedTime().asSeconds(); //matuoja laika sekundem
        clock.restart(); //nunulina laikrodi
        timer += time; //prideda sekundes

        Event e; //sukuriamas ivykis
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) //jeigu ivykis closed tada uzdarom langa
                window.close();

            if (e.type == Event::KeyPressed) //jeigu paspaustas mygtukas
                if (e.key.code == Keyboard::Escape)
                {
                    for (int i = 1; i < P - 1; i++) //visus langelius iskyrus sonines sienas pazymim nuliu
                        for (int j = 1; j < I - 1; j++)
                            Langas[i][j] = 0;

                    Game = true;
                }
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) { dx0 = -1; dy0 = 0; }; //gyvateles valdymas
        if (Keyboard::isKeyPressed(Keyboard::Right)) { dx0 = 1; dy0 = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Up)) { dx0 = 0; dy0 = -1; };
        if (Keyboard::isKeyPressed(Keyboard::Down)) { dx0 = 0; dy0 = 1; };

        if (!Game) continue; //jeigu suklydo

        if (timer > delay)
        {
            x0 += dx0; //pridedam gyvateles zingsni
            y0 += dy0;

            if (x0 < 0) x0 = 0; if (x0 > I - 1) x0 = I - 1;  //apsaugom nuo isejimo uz ribu
            if (y0 < 0) y0 = 0; if (y0 > P - 1) y0 = P - 1;

            if (Langas[y0][x0] == 2) Game = false; //jei uzlipam ant nebaigtos gyvateles zaidimas baigtas
            if (Langas[y0][x0] == 0) Langas[y0][x0] = 2; //kol piesiam gyvatele pazymim 2
            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++)a[i].move(); //vaiduokliu judejimas

        if (Langas[y0][x0] == 1) //jeigu paliecia violetini kuba
        {
            dx0 = dy0 = 0;

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / Kubelis, a[i].x / Kubelis); //nusiuncia vaiduokliu koordinates ir visus elementus kurie patenka i salygas pavercia -1

            for (int i = 0; i < P; i++)
                for (int j = 0; j < I; j++)
                    if (Langas[i][j] == -1) Langas[i][j] = 0; //likusius langelius pavercia atgal i 0
                    else Langas[i][j] = 1; //uzimtus langelius pavercia vienetais
        }

        for (int i = 0; i < enemyCount; i++)
            if (Langas[a[i].y / Kubelis][a[i].x / Kubelis] == 2) Game = false;

        //Teksturos
        window.clear();

        for (int i = 0; i < P; i++)
            for (int j = 0; j < I; j++)
            {
                if (Langas[i][j] == 0) continue;
                if (Langas[i][j] == 1) sTile.setTextureRect(IntRect(18, 0, Kubelis, Kubelis)); //sonines sienas nudazo pasirinkta spalva
                if (Langas[i][j] == 2) sTile.setTextureRect(IntRect(90, 0, Kubelis, Kubelis)); //gyvateles spalva
                sTile.setPosition(j * Kubelis, i * Kubelis);
                window.draw(sTile);
            }

        sTop.setPosition(x0 * Kubelis, y0 * Kubelis); //uzdeda gyvateles gale pacman
        window.draw(sTop);

        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y); //keicia pozicija pagal judejima
            window.draw(sEnemy);
        }

        if (!Game) window.draw(sGameover);

        window.display();
    }

    return 0;
}
