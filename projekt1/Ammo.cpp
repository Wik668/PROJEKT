#include "Ammo.h"
#include <cstdlib>
#include <cmath>

Ammo::Ammo(float x, float y, const sf::Texture& texture) {
    setTexture(texture);
    setPosition(x, y);
    setScale(0.05f, 0.05f); // Corrected the scale call
}

bool Ammo::isFarEnough(const sf::Vector2f& position, const sf::Vector2f& heroPosition, float minDistance) {
    return std::abs(position.x - heroPosition.x) >= minDistance && std::abs(position.y - heroPosition.y) >= minDistance;
}

void Ammo::spawnAmmo(std::vector<Ammo>& ammoPacks, const sf::Texture& ammoTexture, const sf::RenderWindow& window, sf::Clock& ammoRespawnClock, const sf::Sprite& hero, float minDistance) {
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
    } while (!isFarEnough(newPosition, hero.getPosition(), minDistance));

    Ammo newAmmo(newPosition.x, newPosition.y, ammoTexture);
    ammoPacks.push_back(newAmmo);
    ammoRespawnClock.restart();
}

void Ammo::checkHeroAmmoCollisions(std::vector<Ammo>& ammoPacks, sf::Sprite& hero, bool& unlimitedAmmo, sf::Clock& unlimitedAmmoClock, bool& reloading, std::function<void()> updateAmmoText) {
    for (auto it = ammoPacks.begin(); it != ammoPacks.end();) {
        if (hero.getGlobalBounds().intersects(it->getGlobalBounds())) {
            unlimitedAmmo = true;
            unlimitedAmmoClock.restart();
            it = ammoPacks.erase(it); // Remove collected ammo
            if (reloading) {
                reloading = false;
                updateAmmoText();
            }
        } else {
            ++it;
        }
    }
}

void Ammo::setScale(float x, float y) {
    sf::Sprite::setScale(x, y);
}
