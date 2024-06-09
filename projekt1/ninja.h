#ifndef NINJA_H
#define NINJA_H

#include "SFML/Audio/Music.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class Ninja : public sf::Sprite {
public:
    enum Direction { Right, Left, Up, Down };
    Ninja(int fps);

    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY);
    void add_animation_frame_right(const sf::IntRect& frame);
    void add_animation_frame_left(const sf::IntRect& frame);
    void add_animation_frame_up(const sf::IntRect& frame);
    void add_animation_frame_down(const sf::IntRect& frame);
    void add_disappear_frame(const sf::IntRect& frame);
    void add_reappear_frame(const sf::IntRect& frame);
    void addNinjaAnimationFrames();
    void step(const sf::Vector2f& heroPosition, const sf::FloatRect& windowBounds);
    int getHealth() const;
    void setHealth(int hp);
    void takeDamage(int damage);
    void heal(int amount);
    static bool isFarEnoughninja(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance);
    static void createNinja(std::vector<Ninja>& ninjas, sf::Texture& ninja_texture, sf::RenderWindow& window, const sf::Vector2f& heroPosition);
    static void checkHeroNinjaCollisions(std::vector<Ninja>& ninjas, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText);

    void createClone(const sf::Texture* ninja_texture);
    void updateClones();
    std::vector<Ninja> getClones() const; // Accessor for clones

private:
    std::vector<sf::IntRect> framesRight;
    std::vector<sf::IntRect> framesLeft;
    std::vector<sf::IntRect> framesUp;
    std::vector<sf::IntRect> framesDown;
    std::vector<sf::IntRect> framesDisappear;
    std::vector<sf::IntRect> framesReappear;

    std::vector<std::pair<Ninja, sf::Time>> clones; // Store clones with their creation time

    int currentFrame;
    int animationFps;
    int health;
    Direction direction;
    sf::Clock clock;
    sf::Clock teleportTimer;
    sf::Time frameTime;
    bool isDisappearing;
    bool isReappearing;

    const std::vector<sf::IntRect>& getFrames() const;
    void teleport(const sf::Vector2f& heroPosition, const sf::FloatRect& bounds);
};

#endif // NINJA_H
