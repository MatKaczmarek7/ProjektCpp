#include "Enemy.h"

#include <Windows.h>
#include <iostream>

Enemy::Enemy()
{
    if(!texture.loadFromFile("data/potwor.png"))
    {
        MessageBox(NULL,"Texture not found!","Error",NULL);
        return;
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,30,30));
    sprite.setOrigin(15,15);
    sprite.setPosition(0,0);
    anim_clock.restart();
    status=STOP;
}

Enemy::~Enemy()
{

}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(sprite);
}

void Enemy::Update()
{
    if(anim_clock.getElapsedTime()>sf::seconds(0.09f))
    {
        if(status==STOP)return;

        if(framex<3)
        {
            framex++;
        }
        else
        {
            framex=0;
        }
        sprite.setTextureRect(sf::IntRect(framex*30,framey*30,30,30));
        anim_clock.restart();
    }
}

int Enemy::GetState()
{
    if(status==UP)
    {
        return 0;
    }
    if(status==DOWN)
    {
        return 1;
    }
    if(status==LEFT)
    {
        return 2;
    }
    if(status==RIGHT)
    {
        return 3;
    }
}

void Enemy::SetStatus(int a)
{
        if(a==0)
        {
            status=UP;
            framey=3;
            return;
        }

        if(a==1)
        {
            status=DOWN;
            framey=0;
            return;
        }

        if(a==2)
        {
             status=LEFT;
             framey=1;
             return;
        }

        if(a==3)
        {
            status=RIGHT;
            framey=2;
            return;
        }
}
