#include "Bullet.h"

Bullet::Bullet(const sf::Texture& texture, const sf::Vector2f& direction, float speed, float angle)
    : direction(direction), speed(speed) {
    setTexture(texture);
    setRotation(angle);  // Ustawienie rotacji
    // Opcjonalnie można ustawić rect tekstury, jeśli używasz części tekstury
    setTextureRect(sf::IntRect(44, 51, 33, 21));
}

void Bullet::update() {
    move(direction * speed);
}
