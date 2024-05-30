#include "Bullet.h"
#include <cmath> // For std::atan2 and conversion to degrees

Bullet::Bullet(const sf::Texture& texture, const sf::Vector2f& direction, float speed)
    : direction(direction), speed(speed) {
    setTexture(texture);
    setRotationBasedOnDirection();
    // Optionally set texture rect if using a part of a texture
    // setTextureRect(sf::IntRect(0, 0, 10, 10));
}

void Bullet::update() {
    move(direction * speed);
}

void Bullet::setRotationBasedOnDirection() {
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265;
    setRotation(angle + 90); // +90 to adjust for the sprite's default orientation
}
