#ifndef BOSS_H
#define BOSS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "SlimeProjectile.h"
#include "Fireball.h"

class Boss : public sf::Sprite {
public:
    Boss(float speed);

    void shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target);
    void shoot(std::vector<Fireball>& fireballs, const sf::Texture& fireballTexture, sf::Vector2f target);
    void step();
    void takeDamage(int damage);
    float getHealth() const;
    void add_animation_frame_right(const sf::IntRect& frame);
    void moveWithCollision(const sf::FloatRect& bounds, float dx, float dy);

private:
    float speed;
    float health;
    std::size_t current_frame;
    float animation_delay;
    sf::Clock animationClock;
    sf::Clock shootClock;
    std::vector<sf::IntRect> animation_frames_right;
};

#endif // BOSS_H
