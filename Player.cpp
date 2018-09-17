#include "Player.h"
#include <Windows.h>
#include <iostream>

    Player::Player()
    {
        if(!texture.loadFromFile("data/indianajones2.png"))
        {
            MessageBox(NULL,"Texture not found!","Error",NULL);
            return;
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0,0,32,32));
        sprite.setOrigin(16,16);
        status=STOP;
        anim_clock.restart();
    }

    Player::~Player()
    {
    }

    void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(sprite);
    }

    void Player::Stop()
    {
        status=STOP;
        framex=0;
    }

    void Player::Update(sf::Time delta)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            sprite.move(0,-speed*delta.asSeconds());
            status=UP;
            framey=3;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            sprite.move(0,speed*delta.asSeconds());
            status=DOWN;
            framey=0;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            sprite.move(speed*delta.asSeconds(),0);
            status=RIGHT;
            framey=2;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            sprite.move(-speed*delta.asSeconds(),0);
            status=LEFT;
            framey=1;
        }

        if(anim_clock.getElapsedTime()>sf::seconds(0.09f))
        {
            if(status==STOP)
                return;

            if(framex<3)
            {
                framex++;
            }
            else
            {
                framex=0;
            }
            sprite.setTextureRect(sf::IntRect(framex*32,framey*32,32,32));
            anim_clock.restart();
        }
    }
