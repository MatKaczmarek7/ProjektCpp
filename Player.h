#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Kreatura.h"
#include <SFML\Graphics.hpp>
#include <vector>

class Player: public Kreatura, public sf::Drawable,
public sf::Transformable
{
public:
    Player();
    ~Player();

    void Update(sf::Time delta);
    void Stop();
    int GetStatus();

private:
      enum Status{
        UP,
        DOWN,
        LEFT,
        RIGHT,
        STOP};
    Status status;
    int speed=200;

   virtual void draw(sf:: RenderTarget &target, sf::RenderStates states) const;

};

#endif
