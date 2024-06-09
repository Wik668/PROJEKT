#include "Zombie.h"
#include "SFML/Graphics/Rect.hpp"
#include <cmath>
#include <functional>

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

void Zombie::addZombieAnimationFrames() {
    std::vector<sf::IntRect> frames = {
        sf::IntRect(11, 69, 14, 26),
        sf::IntRect(42, 69, 14, 26),
        sf::IntRect(74, 69, 14, 26),
        sf::IntRect(107, 69, 14, 26),
        sf::IntRect(140, 69, 14, 26),
        sf::IntRect(173, 69, 14, 26),
        sf::IntRect(204, 69, 14, 26),
        sf::IntRect(231, 100, 17, 26),
        sf::IntRect(269, 69, 14, 26),
        sf::IntRect(300, 69, 14, 26)
    };

    for (const auto& frame : frames) {
        add_animation_frame_right(frame);
    }
}

bool Zombie::isFarEnough(const sf::Vector2f& pos1, const sf::Vector2f& pos2, float minDistance) {
    return std::hypot(pos1.x - pos2.x, pos1.y - pos2.y) > minDistance;
}

void Zombie::createZombie(std::vector<Zombie>& zombies, sf::Texture& zombie_texture, sf::RenderWindow& window) {
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
    } while (!Zombie::isFarEnough(newPosition, {0, 0}, 75.0f)); // Assuming {0, 0} as a placeholder for hero position

    Zombie zombie(5);
    zombie.setTexture(zombie_texture);
    zombie.addZombieAnimationFrames();
    zombie.setTextureRect(sf::IntRect(11, 69, 14, 26));
    zombie.setScale(2, 2);
    zombie.setPosition(newPosition);
    zombie.step();
    zombies.push_back(zombie);
}

void Zombie::checkHeroZombieCollisions(std::vector<Zombie>& zombies, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText) {
    for (auto& zombie : zombies) {
        if (hero.getGlobalBounds().intersects(zombie.getGlobalBounds())) {
            if (!invulnerable) {
                health -= damage;
                updateHealthText();
                if (health <= 0.0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }
}
