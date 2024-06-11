#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SFML/Graphics.hpp>

class GameObject : public sf::Sprite {
public:
    // Konstruktor domyślny
    GameObject() = default;

    // Konstruktor przyjmujący teksturę
    GameObject(const sf::Texture& texture) {
        setTexture(texture);
    }

    // Wirtualny destruktor
    virtual ~GameObject() = default;

    // Wirtualna metoda aktualizacji obiektu
    virtual void update(float deltaTime) = 0;

    // Metoda ustawiająca skalę obiektu
    virtual void setScale(float x, float y) {
        sf::Sprite::setScale(x, y);
    }
};

#endif // GAME_OBJECT_H
