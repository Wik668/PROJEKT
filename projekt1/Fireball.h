#ifndef FIREBALL_H
#define FIREBALL_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Fireball : public sf::Sprite {
public:
    Fireball(const sf::Texture& texture, sf::Vector2f direction, float speed)
        : direction(direction), speed(speed) {
        setTexture(texture);
        setScale(0.07f, 0.07f); // Możesz dostosować skalę w zależności od rozmiaru tekstury
    }

    void update() {
        move(direction * speed);
        // Obliczamy kąt między kierunkiem pocisku a wektorem horyzontalnym
        float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265;
        // Ustawiamy rotację pocisku
        setRotation(angle);
    }

private:
    sf::Vector2f direction;
    float speed;
};

#endif // FIREBALL_H
