#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Kreatura.h"
#include <SFML\Graphics.hpp>
#include <vector>


class Enemy: public Kreatura, public sf::Drawable,
public sf::Transformable{


public:
           Enemy();
           ~Enemy();
           void Update();
           int GetState();
           void SetStatus(int a);
private:

       virtual void draw(sf:: RenderTarget &target, sf::RenderStates states) const;
       enum Status{
        UP,
        DOWN,
        LEFT,
        RIGHT,
        STOP};

        Status status;
};


#endif // ENEMY_H_INCLUDED
