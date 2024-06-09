#ifndef SLIME_H
#define SLIME_H

#include "SlimeProjectile.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Include SFML Audio module
#include <vector>
#include <functional>

class Slime : public sf::Sprite {
public:
    enum Direction { Right, Left, Up, Down };

    Slime(int fps);

    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY);

    void add_animation_frame_right(const sf::IntRect& frame);
    void add_animation_frame_left(const sf::IntRect& frame);
    void add_animation_frame_up(const sf::IntRect& frame);
    void add_animation_frame_down(const sf::IntRect& frame);
    void addSlimeAnimationFrames();

    void step();

    int getHealth() const;
    void setHealth(int hp);
    void takeDamage(int damage);
    void heal(int amount);

    Slime clone() const;

    void shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target);
    static void createSlime(std::vector<Slime>& slimes, sf::Texture& slime_texture, sf::RenderWindow& window, const sf::Vector2f& heroPosition);
    static void checkHeroSlimeCollisions(std::vector<Slime>& slimes, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText);

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

    static bool isFarEnough(const sf::Vector2f& pos1, const sf::Vector2f& pos2, float minDistance);
};

#endif // SLIME_H
