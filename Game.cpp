    #include "Game.h"
    #include <iostream>
    #include <ctime>
    #include <string>
    #include <sstream>

    Game::Game(void)
    {
        Initialize();
        mode = sf::VideoMode::getDesktopMode();
        window.create(mode,"DAUN",Style::Fullscreen);
        state=END;

        if (!font.loadFromFile("data/devilssnare.ttf"))   //remember to load file for IDE or .exe
        {
            MessageBox(NULL,"FONT NOT FOUND","ERROR",NULL);
            return;
        }

        window.setVerticalSyncEnabled(true); //synchronization 60 fps
        state=MENU; //set state again after loading Font

        audio.LoadMusic(Audio::MenuMusic);
        audio.PlayMusic();
        SetBackGround();
        eyesTextures.loadFromFile("data/czteryoczy.png",IntRect(0,0,147,132));
        eyesEnemySprite.setTexture(eyesTextures);
        eyesEnemySprite.scale(2,1.5);
    }

    void Game::Initialize()
    {
    }

    Game::~Game(void)
    {
    audio.StopMusic();
    }

    void Game::RunGame()
    {
        while(state!=END)
        {
            switch(state)
            {
            case MENU:
                OpenMenu();
                break;
            case CONTROL:
                OpenWindowControl();
                break;
            case GAME:
                OpenGameplay();
                break;
            case CREDIT:
                OpenWindowCredits();
                break;
            }
        }
    }

    void Game::OpenGameplay()
    {
        audio.StopMusic();
        Event event;
        while(state==GAME)
        {
            while(window.pollEvent(event))
            {
                if(event.type==Event::Closed || event.type==Event::KeyPressed &&
                   event.key.code==Keyboard::Escape)
                {
                    state=MENU;
                    audio.PlayMusic();
                    return;
                }
            }

             if(Keyboard::isKeyPressed(Keyboard::Return))
             {
                 state=MENU;
             }
           DrawTextStory();
        }
        Mapa mapa(window);
        FinishGame();
    }

    void Game::DrawTextStory()
    {
        window.clear();
        for (int i=0; i<numberOfTextStory;i++)
        {
             if(i==lastLine)
            {
                textStory[i].setFont(font);
                textStory[i].setCharacterSize(45);
                textStory[i].setColor(Color::Red);
                textStory[i].setString(story[i]);
                textStory[i].setPosition(1280/2-textStory[i].getGlobalBounds().width/2,600);
            }
            else
            {
                textStory[i].setFont(font);
                textStory[i].setCharacterSize(40);
                textStory[i].setColor(Color::White);
                textStory[i].setString(story[i]);
                textStory[i].setPosition(20,100+i*70);
            }
        }

        for(int i=0;i<numberOfTextStory;i++)
            window.draw(textStory[i]);

        window.display();
    }

    void Game::FinishGame()
    {
        audio.PlayMusic();
        window.setView(window.getDefaultView());
        window.setMouseCursorVisible(true);
        state=MENU;
    }

    void Game::OpenMenu()
    {
        SetButtonsMenu();

        while(state==MENU)
        {
            CheckInputEventsForMainMenu();
            window.clear();
            DrawMainMenu();
        }
    }

    void Game::SetBackGround()
    {
        if (!textureBackGround.loadFromFile("data/dauns.png"))
        {
            MessageBox(NULL,"FONT NOT FOUND","ERROR",NULL);
            return;
        }
        backgroundGame.setTexture(textureBackGround);

        titleGame.setString("DAUN'S LABYRINTH");
        titleGame.setFont(font);
        titleGame.setCharacterSize(80);
        titleGame.setStyle(Text::Bold);
        titleGame.setPosition(1280/2-titleGame.getGlobalBounds().width/2,20);
        titleGame.setColor(Color::Red);
    }

    void Game::SetButtonsMenu()
    {
        string buttonsTitle[numberOfButtonsMenu]={"Start","How To Play","Credits","Exit"};

        for (int i=0; i<numberOfButtonsMenu;i++)
        {
            textButtonsMenu[i].setFont(font);
            textButtonsMenu[i].setCharacterSize(45);
            textButtonsMenu[i].setColor(Color::White);
            textButtonsMenu[i].setString(buttonsTitle[i]);
            textButtonsMenu[i].setPosition(1280/2-textButtonsMenu[i].getGlobalBounds().width/2,400+i*70);
        }
    }

    void Game::CheckInputEventsForMainMenu()
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        Event event;

        while(window.pollEvent(event))
        {
            if(event.type==Event::Closed || event.type==Event::KeyPressed &&
                   event.key.code==Keyboard::Escape)
            {
                state=END;
            }
            else if(textButtonsMenu[0].getGlobalBounds().contains(worldPos) &&
                    event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left) //Released ->puszczony
            {
                state = GAME;
            }

            else if(textButtonsMenu[1].getGlobalBounds().contains(worldPos)&&
                event.type == Event::MouseButtonReleased&& event.key.code==Mouse::Left)
            {
                state=CONTROL;
            }

            else if(textButtonsMenu[2].getGlobalBounds().contains(worldPos)&&
                event.type == Event::MouseButtonReleased&& event.key.code==Mouse::Left)
            {
                state=CREDIT;
            }

            else if(textButtonsMenu[3].getGlobalBounds().contains(worldPos) && event.type==Event::MouseButtonReleased &&
                        event.key.code==Mouse::Left)
            {
                Sleep(500);
                state=END;
            }
        }

            for(int i=0;i<numberOfButtonsMenu;i++)
            {
                if(textButtonsMenu[i].getGlobalBounds().contains(worldPos))
                    textButtonsMenu[i].setColor(Color::Yellow);
                    else
                    textButtonsMenu[i].setColor(Color::White);
            }
    }

    void Game::DrawMainMenu()
    {
        sf::View view(sf::FloatRect(0,0,1280,700));
        window.setView(view);

        window.draw(backgroundGame);
        window.draw(titleGame);
        for(int i=0;i<numberOfButtonsMenu;i++)
            window.draw(textButtonsMenu[i]);
        DrawEyesAnimation();
        window.capture();
        window.display();
    }

    void Game::OpenWindowControl()
    {
        Event event;
        while(state==CONTROL)
        {
            while(window.pollEvent(event))
            {
                if(event.type==Event::Closed || event.type==Event::KeyPressed &&
                   event.key.code==Keyboard::Escape)
                {
                    state=MENU;
                }
            }

            sf::Text moveText("WASD - move",font,50);
            moveText.setColor(Color::White);
            moveText.setPosition(1280/2-moveText.getGlobalBounds().width/2,350+70);
            sf::Text gps("ENTER - use gps",font,50);
            gps.setColor(Color::White);
            gps.setPosition(1280/2-gps.getGlobalBounds().width/2,350+140);
            sf::Text torch("SPACE - use torch",font,50);
            torch.setColor(Color::White);
            torch.setPosition(1280/2-torch.getGlobalBounds().width/2,350+210);
            sf::Text escapeText("ESC - exit",font,50);
            escapeText.setColor(Color::White);
            escapeText.setPosition(1280/2-escapeText.getGlobalBounds().width/2,350+280);

            window.clear();
            window.draw(backgroundGame);
            window.draw(titleGame);
            window.draw(moveText);
            window.draw(gps);
            window.draw(torch);
            window.draw(escapeText);
            DrawEyesAnimation();
            window.display();

        }
    }

    void Game::DrawEyesAnimation()
    {
        eyesEnemySprite.setPosition(495,160);
        if(clock.getElapsedTime()<sf::seconds(3) )
        {
            window.draw(eyesEnemySprite);
        }

        if(clock.getElapsedTime()>sf::seconds(3.5))
        {
            clock.restart();
        }
    }

    void Game::OpenWindowCredits()
    {
        Event event;
        while(state==CREDIT)
        {
            while(window.pollEvent(event))
            {
                if(event.type==Event::Closed || event.type==Event::KeyPressed &&
                   event.key.code==Keyboard::Escape)
                {
                    state=MENU;
                }
            }

            Text authorText("Authors",font,45);
            authorText.setColor(Color::White);
            authorText.setPosition(1280/2-authorText.getGlobalBounds().width/2,300+50);

            Text lead("Lead programmer and other things",font,40);
            lead.setColor(Color::White);
            lead.setPosition(1280/2-lead.getGlobalBounds().width/2,300+100);
            Text Mat("Mat",font,30);
            Mat.setColor(Color::White);
            Mat.setPosition(1280/2-Mat.getGlobalBounds().width/2,300+150);
            Text tester("Game script and alpha tester",font,40);
            tester.setColor(Color::White);
            tester.setPosition(1280/2-tester.getGlobalBounds().width/2,300+200);
            Text Fof("FLP",font,30);
            Fof.setColor(Color::White);
            Fof.setPosition(1280/2-Fof.getGlobalBounds().width/2,300+250);
            Text thanks("Special thanks",font,40);
            thanks.setColor(Color::White);
            thanks.setPosition(1280/2-thanks.getGlobalBounds().width/2,300+300);
            Text TK("TK for testing my games",font,30);
            TK.setColor(Color::White);
            TK.setPosition(1280/2-TK.getGlobalBounds().width/2,300+350);
            window.clear();
            window.draw(backgroundGame);
            window.draw(titleGame);
            window.draw(authorText);
            window.draw(lead);
            window.draw(Mat);
            window.draw(tester);
            window.draw(Fof);
            window.draw(thanks);
            window.draw(TK);
            DrawEyesAnimation();
            window.display();
        }
    }
