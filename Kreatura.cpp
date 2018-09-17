#include "Kreatura.h"


sf::Vector2f Kreatura::GetCreaturePosition()
{
    return sprite.getPosition();
}

sf::FloatRect Kreatura::getBoundingBox()
{
	return sprite.getGlobalBounds();
}

void Kreatura::Setposition(float x , float y)
{
    sprite.setPosition(x,y);
}

void Kreatura::MoveYourself(int x, int y)
{
    sprite.move(x,y);
}
