#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

//#pragma once

#include <SFML\Graphics.hpp>
#include <Windows.h>
#include <string>
#include "Mapa.h"
#include "Audio.h"

using namespace std;
using namespace sf;

class Game{
public:
    Game(void);
    ~Game(void);

    void Initialize();
    void RunGame(); //glowna petla-wlacz menu
    void OpenGameplay();  //rozgrywka
    void OpenWindowControl(); //strona z sterowaniem klawiatury i padem
    void OpenMenu(); //menu
    void CheckInputEventsForMainMenu();
    void DrawMainMenu();
    void DrawEyesAnimation();
    void OpenWindowCredits();
    void DrawTextStory();
    void FinishGame();
    void SetBackGround();
    void SetButtonsMenu();

private:
    enum GameState{MENU,CONTROL,CREDIT,GAME,END};
    GameState state;
    Font font;
    Audio audio;
    RenderWindow window;
    Texture textureBackGround;
    sf::VideoMode mode;
    sf::Texture eyesTextures;
    sf::Sprite eyesEnemySprite;
    sf::Sprite backgroundGame;
    Clock clock;
    int numberOfTextStory = 6;
    int lastLine=5;
    int numberOfButtonsMenu = 4;
    Text textButtonsMenu[4];
    Text textStory[6];    //sf::Text
    Text titleGame;
    string story[6]={"God dammit!",
    "The contract on Beelzebub seemed to be profible,",
    "but by my inattencion, i forget to get holy water!",
    "I must back to the surface, but I think I forgot the way back.",
    "I have to find way out of this... Labyrinth... DAUN'S LABYRINTH",
    "PRESS ENTER TO START"};
        };



#endif // GAME_H_INCLUDED

