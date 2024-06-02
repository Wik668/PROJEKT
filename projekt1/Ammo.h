#ifndef AMMO_H
#define AMMO_H

#include <SFML/Graphics.hpp>

class Ammo : public sf::Sprite {
public:
    Ammo(float x, float y, const sf::Texture& texture) {
        setTexture(texture);
        setPosition(x, y);
        setScale(0.05f, 0.05f); // Adjust the scale as necessary
    }
};

#endif // AMMO_H
