#include "Zombie.h"
#include "SFML/Graphics/Rect.hpp"
#include <cmath>
#include <functional>

// Konstruktor klasy Zombie
Zombie::Zombie(int fps) : currentFrame(0), animationFps(fps), health(100), direction(Right) {}

// Metoda przesuwająca postać zombie z uwzględnieniem kolizji
void Zombie::moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY) {
    sf::Vector2f oldPosition = getPosition();
    sf::Sprite::move(offsetX, offsetY);
    sf::FloatRect spriteBounds = getGlobalBounds();

    // Sprawdzanie kolizji z krawędziami bounds
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

// Metody dodające klatki animacji dla różnych kierunków ruchu
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

// Metoda aktualizująca stan zombie
void Zombie::step() {
    frameTime += clock.restart();
    sf::Time timePerFrame = sf::seconds(1.0f / animationFps);

    while (frameTime >= timePerFrame) {
        frameTime -= timePerFrame;
        currentFrame = (currentFrame + 1) % getFrames().size();
        setTextureRect(getFrames()[currentFrame]);
    }
}

// Metoda zwracająca odpowiednie klatki animacji w zależności od kierunku ruchu
const std::vector<sf::IntRect>& Zombie::getFrames() const {
    switch (direction) {
    case Right: return framesRight;
    case Left: return framesLeft;
    case Up: return framesUp;
    case Down: return framesDown;
    }
    return framesRight; // Domyślny przypadek
}

// Metody get/set zdrowia zombie
int Zombie::getHealth() const {
    return health;
}

void Zombie::setHealth(int hp) {
    health = hp;
}

// Metody obsługujące obrażenia i leczenie zombie
void Zombie::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0; // Zapewnienie, że zdrowie nie spadnie poniżej 0
}

void Zombie::heal(int amount) {
    health += amount;
}

// Metoda klonująca zombie
Zombie Zombie::clone() const {
    Zombie clone(*this);
    clone.clock.restart(); // Resetowanie zegara dla klona
    return clone;
}

// Metoda dodająca klatki animacji zombie
void Zombie::addZombieAnimationFrames() {
    std::vector<sf::IntRect> frames = {
        sf::IntRect(11, 69, 20, 26),
        sf::IntRect(42, 69, 20, 26),
        sf::IntRect(74, 69, 20, 26),
        sf::IntRect(107, 69, 20, 26),
        sf::IntRect(140, 69, 20, 26),
        sf::IntRect(173, 69, 20, 26),
        sf::IntRect(204, 69, 20, 26),
        sf::IntRect(269, 69, 20, 26),
        sf::IntRect(300, 69, 20, 26)
    };

    for (const auto& frame : frames) {
        add_animation_frame_right(frame);
    }
}

// Funkcja sprawdzająca, czy dwa punkty są wystarczająco daleko od siebie
bool Zombie::isFarEnough(const sf::Vector2f& pos1, const sf::Vector2f& pos2, float minDistance) {
    return std::hypot(pos1.x - pos2.x, pos1.y - pos2.y) > minDistance;
}

// Metoda tworząca nowego zombie
void Zombie::createZombie(std::vector<std::unique_ptr<Zombie>>& zombies, sf::Texture& zombie_texture, sf::RenderWindow& window) {
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
    } while (!Zombie::isFarEnough(newPosition, {0, 0}, 75.0f)); // Zakładając {0, 0} jako miejsce bohatera

    auto zombie = std::make_unique<Zombie>(5);
    zombie->setTexture(zombie_texture);
    zombie->addZombieAnimationFrames();
    zombie->setTextureRect(sf::IntRect(11, 69, 14, 26));
    zombie->setScale(2, 2);
    zombie->setPosition(newPosition);
    zombie->step();
    zombies.push_back(std::move(zombie));
}

// Metoda sprawdzająca kolizje zombie z bohaterem
void Zombie::checkHeroZombieCollisions(std::vector<std::unique_ptr<Zombie>>& zombies, sf::Sprite& hero, float& health, bool invulnerable, float damage, bool& gameEnded, sf::Music& gameMusic, std::function<void()> updateHealthText) {
    for (auto& zombie : zombies) {
        if (hero.getGlobalBounds().intersects(zombie->getGlobalBounds())) {
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
