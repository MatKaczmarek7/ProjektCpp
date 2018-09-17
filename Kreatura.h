#ifndef KREATURA_H_INCLUDED
#define KREATURA_H_INCLUDED

#include <SFML\Graphics.hpp>


class Kreatura{

public:

    sf::Vector2f GetCreaturePosition();
    void Setposition(float x, float y);
    sf::FloatRect getBoundingBox();
    void MoveYourself(int x, int y);


protected:
    sf::Sprite sprite;
    sf::Texture texture;
    size_t framex;
    size_t framey;
    sf::Clock anim_clock;
};


#endif // KREATURA_H_INCLUDED
