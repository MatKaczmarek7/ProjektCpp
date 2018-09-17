    #include "Mapa.h"
    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <string>
    #include <cstdlib>
    #include <ctime> //lottery numbers
    #include <cmath>  //std::abs
    #include <stack> //generating
    #include <vector>
    #include <sstream> //saving

    Mapa::Mapa(sf::RenderWindow &win)
    {
        font.loadFromFile("data/devilssnare.ttf");
        theEnd=false;
        victory=false;

        music_background.LoadMusic(Audio::GameMusic);
        musicEnemy.LoadMusic(Audio::PanicMusic);
        music_background.PlayMusic();

        InitializeLighting();
        InitializeGPS();
        InitializeTorch();
    //!Maze
        GenerateMaze();
        LocateGamePlayElements();
        InitializeCompass();
        RunMap(win);
    }

    Mapa::~Mapa()
    {

    }

    void Mapa::InitializeLighting()
    {
        m_layer.create(2500,2500);
        m_layer.clear();
        m_flashlightTexture.create(200,200);
        m_flashlightTexture.clear();

            // Draw 6 circles with increasing transparency
        for(unsigned int i = 0; i < 6; ++i)
        {
            sf::CircleShape circleShape(90.f-(i*2.f));
            circleShape.setOrigin(sf::Vector2f(30.f-(i*2.f), 30.f-(i*2.f)));
            circleShape.setFillColor(sf::Color(255, 255, 255, 61-(i*10))); //RGB + transparency
            circleShape.setPosition(sf::Vector2f(30.f, 30.f));

            m_flashlightTexture.draw(circleShape,sf::BlendAdd);
        }

        m_flashlightTexture.display();
        m_flashlight.setTexture(m_flashlightTexture.getTexture(), true);

        m_flashlight.setOrigin(30.f, 30.f);
        m_sprite.setTexture(m_layer.getTexture());
    }

    void Mapa::InitializeTorch()
    {
        tex_torch.loadFromFile("data/pochodnieink.png",IntRect(0,0,50,50));
        torch.setTexture(tex_torch);
        iconTorchTexture.loadFromFile("data/flame.png",IntRect(0,0,318,600));
    }

    void Mapa::InitializeGPS()
    {
        textureGPS.loadFromFile("data/Red_Arrow.png",IntRect(0,0,100,100));
        gps.setTexture(textureGPS);
        gps.setOrigin(50,50);
        gps.scale(0.25,0.25);
    }

    void Mapa::InitializeCompass()
    {
        compassTexture.loadFromFile("data/kompas1.png",IntRect(0,0,200,213));
        compassSprite.setTexture(compassTexture);
        compassSprite.scale(0.2,0.2);
        compassSprite.setPosition(player.getPosition().x-280,player.getPosition().y+110);
    }

    void Mapa::InitializeMaze()
    {
        for(int i=1;i<COUNT;i++)
        {
            textureMap[i].loadFromFile("data/mappa.png",IntRect((i-1)*tile_width,0,tile_width,tile_height));
        }

        textureExit.loadFromFile("data/Rysunek.png",IntRect(0,0,50,50));
        EXIT.setTexture(textureExit);

        for(int i=0; i<SIZE; i++)
        {
            for(int j=0; j<SIZE; j++)
            {
                Level[i][j].visited = true;
                if(i%2==1 || j%2==1)
                {
                    Level[i][j].visited=false;
                }
                Level[i][j].nr_cell=0;
                (Level[i][j].display).setTexture(textureMap[1]);
                (Level[i][j].display).setPosition(j*tile_width,i*tile_height);
            }
        }

    //!Edges - Boundering
        for(int i=1; i<SIZE; i++)
        {
            for(int j=1; j<SIZE; j++)
            {
                Level[0][j].visited = true;
                Level[SIZE-1][j].visited = true;
                Level[i][0].visited = true;
                Level[i][SIZE-1].visited = true;
            }
        }
    }

    void Mapa::GenerateMaze()
    {
        InitializeMaze();
        srand((unsigned)time(NULL));
        int random = 0;
        int randomX = ((2*rand())+1)%(SIZE-1);
        int randomY = ((2*rand())+1)%(SIZE-1);


        while(randomX==0 || randomY==0)
        {
            randomX = ((2*rand())+1)%(SIZE-1);
            randomY = ((2*rand())+1)%(SIZE-1);
        }

        int visitedCells = 1;

        stack<int> back_trackX, back_trackY;

       //!!First Cell
        Level[randomY][randomX].display.setTexture(textureMap[2]);
        Level[randomY][randomX].nr_cell=1;
        Level[randomY][randomX].visited = true;


        while(visitedCells<1600)
        {
            std::vector<string> PossibleDirection;
            PossibleDirection.clear();

             if (randomX>0 && Level[randomY][ randomX - 1].visited==false) //
             {
                PossibleDirection.push_back("left");
             }
             if (randomY>0 && Level[randomY-1][ randomX].visited==false)
             {
                PossibleDirection.push_back("top");
             }
             if (randomY<SIZE-1 && Level[randomY+1][ randomX].visited==false)
             {
                PossibleDirection.push_back("bottom");

             }
            if (randomX<SIZE-1 && Level[randomY][ randomX + 1].visited==false)
            {
                PossibleDirection.push_back("right");
            }

            unsigned int posibbleDirectionSize=PossibleDirection.size();
                    if(posibbleDirectionSize!=0)
                    {
                        random = (rand() % (posibbleDirectionSize));
                        string dir = PossibleDirection[random];
                        SetVisitedCell(randomX,randomY);
                        visitedCells++;
                         if (dir == "left")
                         {
                            randomX--;
                         }

                        else if (dir == "right")
                        {
                            randomX++;
                        }
                        else if (dir == "top")
                        {
                            randomY--;
                        }

                        else if (dir == "bottom")
                        {
                            randomY++;
                        }
                        else
                        std::cout<<"error"<<std::endl;

                    back_trackX.push(randomX);
                    back_trackY.push(randomY);
                }
                else
                {
                    randomX = back_trackX.top();
                    back_trackX.pop();
                    randomY = back_trackY.top();
                    back_trackY.pop();
                }
            }
        OptimizeMaze();
    }

    void Mapa::LocateGamePlayElements()
    {
        int randomx,randomy;
        bool lookingPosition=true;
        while(lookingPosition)
        {
            randomx=(rand()%(SIZE-3))+2;
            randomy=(rand()%(SIZE-3))+2;

               if(Level[randomy][randomx].nr_cell==1)
               {
                   lookingPosition=false;
                   player.Setposition((randomx)*tile_width+20,(randomy)*tile_width+20);
               }
        }

        sf::Vector2f playerPosition =player.GetCreaturePosition();
        lookingPosition=true;

        while(lookingPosition)
        {
            randomx=(rand()%(SIZE-3))+2;
            randomy=(rand()%(SIZE-3))+2;

               if(Level[randomy][randomx].nr_cell==1)
               {
                   enemy.Setposition((randomx)*tile_width+20,(randomy)*tile_width+20);
                   sf::Vector2f poz2=enemy.GetCreaturePosition();
                    if(abs(playerPosition.x-poz2.x)>1000 && abs(playerPosition.y-poz2.y)>1000)
                   lookingPosition=false;
               }
        }

        /**Exit**/

        lookingPosition=true;
        while(lookingPosition)
        {
            randomx=(rand()%(SIZE-3))+2;
            randomy=(rand()%(SIZE-3))+2;

            if(Level[randomy][randomx].nr_cell==1)
            {
                EXIT.setPosition((randomx)*tile_width,(randomy)*tile_width);
                sf::Vector2f poz2=EXIT.getPosition();
                if(abs(playerPosition.x-poz2.x)>1000 && abs(playerPosition.y-poz2.y)>1000)
                    lookingPosition=false;
            }
        }
    }

    void Mapa::SetVisitedCell(int X, int Y)
    {
        Level[Y][X].visited=true;
        (Level[Y][X].display).setTexture(textureMap[2]);
        Level[Y][X].nr_cell=1;
    }


    void Mapa::OptimizeMaze()
    {
        int visited=1;
        int total=200;
        int randomx;
        int randomy;
        srand((unsigned)time(NULL));

        while(visited<total)
        {
            randomx=(rand()%(SIZE-3))+2;
            randomy=(rand()%(SIZE-3))+2;

                if(Level[randomx][randomy].visited==false)
                {
                    Level[randomx][randomy].display.setTexture(textureMap[2]);
                     Level[randomx][randomy].nr_cell=1;
                    visited++;
                }
        }
    }

    void Mapa::RunMap(sf::RenderWindow &window)
    {
        window_f=&window;
        bool menu =false;
        Clock clock;
        Time dt;
        clockGame.restart();

        sf::Vector2f playerPosition;

        gps_time=gps_clock.restart();
        torch_time.restart();

        /***/

        const int TICKS_PER_SECOND = 50;
        const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
        const int MAX_FRAMESKIP = 10;

        sf::Clock TEST;
        sf::Time TEST_TIME=TEST.restart();
        float next_game_tick=TEST_TIME.asMilliseconds();
        int loops;

        while(!menu)
        {
            loops = 0;

            //!!Set Screen
            playerPosition=player.GetCreaturePosition();
            sf::Vector2f sizeScreen(600,300);
            playerPosition.x=playerPosition.x-(sizeScreen.x/2);
            playerPosition.y=playerPosition.y-(sizeScreen.y/2);
            sf::View view(sf::FloatRect(playerPosition, sizeScreen));

            dt=clock.restart();

            Event event;
            Vector2f mysz(Mouse::getPosition(window));
            //!Events
            while(window.pollEvent(event))
            {
                if(event.type == Event::KeyReleased && event.key.code == Keyboard::Escape)
                    menu =true;

                if(event.type==Event::KeyReleased)
                {
                    if(event.key.code==Keyboard::W || event.key.code==Keyboard::S || event.key.code==Keyboard::D || event.key.code==Keyboard::A)
                    {
                        player.Stop();
                    }
                }
            }

            TEST_TIME=TEST.getElapsedTime();
            while( TEST_TIME.asMilliseconds() > next_game_tick && loops < MAX_FRAMESKIP)
            {
                next_game_tick += SKIP_TICKS;
                loops++;

                if(!theEnd)
                {
                    CheckInputForMoveScreen(view,dt);
                }
                CheckGPSState();
                ManageCreatingTorches();
                UpdateCreaturesState(dt);
                CheckGameOver();
             }

            DrawAllElements(view);

        }
        music_background.StopMusic();
        musicEnemy.StopMusic();
    }

    void Mapa::CheckInputForMoveScreen(sf::View view,sf::Time dt)
    {
        if(Keyboard::isKeyPressed(Keyboard::W))
        {
            view.move(0,-300*dt.asSeconds());
        }
        if(Keyboard::isKeyPressed(Keyboard::S))
        {
            view.move(0,300*dt.asSeconds());
        }
        if(Keyboard::isKeyPressed(Keyboard::A))
        {
            view.move(-300*dt.asSeconds(),0);
        }
        if(Keyboard::isKeyPressed(Keyboard::D))
        {
            view.move(300*dt.asSeconds(),0);
        }
    }

    void Mapa::DrawAllElements(sf::View view)
    {
            sf::Vector2f playerPosition;
            window_f->clear();
            window_f->setView(view);

            DrawMap();
            window_f->draw(EXIT);
            window_f->draw(player);
            window_f->draw(enemy);
            DrawTorches();
            DrawMiniMap();
            window_f->setView(view);
            SetWatchGame();
            SetGPS();
            DrawCompassSprite();
            DrawTorchIconUI();
            TryToSetGameOverText();

            window_f->display();
    }

    void Mapa::DrawMap()
    {
        int wspx, wspy;
        sf::Vector2f playerPosition= player.GetCreaturePosition();
        wspx=(int)(playerPosition.x/50);
        wspy=(int)(playerPosition.y/50);

        window_f->draw(Level[wspy][wspx].display);
        int rangeX=wspx+8;
        int rangeY=wspy+8;
        wspx-=8;
        wspy-=8;

        for(int i=wspx;i<rangeX;i++)
        {
            for(int z=wspy;z<rangeY;z++)
            {
                if(i>=0 && i<=SIZE-1 && z>=0 && z<=SIZE-1)
                {
                        window_f->draw(Level[z][i].display);
                }
            }
        }
    }

    void Mapa::DrawTorches()
    {
        sf::Vector2f playerPosition=player.GetCreaturePosition();
        for(int i=0;i<torches.size();i++)
        {
            if(abs(torches[i].getPosition().x-playerPosition.x)<600&&abs(torches[i].getPosition().y-playerPosition.y)<600)
            {
                window_f->draw(torches[i]);
            }
        }

        m_layer.clear();

        for(int i=0;i<torches.size();i++)
        {
            sf::Vector2f lightFromTorch(torches[i].getGlobalBounds().left,torches[i].getGlobalBounds().top);
            m_flashlight.setPosition(lightFromTorch.x-45,lightFromTorch.y-40);

            m_layer.draw(m_flashlight, sf::BlendAdd);
            m_layer.display();
        }

        sf::Vector2f lightPosition(player.getBoundingBox().left,player.getBoundingBox().top);
        m_flashlight.setPosition(lightPosition.x-45,lightPosition.y-40);
        m_layer.draw(m_flashlight,sf::BlendAdd);
        m_layer.display();
        window_f->draw(m_sprite,sf::BlendMultiply);
    }

    void Mapa::DrawCompassSprite()
    {
            sf::Vector2f playerPosition=player.GetCreaturePosition();
            compassSprite.setPosition(playerPosition.x-275,playerPosition.y+100);
            window_f->draw(compassSprite);

            std::string countGPSString;
            std::ostringstream ss;

            ss<<gpsCounter;
            countGPSString=ss.str();

            sf::Text countGPSText;
            countGPSText.setString(countGPSString);
            countGPSText.setCharacterSize(30);
            countGPSText.setFont(font);
            countGPSText.setColor(Color::Red);
            countGPSText.setPosition(playerPosition.x-220,playerPosition.y+100);
             window_f->draw(countGPSText);
    }

    void Mapa::DrawMiniMap()
    {
        sf::View minimap;
        minimap.setViewport(sf::FloatRect(0.75f,0,0.25f,0.25f));
        minimap.setCenter(player.GetCreaturePosition());
        sf::Vector2f playerPosition = player.GetCreaturePosition();

        window_f->setView(minimap);

        sf::RectangleShape object;
        object.setSize(sf::Vector2f(50,50));

        sf::RectangleShape object2;
        object2.setSize(sf::Vector2f(50,50));
        object2.setFillColor(sf::Color::Yellow);
        object2.setPosition(player.GetCreaturePosition());

        object.setFillColor(sf::Color::White);

        if(mini.empty())
        {
            object.setPosition(player.GetCreaturePosition());
            mini.push_back(object);
        }

        bool covering=false;
        for(int i=0; i<mini.size(); i++)
        {
            sf::FloatRect miniFloatRect = mini[i].getGlobalBounds();
            sf::FloatRect objectFloatRect = object.getGlobalBounds();
            if(miniFloatRect.intersects(objectFloatRect))
            {
                covering=true;
                break;
            }
        }

        if(!covering)
        {
            object.setPosition(player.GetCreaturePosition());
            mini.push_back(object);
        }

        if(canTorchIcon)
        {
            canTorchIcon=false;
            sf::RectangleShape object3;
            object3.setPosition(player.GetCreaturePosition());
            object3.setSize(sf::Vector2f(50,50));
            object3.setFillColor(sf::Color::Red);
            minitorch.push_back(object3);
        }

        for(int i=0;i<mini.size();i++)
        {
            if(mini[i].getPosition().x>playerPosition.x-550 && mini[i].getPosition().x<playerPosition.x+550
                && mini[i].getPosition().y>playerPosition.y-550 && mini[i].getPosition().y<playerPosition.y+550)
                window_f->draw(mini[i]);
        }

        for(int i=0;i<minitorch.size();i++)
        {
            if(minitorch[i].getPosition().x>playerPosition.x-550 && minitorch[i].getPosition().x<playerPosition.x+550
                && minitorch[i].getPosition().y>playerPosition.y-550 && minitorch[i].getPosition().y<playerPosition.y+550)
            window_f->draw(minitorch[i]);
        }

        window_f->draw(object2);
    }

    void Mapa::CheckGPSState()
    {
        if(gpsCounter!=0 && !gate_gps && Keyboard::isKeyPressed(Keyboard::Return))
        {
            gate_gps=true;
            gps_time=gps_clock.restart();
            gpsCounter--;
        }

        gps_time=gps_clock.getElapsedTime();
        float sec=gps_time.asSeconds();
        if(sec>5)
        {
            gate_gps=false;
        }
    }

    void Mapa::ManageCreatingTorches()
    {
        if(canCountTorch&&!torch_gate && Keyboard::isKeyPressed(Keyboard::Space))
        {
            torchCounter--;
            sf::Vector2f torchPosition=player.GetCreaturePosition();
            torch.setPosition(torchPosition.x-25,torchPosition.y-25);
            torches.push_back(torch);
            torch_time.restart();
            torch_gate=true;
            canTorchIcon=true;
            if(torchCounter==0)
            {
                canCountTorch=false;
            }
            else
            {
                canCountTorch=true;
            }
        }

        if(torch_time.getElapsedTime()>sf::seconds(5))
        {
            torch_gate=false;
        }
    }

    void Mapa::UpdateCreaturesState(sf::Time time)
    {
        if(!theEnd)
        {
            player.Update(time);
        }

        enemy.Update();
        if(!theEnd)
        {
            Astar();
        }

        CheckCollision(time);
    }

    void Mapa::CheckCollision(sf::Time delta)
    {
        sf::FloatRect player_box= player.getBoundingBox();
        sf::FloatRect object_box;

        object_box.top=player_box.top;
        object_box.left=player_box.left+4;
        object_box.height=player_box.height;
        object_box.width=player_box.width;

        sf::Vector2f poz=player.GetCreaturePosition();
        int xx=(int)(poz.x/50);
        int yy=(int)(poz.y/50);

        std::vector<Cell> cells;
        cells.push_back(Level[yy][xx+1]);
        cells.push_back(Level[yy][xx-1]);
        cells.push_back(Level[yy+1][xx]);
        cells.push_back(Level[yy-1][xx]);

        cells.push_back(Level[yy+1][xx+1]);
        cells.push_back(Level[yy+1][xx-1]);
        cells.push_back(Level[yy-1][xx+1]);
        cells.push_back(Level[yy-1][xx-1]);

        for(int i=0;i<cells.size();i++)
        {
            sf::FloatRect level_box=(cells[i].display).getGlobalBounds();
            if(object_box.intersects(level_box))
            {
                if(cells[i].nr_cell==1)
                    continue;
                player.MoveYourself(-400*delta.asSeconds(),0);
            }
       }
    //Right Wall
        object_box.left=player_box.left-4;

        for(int i=0;i<cells.size();i++)
        {
            sf::FloatRect level_box=(cells[i].display).getGlobalBounds();
            if(object_box.intersects(level_box))
            {
                if(cells[i].nr_cell==1)
                    continue;
                player.MoveYourself(400*delta.asSeconds(),0);
            }
        }
    //Up wall

        object_box.left=player_box.left;
        object_box.top=player_box.top+4;

        for(int i=0;i<cells.size();i++)
        {
            sf::FloatRect level_box=(cells[i].display).getGlobalBounds();
            if(object_box.intersects(level_box))
            {
                if(cells[i].nr_cell==1)
                    continue;
                player.MoveYourself(0,-400*delta.asSeconds());
            }
        }
    //Down wall
        object_box.top=player_box.top-4;

        for(int i=0;i<cells.size();i++)
        {
            sf::FloatRect level_box=(cells[i].display).getGlobalBounds();
            if(object_box.intersects(level_box))
            {
                if(cells[i].nr_cell==1)
                    continue;
                player.MoveYourself(0,400*delta.asSeconds());
            }
        }
        cells.clear();
    }

    void Mapa::Astar()
    {
        countMusic=0;
        openSet.clear();
        closedSet.clear();

        sf::Vector2f enemyPosition = enemy.GetCreaturePosition();
        sf::Vector2f playerPosition = player.GetCreaturePosition();

        Cell* start_Node=&Level[(int) (enemyPosition.y/50) ] [(int)(enemyPosition.x/50)];
        target_Node=&Level[(int)(playerPosition.y/50) ] [(int)(playerPosition.x/50)];

        start_Node->gCost=0;
        start_Node->parent=NULL;
        start_Node->hCost=getTrace(start_Node,target_Node);
        start_Node->fCost=start_Node->gCost+start_Node->hCost;
        openSet.push_back(start_Node);
        if(start_Node==target_Node)
        {
            sf::Vector2f startPosition(start_Node->display.getPosition().x,start_Node->display.getPosition().y);
            sf::Vector2f daunPosition(enemy.GetCreaturePosition().x,enemy.GetCreaturePosition().y);

            if(startPosition.x+20 == daunPosition.x && startPosition.y+20 == daunPosition.y)
            {
                if(daunPosition.x>playerPosition.x+5)
                {
                    enemy.MoveYourself(-2,0);
               //Left
                    enemy.SetStatus(2);
                    return;
                }
                if(daunPosition.x+5<playerPosition.x)
                {
                    enemy.MoveYourself(2,0);
                 // Right
                    enemy.SetStatus(3);
                    return;
                }

                if(daunPosition.y>playerPosition.y+5)
                {
                    enemy.MoveYourself(0,-2);
                   //   Up
                    enemy.SetStatus(0);
                    return;
                }

                if(daunPosition.y+5<playerPosition.y)
                {
                    enemy.MoveYourself(0,2);
                     //   downn
                    enemy.SetStatus(1);
                    return;
                }
            }
            else
            {
                if(enemy.GetState()==0)
                {
                    enemy.MoveYourself(0,-2);
                    return;
                }

                if(enemy.GetState()==1)
                {
                    enemy.MoveYourself(0,2);
                    return;
                }

                if(enemy.GetState()==2)
                {
                    enemy.MoveYourself(-2,0);
                    return;
                }

                if(enemy.GetState()==3)
                {
                    enemy.MoveYourself(2,0);
                    return;
                }
            }
        }

        while(!openSet.empty())
        {
            ++countMusic;

            current_Node=openSet[0];
            for(int i=1; i<openSet.size();i++)
            {
                if(openSet[i]->fCost<current_Node->fCost || openSet[i]->fCost==current_Node->fCost && openSet[i]->hCost< current_Node->hCost)
                {
                    current_Node=openSet[i];
                }
            }
            remove_node(current_Node);
            closedSet.push_back(current_Node);

            if(current_Node==target_Node)
            {
                if(countMusic<5)
                {
                    if(music_end==true)
                    {
                        music_end=false;
                        music_start=true;
                        music_background.StopMusic();
                        musicEnemy.PlayMusic();
                    }
                }

                if(countMusic>8)
                {
                    if(music_start==true)
                    {
                        music_end=true;
                        music_start=false;
                        musicEnemy.StopMusic();
                        music_background.PlayMusic();
                    }
                }

                Cell *currentCell=current_Node;

                while(currentCell->parent->display.getPosition().x!=start_Node->display.getPosition().x  || currentCell->parent->display.getPosition().y!=start_Node->display.getPosition().y ) //dodalem &
                {
                    currentCell=currentCell->parent;
                }

                sf::Vector2f startPosition(start_Node->display.getPosition().x,start_Node->display.getPosition().y);
                sf::Vector2f daunPosition(enemy.GetCreaturePosition().x,enemy.GetCreaturePosition().y);

                if(startPosition.x+20 == daunPosition.x && startPosition.y+20 == daunPosition.y)
                {
                    if(start_Node->display.getPosition().x > currentCell->display.getPosition().x)
                    {
                        enemy.MoveYourself(-2,0);
               //       Left
                        enemy.SetStatus(2);
                        return;
                    }

                    if(start_Node->display.getPosition().x < currentCell->display.getPosition().x)
                    {
                        enemy.MoveYourself(2,0);
                 //     Right
                        enemy.SetStatus(3);
                        return;
                    }

                    if(start_Node->display.getPosition().y > currentCell->display.getPosition().y)
                    {
                        enemy.MoveYourself(0,-2);
                   //   Up
                        enemy.SetStatus(0);
                        return;
                    }

                    if(start_Node->display.getPosition().y < currentCell->display.getPosition().y)
                    {
                         enemy.MoveYourself(0,2);
                     //   Down
                        enemy.SetStatus(1);
                           return;
                    }
                }
                else
                {
                    if(enemy.GetState()==0)
                    {
                        enemy.MoveYourself(0,-2);
                        return;
                    }
                    if(enemy.GetState()==1)
                    {
                        enemy.MoveYourself(0,2);
                        return;
                    }
                    if(enemy.GetState()==2)
                    {
                        enemy.MoveYourself(-2,0);
                        return;
                    }
                    if(enemy.GetState()==3)
                    {
                        enemy.MoveYourself(2,0);
                        return;
                    }
                }
            }
            std::vector<Cell *> cachedNeighBours=getNeighbours(current_Node);

            for(int i=0; i<cachedNeighBours.size();i++)
            {
                SetCostNeighbour(cachedNeighBours[i]);
            }
            cachedNeighBours.clear();
            }
    }

    void Mapa::SetCostNeighbour(Cell *neighbour)
    {
        bool gate=false;

        for(int i=0;i<closedSet.size();i++)
        {
            if(neighbour==closedSet[i])
            {
                return;
            }
        }

        int newMovementToCost = current_Node->gCost+getTrace(current_Node,neighbour);

        for(int i=0; i<openSet.size(); i++)
        {
            if(neighbour==openSet[i])
            {
                gate=true;
                break;
            }
        }

        if(newMovementToCost<neighbour->gCost || gate==false)
        {
            neighbour->gCost=newMovementToCost;
            neighbour->hCost=getTrace(neighbour,target_Node);
            neighbour->fCost=neighbour->hCost+neighbour->gCost;

            neighbour->parent=current_Node;

            if(gate==false)
            {
                openSet.push_back(neighbour);
            }
        }
    }

    vector<Mapa::Cell*> Mapa::getNeighbours(Mapa::Cell* node)
    {
        std::vector <Cell*> targetNeighbours;
        sf::Vector2f pozycja_Node = node->display.getPosition();
        int xx=pozycja_Node.x/50;
        int yy=pozycja_Node.y/50;

        if(xx!=50)
        if(Level[yy][xx+1].nr_cell==1) /*****Right*****/
        {
            targetNeighbours.push_back(&Level[yy][xx+1]);
        }

        if(xx!=-1)
         if(Level[yy][xx-1].nr_cell==1)    /***Left**/
        {
            targetNeighbours.push_back(&Level[yy][xx-1]);
        }
        if(yy!=50)
         if(Level[yy+1][xx].nr_cell==1) /***Down****/
        {
            targetNeighbours.push_back(&Level[yy+1][xx]);
        }

        if(yy!=-1)
         if(Level[yy-1][xx].nr_cell==1) /*****Up*****/
        {
            targetNeighbours.push_back(&Level[yy-1][xx]);
        }

        return targetNeighbours;
    }

    int Mapa::getTrace(Cell *A, Cell *B)
    {

        int dstX=(int)(std::abs(A->display.getPosition().x-B->display.getPosition().x));
        int dstY=(int)(std::abs(A->display.getPosition().y-B->display.getPosition().y));

        if(dstX>dstY)
        {
            return 71*dstY + 50*(dstX-dstY);
        }
        return 71*dstX +50 *(dstY-dstX);

    }

    void Mapa::remove_node(Cell* A)
    {

        for(int i=0; i<openSet.size(); i++)
        {
            if(openSet[i]==A)
            {
                for(int y=i;y<openSet.size()-1;y++)
                {
                    std::swap(openSet[y],openSet[y+1]);

                }
                openSet.resize(openSet.size()-1);
                break;
            }
        }
    }

    void Mapa::SetWatchGame()
    {
        sf::Text timeText;
        std::string s,d,m;
        std::ostringstream rr;
        if(theEnd==false)
        time_game=clockGame.getElapsedTime();
        int timeGameAsSecond=(int)time_game.asSeconds();
        if(timeGameAsSecond==60 && theEnd==false)
        {
            clockGame.restart();
            timeGameAsSecond=0;
            minutes++;
        }
        rr<<timeGameAsSecond;
        s=rr.str();

        std::ostringstream ss;
        ss<<minutes;
        m=ss.str();
        if(timeGameAsSecond<10)
        {
             d=m+" :0"+s;
        }
        else
        {
            d=m+" :"+s;
        }

        timeText.setCharacterSize(30);
        timeText.setString(d);
        timeText.setFont(font);
        timeText.setColor(sf::Color::Red);
        sf::Vector2f playerPosition=player.GetCreaturePosition();
        timeText.setPosition(playerPosition.x-270,playerPosition.y-140);

        window_f->draw(timeText);

    }

    void Mapa::CheckGameOver()
    {
        sf::FloatRect playerFloatRect = player.getBoundingBox();
        sf::RectangleShape rectanglePlayerShape;
        rectanglePlayerShape.setPosition(playerFloatRect.left+14,playerFloatRect.top+12);
        rectanglePlayerShape.setSize(sf::Vector2f(5,5));
        sf::FloatRect playerBox=rectanglePlayerShape.getGlobalBounds();

        sf::FloatRect enem= enemy.getBoundingBox();
        sf::RectangleShape rectangleEnemyShape;
        rectangleEnemyShape.setPosition(enem.left,enem.top);
        rectangleEnemyShape.setSize(sf::Vector2f(30,30));
        sf::FloatRect enemyBox=rectangleEnemyShape.getGlobalBounds();

        if(playerBox.intersects(enemyBox))
        {
            theEnd=true;
        }
    }

    void Mapa::SetGPS()
    {
        if(gpsCounter>=0 && gate_gps)
        {
            sf::Vector2f coordy2 = player.GetCreaturePosition();
            sf::Vector2f poz_exit(EXIT.getPosition().x+25,EXIT.getPosition().y+25);
            sf::Vector2f norm=poz_exit-coordy2;
            gps.setPosition(coordy2.x,coordy2.y);
            float rotation=atan2(norm.y,norm.x);
            rotation=rotation*180.f/3.14;
            rotation+=190;
            gps.setRotation(rotation);
            float kat=gps.getRotation();
            float xN = sin(( kat * 3.14 ) / 180.0f );
            float yN = -cos(( kat * 3.14 ) / 180.0f );

            gps.move(yN*25,-xN*25);
            window_f->draw(gps);
        }
    }

    void Mapa::DrawTorchIconUI()
    {
        sf::Sprite iconT;
        iconT.setTexture(iconTorchTexture);
        iconT.scale(0.09,0.09);
        sf::Vector2f poz=player.GetCreaturePosition();
        iconT.setPosition(poz.x-190,poz.y+90);

        window_f->draw(iconT);

        sf::Text countTorchText;
        std::ostringstream rr;
        rr<<torchCounter;
        string s;
        s=rr.str();
        countTorchText.setString(s);
        countTorchText.setCharacterSize(30);
        countTorchText.setFont(font);
        countTorchText.setPosition(poz.x-150,poz.y+100);
        countTorchText.setColor(Color::Red);
        window_f->draw(countTorchText);
    }

    void Mapa::TryToSetGameOverText()
    {
        if(theEnd && !victory)
        {
            Text deathText("YOU DIE",font,45);
            deathText.setColor(Color::Red);
            sf::Vector2f coord = player.GetCreaturePosition();
            deathText.setPosition(coord.x-90,coord.y-30);
            window_f->draw(deathText);
        }

        sf::FloatRect box1 =player.getBoundingBox();
        sf::FloatRect box2 =EXIT.getGlobalBounds();

        if(box1.intersects(box2))
        {
            theEnd=true;
            victory=true;
            Text victoryText("YOU WIN",font,45);
            victoryText.setColor(Color::Blue);
            sf::Vector2f coordy = player.GetCreaturePosition();
            victoryText.setPosition(coordy.x-90,coordy.y-30);
            window_f->draw(victoryText);
        }
    }
