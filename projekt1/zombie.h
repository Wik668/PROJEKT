#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Graphics.hpp>
#include <vector>

class Zombie : public sf::Sprite {
public:
    enum Direction { Right, Left, Up, Down };

    Zombie(int fps);

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

    Zombie clone() const;

private:
    std::vector<sf::IntRect> framesRight;
    std::vector<sf::IntRect> framesLeft;
    std::vector<sf::IntRect> framesUp;
    std::vector<sf::IntRect> framesDown;

    const std::vector<sf::IntRect>& getFrames() const;

    int currentFrame;
    int animationFps;
    int health;
    Direction direction;
    sf::Clock clock;
    sf::Time frameTime;
};

#endif // ZOMBIE_H
