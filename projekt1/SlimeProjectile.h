#ifndef SLIMEPROJECTILE_H
#define SLIMEPROJECTILE_H

#include <SFML/Graphics.hpp>
#include <cmath>

// Klasa SlimeProjectile dziedzicząca po sf::Sprite, reprezentująca pocisk slime w grze
class SlimeProjectile : public sf::Sprite {
public:
    // Konstruktor inicjalizujący pocisk slime z teksturą, kierunkiem i prędkością
    SlimeProjectile(const sf::Texture& texture, sf::Vector2f direction, float speed)
        : direction(direction), speed(speed) {
        setTexture(texture);
        setScale(0.05f, 0.05f);
    }

    // Metoda aktualizująca pozycję i rotację pocisku
    void update() {
        move(direction * speed);
        // Obliczamy kąt między kierunkiem pocisku a wektorem horyzontalnym
        float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265;
        // Ustawiamy rotację pocisku
        setRotation(angle);
    }

private:
    sf::Vector2f direction; // Kierunek pocisku
    float speed;            // Prędkość pocisku
};

#endif // SLIMEPROJECTILE_H
