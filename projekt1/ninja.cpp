#include "ninja.h"
#include <cmath>

Ninja::Ninja(int fps) : currentFrame(0), animationFps(fps), direction(Right), health(100) {}

void Ninja::moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY) {
    sf::Vector2f oldPosition = getPosition();
    sf::Sprite::move(offsetX, offsetY);
    sf::FloatRect spriteBounds = getGlobalBounds();

    // Check for collisions with the edges of the bounds
    if (spriteBounds.left < bounds.left) {
        setPosition(bounds.left, oldPosition.y);
    } else if (spriteBounds.top < bounds.top) {
        setPosition(oldPosition.x, bounds.top);
    } else if (spriteBounds.left + spriteBounds.width > bounds.left + bounds.width) {
        setPosition(bounds.left + bounds.width - spriteBounds.width, oldPosition.y);
    } else if (spriteBounds.top + spriteBounds.height > bounds.top + bounds.height) {
        setPosition(oldPosition.x, bounds.top + bounds.height - spriteBounds.height);
    }
}

void Ninja::add_animation_frame_right(const sf::IntRect& frame) {
    framesRight.push_back(frame);
}

void Ninja::add_animation_frame_left(const sf::IntRect& frame) {
    framesLeft.push_back(frame);
}

void Ninja::add_animation_frame_up(const sf::IntRect& frame) {
    framesUp.push_back(frame);
}

void Ninja::add_animation_frame_down(const sf::IntRect& frame) {
    framesDown.push_back(frame);
}

void Ninja::add_standing_frame_right(const sf::IntRect& frame) {
    framesRight.push_back(frame);
}

void Ninja::add_standing_frame_left(const sf::IntRect& frame) {
    framesLeft.push_back(frame);
}

void Ninja::add_standing_frame_up(const sf::IntRect& frame) {
    framesUp.push_back(frame);
}

void Ninja::add_standing_frame_down(const sf::IntRect& frame) {
    framesDown.push_back(frame);
}

void Ninja::step() {
    frameTime += clock.restart();
    sf::Time timePerFrame = sf::seconds(1.0f / animationFps);

    while (frameTime >= timePerFrame) {
        frameTime -= timePerFrame;
        currentFrame = (currentFrame + 1) % (getFrames().size() - 1); // Skip standing frame
        setTextureRect(getFrames()[currentFrame]);
    }
}

const std::vector<sf::IntRect>& Ninja::getFrames() const {
    switch (direction) {
    case Right: return framesRight;
    case Left: return framesLeft;
    case Up: return framesUp;
    case Down: return framesDown;
    }
    // To avoid warning, returning framesRight as a default case.
    return framesRight;
}

// Health-related methods
int Ninja::getHealth() const {
    return health;
}

void Ninja::setHealth(int hp) {
    health = hp;
}

void Ninja::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;  // Ensure health doesn't go below 0
}

void Ninja::heal(int amount) {
    health += amount;
    // Optionally, you could add a maximum health limit
}
