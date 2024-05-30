#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite {
public:
    Bullet(const sf::Texture& texture, const sf::Vector2f& direction, float speed);

    void update();

private:
    sf::Vector2f direction;
    float speed;

    void setRotationBasedOnDirection();
};

#endif // BULLET_H
