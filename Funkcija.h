#pragma once
#include "SFML\Audio.hpp"



class SoundSystem
{
    public:
    static SoundSystem& Instance() 
    {
        static SoundSystem theSS;
        return theSS;
    }
    void playAmbiance() 
    { 
        ambiance.play(); 
    }
    void stopAmbiance() 
    { 
        ambiance.stop(); 
    }
    void playGM() 
    { 
        GM.play(); 
    }
    void pauseGM() 
    { 
        GM.pause(); 
    }
    void stopGM() 
    { 
        GM.stop(); 
    }

    private:
    SoundSystem()
    {
        ambiance.openFromFile("Pacxon.wav");
        ambiance.setLoop(true);
        GM.openFromFile("gameover_out.wav");
    }
    SoundSystem(SoundSystem const&);
    SoundSystem& operator= (SoundSystem const&) {}
    ~SoundSystem() {}

    sf::Music ambiance;
    sf::Music GM;
};
extern struct Enemy
{
    int x, y, dx, dy;

    Enemy() //konstruktorius
    {
        x = 350; //pozicija
        y = 200;
        dx = 4 - rand() % 4;
        dy = 4 - rand() % 4;
    }
    ~Enemy() {}

    void move()
    {
        x += dx; 
        if (Langas[y / Kubelis][x / Kubelis] == 1) 
        { 
            dx = -dx; 
            x += dx; 
        } // jeigu paliecia kubeli apsisuka
        y += dy; 
        if (Langas[y / Kubelis][x / Kubelis] == 1) 
        { 
            dy = -dy; 
            y += dy; 
        }
    }
    void powerDown()
    {
        dx *= 0.5;
        dy *= 0.5;
    }
};