#include "ninja.h"
#include <cmath>

Ninja::Ninja(int fps) : currentFrame(0), animationFps(fps), health(100), direction(Right), teleportTimer(), frameTime(sf::Time::Zero), isDisappearing(false), isReappearing(false) {}

void Ninja::moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY) {
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

void Ninja::createClone(const sf::Texture* ninja_texture) {
    Ninja clone(*this);
    clone.setTexture(*ninja_texture);
    clone.setHealth(25); // Clones have less health
    clones.push_back(std::make_pair(clone, clock.getElapsedTime()));
}

void Ninja::updateClones() {
    sf::Time currentTime = clock.getElapsedTime();
    clones.erase(std::remove_if(clones.begin(), clones.end(),
                                [currentTime](const std::pair<Ninja, sf::Time>& clone) {
                                    return (currentTime - clone.second).asSeconds() > 3.0f;
                                }),
                 clones.end());
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

void Ninja::add_disappear_frame(const sf::IntRect& frame) {
    framesDisappear.push_back(frame);
}

void Ninja::add_reappear_frame(const sf::IntRect& frame) {
    framesReappear.push_back(frame);
}

void Ninja::addNinjaAnimationFrames() {
    std::vector<sf::IntRect> frames = {
        sf::IntRect(6, 100, 17, 26),
        sf::IntRect(39, 100, 17, 26),
        sf::IntRect(71, 100, 17, 26),
        sf::IntRect(103, 100, 17, 26),
        sf::IntRect(134, 100, 17, 26),
        sf::IntRect(167, 100, 17, 26),
        sf::IntRect(199, 100, 17, 26),
        sf::IntRect(231, 100, 17, 26)
    };

    for (const auto& frame : frames) {
        add_animation_frame_right(frame);
    }

    std::vector<sf::IntRect> disappearFrames = {
        sf::IntRect(6, 196, 18, 28),
        sf::IntRect(38, 198, 18, 26),
        sf::IntRect(69, 200, 20, 25)
    };

    for (const auto& frame : disappearFrames) {
        add_disappear_frame(frame);
    }

    std::vector<sf::IntRect> reappearFrames = {
        sf::IntRect(69, 200, 20, 25),
        sf::IntRect(38, 198, 18, 26),
        sf::IntRect(6, 196, 18, 28)
    };

    for (const auto& frame : reappearFrames) {
        add_reappear_frame(frame);
    }
}

void Ninja::step(const sf::Vector2f& heroPosition, const sf::FloatRect& windowBounds) {
    frameTime += clock.restart();
    sf::Time timePerFrame = sf::seconds(1.0f / animationFps);

    while (frameTime >= timePerFrame) {
        frameTime -= timePerFrame;

        if (isDisappearing) {
            currentFrame = (currentFrame + 1) % framesDisappear.size();
            setTextureRect(framesDisappear[currentFrame]);
            if (currentFrame == static_cast<int>(framesDisappear.size()) - 1) {
                isDisappearing = false;
                teleport(heroPosition, windowBounds);
                isReappearing = true;
                currentFrame = 0;
            }
        } else if (isReappearing) {
            currentFrame = (currentFrame + 1) % framesReappear.size();
            setTextureRect(framesReappear[currentFrame]);
            if (currentFrame == static_cast<int>(framesReappear.size()) - 1) {
                isReappearing = false;
                currentFrame = 0;
            }
        } else {
            currentFrame = (currentFrame + 1) % getFrames().size();
            setTextureRect(getFrames()[currentFrame]);
        }
    }

    // Handle teleportation and clone creation
    if (!isDisappearing && !isReappearing && teleportTimer.getElapsedTime().asSeconds() >= 5.0f) {
        isDisappearing = true;
        createClone(getTexture()); // Create a clone
        currentFrame = 0;
        teleportTimer.restart();
    }

    updateClones();
}

const std::vector<sf::IntRect>& Ninja::getFrames() const {
    switch (direction) {
    case Right: return framesRight;
    case Left: return framesLeft;
    case Up: return framesUp;
    case Down: return framesDown;
    }
    return framesRight; // Default case
}

int Ninja::getHealth() const {
    return health;
}

void Ninja::setHealth(int hp) {
    health = hp;
}

void Ninja::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0; // Ensure health doesn't go below 0
}

void Ninja::heal(int amount) {
    health += amount;
}

bool Ninja::isFarEnoughninja(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance) {
    float distance = std::sqrt((position.x - heroPosition.x) * (position.x - heroPosition.x) + (position.y - heroPosition.y) * (position.y - heroPosition.y));
    return distance >= minDistance;
}

void Ninja::createNinja(std::vector<Ninja>& ninjas, sf::Texture& ninja_texture, sf::RenderWindow& window, const sf::Vector2f& heroPosition) {
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
    } while (!Ninja::isFarEnoughninja(newPosition, heroPosition, 75.0f));

    Ninja ninja(3);
    ninja.setTexture(ninja_texture);
    ninja.addNinjaAnimationFrames();
    ninja.setTextureRect(sf::IntRect(6, 100, 17, 26));
    ninja.setScale(1, 1);
    ninja.setPosition(newPosition);
    ninja.step(heroPosition, sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    ninjas.push_back(ninja);
}

void Ninja::checkHeroNinjaCollisions(std::vector<Ninja>& ninjas, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText) {
    for (auto& ninja : ninjas) {
        if (hero.getGlobalBounds().intersects(ninja.getGlobalBounds())) {
            if (!invulnerable) {
                health -= damage * 5;
                updateHealthText();
                if (health <= 0.0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }
}

void Ninja::teleport(const sf::Vector2f& heroPosition, const sf::FloatRect& bounds) {
    const int margin = 50;
    int maxX = bounds.width - margin;
    int maxY = bounds.height - margin;
    int minX = bounds.left + margin;
    int minY = bounds.top + margin;

    sf::Vector2f newPosition;
    do {
        int posX = rand() % (maxX - minX + 1) + minX;
        int posY = rand() % (maxY - minY + 1) + minY;
        newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
    } while (!isFarEnoughninja(newPosition, heroPosition, 50.0f));

    setPosition(newPosition);
}

std::vector<Ninja> Ninja::getClones() const {
    std::vector<Ninja> cloneNinjas;
    for (const auto& clone : clones) {
        cloneNinjas.push_back(clone.first);
    }
    return cloneNinjas;
}
