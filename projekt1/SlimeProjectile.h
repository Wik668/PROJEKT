#ifndef SLIMEPROJECTILE_H
#define SLIMEPROJECTILE_H

#include <SFML/Graphics.hpp>
#include <cmath>

class SlimeProjectile : public sf::Sprite {
public:
    SlimeProjectile(const sf::Texture& texture, sf::Vector2f direction, float speed)
        : direction(direction), speed(speed) {
        setTexture(texture);
        setScale(0.05f, 0.05f);
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

#endif // SLIMEPROJECTILE_H
