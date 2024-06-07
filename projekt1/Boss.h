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
    void add_death_animation_frame(const sf::IntRect& frame);
    void moveWithCollision(const sf::FloatRect& bounds, float dx, float dy);
    void dieAnimation();
    bool isDying() const;
    bool isDead() const;
    bool canSpawnNewEnemies() const;

private:
    float speed;
    float health;
    std::size_t current_frame;
    float animation_delay;
    sf::Clock animationClock;
    sf::Clock shootClock;
    sf::Clock deathClock; // Clock to manage the death animation and delay
    std::vector<sf::IntRect> animation_frames_right;
    std::vector<sf::IntRect> death_animation_frames;
    bool dying;
    bool dead;
    float deathAnimationDuration; // Duration of the death animation in seconds
    float postDeathDelay; // Delay after death animation in seconds
};

#endif // BOSS_H
