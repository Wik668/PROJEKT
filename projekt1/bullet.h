#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

// Klasa Bullet dziedzicząca po sf::Sprite
class Bullet : public sf::Sprite {
public:
    // Konstruktor klasy Bullet
    Bullet(const sf::Texture& texture, const sf::Vector2f& direction, float speed, float angle);

    // Aktualizacja pozycji pocisku
    void update();

private:
    sf::Vector2f direction; // Kierunek ruchu pocisku
    float speed;            // Prędkość pocisku
};

#endif // BULLET_H
