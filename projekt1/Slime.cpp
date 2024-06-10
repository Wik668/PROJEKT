#include "Slime.h"
#include "SFML/Graphics/Rect.hpp"
#include "utils.h"
#include <cmath>
#include <functional>

Slime::Slime(int fps) : currentFrame(0), animationFps(fps), health(100), direction(Right) {}

void Slime::moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY) {
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

void Slime::shoot(std::vector<SlimeProjectile>& slimeProjectiles, const sf::Texture& projectileTexture, sf::Vector2f target) {
    if (shootClock.getElapsedTime().asSeconds() > 4.0f) {
        sf::Vector2f slimeCenter = getPosition() + sf::Vector2f(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
        sf::Vector2f direction = normalize(target - slimeCenter);
        SlimeProjectile projectile(projectileTexture, direction, 0.1f);
        projectile.setPosition(slimeCenter);
        slimeProjectiles.push_back(projectile);
        shootClock.restart();
    }
}

void Slime::add_animation_frame_right(const sf::IntRect& frame) {
    framesRight.push_back(frame);
}

void Slime::add_animation_frame_left(const sf::IntRect& frame) {
    framesLeft.push_back(frame);
}

void Slime::add_animation_frame_up(const sf::IntRect& frame) {
    framesUp.push_back(frame);
}

void Slime::add_animation_frame_down(const sf::IntRect& frame) {
    framesDown.push_back(frame);
}

void Slime::addSlimeAnimationFrames() {
    std::vector<sf::IntRect> frames = {
        sf::IntRect(3, 4, 38, 26),
        sf::IntRect(46, 5, 38, 26),
        sf::IntRect(88, 5, 38, 26),
        sf::IntRect(133, 5, 38, 26),
        sf::IntRect(178, 5, 38, 26),
        sf::IntRect(223, 5, 38, 26),
        sf::IntRect(268, 2, 38, 26),
        sf::IntRect(313, 1, 38, 26),
        sf::IntRect(356, 1, 38, 26),
        sf::IntRect(400, 2, 38, 26)
    };

    for (const auto& frame : frames) {
        add_animation_frame_right(frame);
    }
}

void Slime::step() {
    frameTime += clock.restart();
    sf::Time timePerFrame = sf::seconds(1.0f / animationFps);

    while (frameTime >= timePerFrame) {
        frameTime -= timePerFrame;
        currentFrame = (currentFrame + 1) % getFrames().size();
        setTextureRect(getFrames()[currentFrame]);
    }
}

const std::vector<sf::IntRect>& Slime::getFrames() const {
    switch (direction) {
    case Right: return framesRight;
    case Left: return framesLeft;
    case Up: return framesUp;
    case Down: return framesDown;
    }
    return framesRight;
}

int Slime::getHealth() const {
    return health;
}

void Slime::setHealth(int hp) {
    health = hp;
}

void Slime::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Slime::heal(int amount) {
    health += amount;
}

Slime Slime::clone() const {
    Slime clone(*this);
    clone.clock.restart();
    return clone;
}

bool Slime::isFarEnough(const sf::Vector2f& pos1, const sf::Vector2f& pos2, float minDistance) {
    return std::hypot(pos1.x - pos2.x, pos1.y - pos2.y) > minDistance;
}

void Slime::createSlime(std::vector<Slime>& slimes, sf::Texture& slime_texture, sf::RenderWindow& window, const sf::Vector2f& heroPosition) {
    const int margin = 50;
    int maxX = window.getSize().x - margin;
    int maxY = window.getSize().y - margin;
    int minX = margin;
    int minY = margin;

    sf::Vector2f newPosition;
    do {
        int posX = rand() % (maxX - minX + 1) + minX;
        int posY = rand() % (maxY - minY + 1) + minY;
        newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
    } while (!Slime::isFarEnough(newPosition, heroPosition, 75.0f));

    Slime slime(5);
    slime.setTexture(slime_texture);
    slime.addSlimeAnimationFrames();
    slime.setTextureRect(sf::IntRect(3, 4, 38, 26));
    slime.setScale(2, 2);
    slime.setPosition(newPosition);
    slime.step();
    slimes.push_back(slime);
}

void Slime::checkHeroSlimeCollisions(std::vector<Slime>& slimes, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText) {
    for (auto& slime : slimes) {
        if (hero.getGlobalBounds().intersects(slime.getGlobalBounds())) {
            if (!invulnerable) {
                health -= damage * 0.01;
                updateHealthText();
                if (health <= 0.0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }
}
