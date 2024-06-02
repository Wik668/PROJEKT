#ifndef BOSS_H
#define BOSS_H

#include "SlimeProjectile.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Boss : public sf::Sprite {
public:
    enum Direction { Right, Left, Up, Down };

    Boss(int fps);

    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY);

    void add_animation_frame_right(const sf::IntRect& frame);
    void add_animation_frame_left(const sf::IntRect& frame);
    void add_animation_frame_up(const sf::IntRect& frame);
    void add_animation_frame_down(const sf::IntRect& frame);

    void step();

    int getHealth() const;
    void setHealth(int hp);
    void takeDamage(int damage);
    void heal(int amount);

    Boss clone() const;

    void shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target);

private:
    sf::Clock shootClock;

    std::vector<sf::IntRect> framesRight;
    std::vector<sf::IntRect> framesLeft;
    std::vector<sf::IntRect> framesUp;
    std::vector<sf::IntRect> framesDown;

    const std::vector<sf::IntRect>& getFrames() const;

    int currentFrame;
    int animationFps;
    int health;
    int speed;
    Direction direction;
    sf::Clock clock;
    sf::Time frameTime;
};

#endif // BOSS_H
