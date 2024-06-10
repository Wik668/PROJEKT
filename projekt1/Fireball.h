#ifndef FIREBALL_H
#define FIREBALL_H

#include <SFML/Graphics.hpp>
#include <cmath>

// Klasa Fireball dziedzicząca po sf::Sprite
class Fireball : public sf::Sprite {
public:
    // Konstruktor klasy Fireball
    Fireball(const sf::Texture& texture, sf::Vector2f direction, float speed)
        : direction(direction), speed(speed) {
        setTexture(texture);
        setScale(0.07f, 0.07f); // Możesz dostosować skalę w zależności od rozmiaru tekstury
    }

    // Aktualizacja pozycji i rotacji fireball
    void update() {
        move(direction * speed);
        // Obliczamy kąt między kierunkiem pocisku a wektorem horyzontalnym
        float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;
        // Ustawiamy rotację pocisku
        setRotation(angle);
    }

private:
    sf::Vector2f direction; // Kierunek ruchu pocisku
    float speed;            // Prędkość pocisku
};

#endif // FIREBALL_H
