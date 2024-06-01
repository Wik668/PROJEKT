#include "zombie.h"

Zombie::Zombie(int fps) : currentFrame(0), animationFps(fps), health(100), direction(Right) {}

void Zombie::moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY) {
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

void Zombie::add_animation_frame_right(const sf::IntRect& frame) {
    framesRight.push_back(frame);
}

void Zombie::add_animation_frame_left(const sf::IntRect& frame) {
    framesLeft.push_back(frame);
}

void Zombie::add_animation_frame_up(const sf::IntRect& frame) {
    framesUp.push_back(frame);
}

void Zombie::add_animation_frame_down(const sf::IntRect& frame) {
    framesDown.push_back(frame);
}

void Zombie::step() {
    frameTime += clock.restart();
    sf::Time timePerFrame = sf::seconds(1.0f / animationFps);

    while (frameTime >= timePerFrame) {
        frameTime -= timePerFrame;
        currentFrame = (currentFrame + 1) % getFrames().size();
        setTextureRect(getFrames()[currentFrame]);
    }
}

const std::vector<sf::IntRect>& Zombie::getFrames() const {
    switch (direction) {
    case Right: return framesRight;
    case Left: return framesLeft;
    case Up: return framesUp;
    case Down: return framesDown;
    }
    return framesRight; // Default case
}

int Zombie::getHealth() const {
    return health;
}

void Zombie::setHealth(int hp) {
    health = hp;
}

void Zombie::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0; // Ensure health doesn't go below 0
}

void Zombie::heal(int amount) {
    health += amount;
}

Zombie Zombie::clone() const {
    Zombie clone(*this);
    clone.clock.restart(); // Reset clock for the clone
    return clone;
}
