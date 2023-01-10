#pragma once
extern struct Enemy
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