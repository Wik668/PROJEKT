#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <sstream>
#include <iomanip>
#include <SFML/System/Vector2.hpp> // Include the necessary header
#include <cmath>
using namespace std;
using namespace sf;

class Zombie : public Sprite {
private:
    vector<IntRect> framesRight;
    vector<IntRect> framesLeft;
    vector<IntRect> framesUp;
    vector<IntRect> framesDown;
    int currentFrame;
    int animationFps;
    Time frameTime;
    Clock clock;
    enum Direction { Up, Down, Left, Right } direction;


public:
    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY) {
        sf::Vector2f oldPosition = getPosition();
        sf::Sprite::move(offsetX, offsetY);
        sf::FloatRect spriteBounds = getGlobalBounds();

        if (spriteBounds.left < bounds.left ||
            spriteBounds.top < bounds.top ||
            spriteBounds.left + spriteBounds.width > bounds.left + bounds.width ||
            spriteBounds.top + spriteBounds.height > bounds.top + bounds.height) {
            setPosition(oldPosition);
        }
    }

    Zombie(int fps) : currentFrame(0), animationFps(fps), direction(Right) {}

    void add_animation_frame_right(const IntRect& frame) {
        framesRight.push_back(frame);
    }

    void add_animation_frame_left(const IntRect& frame) {
        framesLeft.push_back(frame);
    }

    void add_animation_frame_up(const IntRect& frame) {
        framesUp.push_back(frame);
    }

    void add_animation_frame_down(const IntRect& frame) {
        framesDown.push_back(frame);
    }

    void add_standing_frame_right(const IntRect& frame) {
        framesRight.push_back(frame);
    }

    void add_standing_frame_left(const IntRect& frame) {
        framesLeft.push_back(frame);
    }

    void add_standing_frame_up(const IntRect& frame) {
        framesUp.push_back(frame);
    }

    void add_standing_frame_down(const IntRect& frame) {
        framesDown.push_back(frame);
    }

    void step() {
        frameTime += clock.restart();
        Time timePerFrame = seconds(1.0f / animationFps);

        while (frameTime >= timePerFrame) {
            frameTime -= timePerFrame;
            currentFrame = (currentFrame + 1) % (getFrames().size() - 1); // Skip standing frame
            setTextureRect(getFrames()[currentFrame]);
        }
    }


private:
    const vector<IntRect>& getFrames() const {
        switch (direction) {
        case Right: return framesRight;
        case Left: return framesLeft;
        case Up: return framesUp;
        case Down: return framesDown;
        }
        // To avoid warning, returning framesRight as a default case.
        return framesRight;
    }
};
