#include "Boss.h"
#include "SFML/Graphics/Rect.hpp"
#include "utils.h"

Boss::Boss(int fps) : currentFrame(0), animationFps(fps), health(200), direction(Right) {}

void Boss::moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY) {
    sf::Vector2f oldPosition = getPosition();
    sf::Sprite::move(offsetX, offsetY);
    sf::FloatRect spriteBounds = getGlobalBounds();

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

void Boss::shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target) {
    if (shootClock.getElapsedTime().asSeconds() > 2.0f) { // Shoot every 2 seconds
        sf::Vector2f bossCenter = getPosition() + sf::Vector2f(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
        sf::Vector2f direction = normalize(target - bossCenter);
        SlimeProjectile projectile(projectileTexture, direction, 0.1f); // Set the speed as appropriate
        projectile.setPosition(bossCenter);
        slimeProjectiles.push_back(projectile);
        shootClock.restart();
    }
}

void Boss::add_animation_frame_right(const sf::IntRect& frame) {
    framesRight.push_back(frame);
}

void Boss::add_animation_frame_left(const sf::IntRect& frame) {
    framesLeft.push_back(frame);
}

void Boss::add_animation_frame_up(const sf::IntRect& frame) {
    framesUp.push_back(frame);
}

void Boss::add_animation_frame_down(const sf::IntRect& frame) {
    framesDown.push_back(frame);
}

void Boss::step() {
    frameTime += clock.restart();
    sf::Time timePerFrame = sf::seconds(1.0f / animationFps);

    while (frameTime >= timePerFrame) {
        frameTime -= timePerFrame;
        currentFrame = (currentFrame + 1) % getFrames().size();
        setTextureRect(getFrames()[currentFrame]);
    }
}

const std::vector<sf::IntRect>& Boss::getFrames() const {
    switch (direction) {
    case Right: return framesRight;
    case Left: return framesLeft;
    case Up: return framesUp;
    case Down: return framesDown;
    }
    return framesRight; // Default case
}

int Boss::getHealth() const {
    return health;
}

void Boss::setHealth(int hp) {
    health = hp;
}

void Boss::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0; // Ensure health doesn't go below 0
}

void Boss::heal(int amount) {
    health += amount;
}

Boss Boss::clone() const {
    Boss clone(*this);
    clone.clock.restart(); // Reset clock for the clone
    return clone;
}
