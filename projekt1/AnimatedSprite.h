#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class AnimatedSprite : public Sprite {
public:
    enum Direction { Right, Left, Up, Down };

    AnimatedSprite(int fps);

    void add_animation_frame_right(const IntRect& frame);
    void add_animation_frame_left(const IntRect& frame);
    void add_animation_frame_up(const IntRect& frame);
    void add_animation_frame_down(const IntRect& frame);

    void add_standing_frame_right(const IntRect& frame);
    void add_standing_frame_left(const IntRect& frame);
    void add_standing_frame_up(const IntRect& frame);
    void add_standing_frame_down(const IntRect& frame);

    void step();
    void moveWithCollision(const FloatRect& bounds, float offsetX, float offsetY);
    void initializeHero(const Texture& character_texture);

private:
    vector<IntRect> framesRight;
    vector<IntRect> framesLeft;
    vector<IntRect> framesUp;
    vector<IntRect> framesDown;
    Clock clock;
    Time frameTime;
    int currentFrame;
    int animationFps;
    Direction direction;

    const vector<IntRect>& getFrames() const;
};
