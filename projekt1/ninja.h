#ifndef NINJA_H
#define NINJA_H

#include "utils.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;


class Ninja :public Sprite
{
private:
    const vector<IntRect>& getFrames() const;
    vector<IntRect> framesRight;
    vector<IntRect> framesLeft;
    vector<IntRect> framesUp;
    vector<IntRect> framesDown;
    int currentFrame;
    int animationFps;
    Time frameTime;
    Clock clock;
    int health;  // Health attribute
    enum Direction { Up, Down, Left, Right } direction;
public:
    Ninja(int fps);
    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY);
    void add_animation_frame_right(const IntRect& frame);
    void add_animation_frame_left(const IntRect& frame);
    void add_animation_frame_up(const IntRect& frame);
    void add_animation_frame_down(const IntRect& frame);
    void add_standing_frame_right(const IntRect& frame);
    void add_standing_frame_left(const IntRect& frame);
    void add_standing_frame_up(const IntRect& frame);
    void add_standing_frame_down(const IntRect& frame);
    void step();

    // Health-related methods
    int getHealth() const;
    void setHealth(int hp);
    void takeDamage(int damage);
    void heal(int amount);
};

#endif // NINJA_H
