#include "Bullet.h"

// Konstruktor klasy Bullet inicjalizujący pocisk z podaną teksturą, kierunkiem, prędkością i kątem
Bullet::Bullet(const sf::Texture& texture, const sf::Vector2f& direction, float speed, float angle)
    : direction(direction), speed(speed) {
    setTexture(texture);      // Ustawienie tekstury pocisku
    setRotation(angle);       // Ustawienie rotacji pocisku

    // Ustawienie wycinka tekstury, jeśli używasz tylko części tekstury
    setTextureRect(sf::IntRect(44, 51, 33, 21));
}

// Metoda aktualizująca pozycję pocisku na podstawie kierunku i prędkości
void Bullet::update() {
    move(direction * speed);
}
