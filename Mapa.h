#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Mapa.h"
#include "Audio.h"
#include <vector>
#include <list>

#define SIZE 50

using namespace std;
using namespace sf;

class Mapa{

private:

    struct Cell
    {
        bool visited;
        sf:: Sprite display;
        int gCost;      //FROM START TO CURRENT
        int hCost;      //FROM CURRENT TO END
        int fCost;      //GCOS + FCOST
        int nr_cell;    //which texture
        Cell *parent;

        bool operator ==(const Cell* B)
        {
            sf::Vector2f sizeA=this->display.getPosition();
            sf::Vector2f sizeB=B->display.getPosition();
            return (sizeA.x==sizeB.x && sizeA.y==sizeB.y);
        }
    };

        Cell Level[SIZE][SIZE];     //Map

         enum FieldType{
        NONE,STONE,FLOOR,COUNT};    //TYP
        sf::Texture textureMap[COUNT];
        sf::Texture textureExit;

        sf::Sprite EXIT;

        int tile_width=50;
        int tile_height=50;

    //!!DO A STAR
        Cell *current_Node; //CURRENT
        Cell *target_Node;  //END
        std::vector<Cell *> openSet;
        std::vector<Cell *> closedSet;

        int countMusic=0;    //WYBIERANIE MUZYKI W ASTAR
        bool music_start=false;
        bool music_end=true;

        bool theEnd;
        bool victory;

        Font font;

        Player player;
        Enemy enemy;

    //Lighting
        sf::RenderTexture m_flashlightTexture;
        sf::RenderTexture m_layer;
        sf::Sprite m_flashlight;
        sf::Sprite m_sprite;


        //!GPS
        sf::Texture textureGPS;
        sf::Sprite gps;
        int gpsCounter=2;
        bool gate_gps=false;
        Clock gps_clock;
        Time gps_time;

        std::vector<sf::Sprite> compasses;
        sf::Sprite compassSprite;
        sf::Texture compassTexture;


        //!AUDIO

        Audio music_background;
        Audio musicEnemy;

        //!Timer
        int minutes=0;
        sf::Time time_game;
        Clock clockGame;

        //!TORCH

        std::vector<sf::Sprite> torches;
        sf::Sprite torch;
        sf::Texture tex_torch;
        Clock torch_time;
        int torchCounter=5;
        sf::Texture iconTorchTexture;
        bool canTorchIcon=false;
        bool torch_gate=false;
        bool canCountTorch=true;

        std::vector<sf::RectangleShape> mini;
        std::vector<sf::RectangleShape>minitorch;

        RenderWindow *window_f;

public:

        Mapa(sf::RenderWindow &win);
        ~Mapa(void);
        void RunMap(sf::RenderWindow &window);

        void InitializeLighting();
        void InitializeMaze();
        void GenerateMaze();
        void OptimizeMaze();
        void LocateGamePlayElements();
        void SetVisitedCell(int X, int Y);
        void CheckCollision(sf::Time delta);
        void CheckGameOver();
        void CheckInputForMoveScreen(sf::View view,sf::Time dt);

        void InitializeTorch();
        void InitializeGPS();
        void InitializeCompass();

        void CheckGPSState();
        void ManageCreatingTorches();

        void UpdateCreaturesState(sf::Time dt);

        //! A STAR
        void Astar();
        int getTrace(Cell *A, Cell *B);
        vector<Cell*> getNeighbours(Cell *node);
        void remove_node(Cell * A);
        void SetCostNeighbour(Cell *neighbour);

        void SetWatchGame();
        void DrawTorchIconUI();
        void SetGPS();
        void TryToSetGameOverText();

        void DrawAllElements(sf::View view);
        void DrawMap();
        void DrawTorches();
        void DrawMiniMap();
        void DrawCompassSprite();
};

#endif // MAPA_H_INCLUDED
