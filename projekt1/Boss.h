#ifndef BOSS_H
#define BOSS_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Include SFML Audio module
#include <vector>
#include "SlimeProjectile.h"
#include "Fireball.h"
#include <functional>

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
    bool shouldBeRemoved() const;
    bool isDying() const;
    bool isDead() const;
    bool canSpawnNewEnemies() const;

    static void addBossAnimationFrames(Boss& boss);
    static void createBoss(std::vector<Boss>& bosses, sf::Texture& boss_texture, sf::RenderWindow& window);
    static void checkHeroBossCollisions(std::vector<Boss>& bosses, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText);

private:
    float speed;
    float health;
    std::size_t current_frame;
    float animation_delay;
    sf::Clock animationClock;
    sf::Clock shootClock;
    sf::Clock deathClock;
    std::vector<sf::IntRect> animation_frames_right;
    std::vector<sf::IntRect> death_animation_frames;
    bool dying;
    bool dead;
    float deathAnimationDuration;
    float postDeathDelay;

    static bool isFarEnough(const sf::Vector2f& pos1, float minDistance);
};

#endif // BOSS_H
